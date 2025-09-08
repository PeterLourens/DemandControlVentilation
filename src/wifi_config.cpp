#include "wifi_config.h"

// open config file and check if SSID and password are not empty
// if empty start WIFI as access point
// else connect to access point defined in SSID with either DHCP or fixed

void config_wifi()
{
    const char *enable_dhcp;
    const char *ssid;
    const char *wifi_password;
    const char *ip_address;
    const char *subnet_mask;
    const char *gateway;
    const char *primary_dns;
    const char *secondary_dns;

    char msg[MSG_SIZE] = {};

    // Copy to local variables to prevent nested mutexes
    if (settings_network_mutex != NULL)
    {
        if (xSemaphoreTake(settings_network_mutex, (TickType_t)10))
        {
            enable_dhcp = networksettings.enable_dhcp;
            ssid = networksettings.ssid;
            wifi_password = networksettings.wifi_password;
            ip_address = networksettings.ip_address;
            subnet_mask = networksettings.subnet_mask;
            gateway = networksettings.gateway;
            primary_dns = networksettings.primary_dns;
            secondary_dns = networksettings.secondary_dns;
            xSemaphoreGive(settings_network_mutex);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "Failed to copy network settings to local variables. Check network configuration.");
        printmessage(LOG_ERROR, msg);
        const char *enable_dhcp = "";
        const char *ssid = "";
        const char *wifi_password = "";
        const char *ip_address = "";
        const char *subnet_mask = "";
        const char *gateway = "";
        const char *primary_dns = "";
        const char *secondary_dns = "";
    }

    if (ssid && wifi_password && strcmp(ssid, "") != 0 && strcmp(wifi_password, "") != 0)
    {
        if (strcmp(enable_dhcp, "On") == 0)
        {
            if (!setupDHCP(ssid, wifi_password))
                return;
        }
        else if (strcmp(enable_dhcp, "Off") == 0)
        {
            if (!setupStaticIP(ssid, wifi_password, ip_address, subnet_mask, gateway, primary_dns, secondary_dns))
                return;
        }
        else
        {
            snprintf(msg, sizeof(msg), "WiFi configuration failed.");
            printmessage(LOG_ERROR, msg);
        }
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("OSVENTILATION-WIFI", NULL);
        IPAddress IP = WiFi.softAPIP();

        snprintf(msg, sizeof(msg), "AP IP address: %s", IP.toString());
        printmessage(LOG_INFO, msg);

        if (ap_active_mutex != NULL)
        {
            if (xSemaphoreTake(ap_active_mutex, (TickType_t)10))
            {
                ap_active = 1;
                xSemaphoreGive(ap_active_mutex);
            }
        }

        if (WiFi.waitForConnectResult() == WL_CONNECTED && ap_active == 0)
        {
            sync_rtc_ntp();
        }
    }
}

bool setupDHCP(const char *ssid, const char *wifi_password)
{
    char msg[MSG_SIZE] = {};

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, wifi_password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        snprintf(msg, sizeof(msg), "WiFi Failed!");
        printmessage(LOG_ERROR, msg);
        return false;
    }

    if (ap_active_mutex != NULL)
    {
        if (xSemaphoreTake(ap_active_mutex, (TickType_t)10))
        {
            ap_active = 0;
            xSemaphoreGive(ap_active_mutex);
        }
    }

    return true;
}

bool setupStaticIP(const char *ssid, const char *wifi_password, const char *ip_address,
                   const char *subnet_mask, const char *gateway,
                   const char *primary_dns, const char *secondary_dns)
{
    const char *ip_addresses[5] = {ip_address, subnet_mask, gateway, primary_dns, secondary_dns};
    int **ip_address_numbers = splitStringsToInts(ip_addresses);

    char msg[MSG_SIZE] = {};

    if (!ip_address_numbers)
    {
        snprintf(msg, sizeof(msg), "Failed to parse IP addresses");
        printmessage(LOG_ERROR, msg);
        return false;
    }

    IPAddress local_IP(ip_address_numbers[0][0], ip_address_numbers[0][1], ip_address_numbers[0][2], ip_address_numbers[0][3]);
    IPAddress subnet_mask_IP(ip_address_numbers[1][0], ip_address_numbers[1][1], ip_address_numbers[1][2], ip_address_numbers[1][3]);
    IPAddress gateway_IP(ip_address_numbers[2][0], ip_address_numbers[2][1], ip_address_numbers[2][2], ip_address_numbers[2][3]);
    IPAddress primary_dns_IP(ip_address_numbers[3][0], ip_address_numbers[3][1], ip_address_numbers[3][2], ip_address_numbers[3][3]);
    IPAddress secondary_dns_IP(ip_address_numbers[4][0], ip_address_numbers[4][1], ip_address_numbers[4][2], ip_address_numbers[4][3]);

    for (int i = 0; i < 5; i++)
        delete[] ip_address_numbers[i];
    delete[] ip_address_numbers;

    WiFi.mode(WIFI_STA);
    if (!WiFi.config(local_IP, gateway_IP, subnet_mask_IP, primary_dns_IP, secondary_dns_IP))
    {
        snprintf(msg, sizeof(msg), "STA Failed to configure");
        printmessage(LOG_ERROR, msg);
        return false;
    }

    WiFi.begin(ssid, wifi_password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        snprintf(msg, sizeof(msg), "Wifi failed");
        printmessage(LOG_ERROR, msg);
        return false;
    }

    if (ap_active_mutex && xSemaphoreTake(ap_active_mutex, (TickType_t)10))
    {
        ap_active = 0;
        xSemaphoreGive(ap_active_mutex);
    }

    return true;
}

int **splitStringsToInts(const char *input[])
{
    // 5 string to convert in 4 ints each
    int rows = 5;
    int cols = 4;

    // Allocate a 2D integer array dynamically
    int **output = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        output[i] = new int[cols]; // Allocate columns

        // Make a copy of the input string because strtok modifies it
        char temp[32];

        strncpy(temp, input[i], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';

        char *token = strtok(temp, ".");
        int count = 0;

        while (token != nullptr && count < cols)
        {
            output[i][count++] = atoi(token);
            token = strtok(nullptr, ".");
        }

        // Fill remaining slots with 0 if fewer than 4 tokens
        while (count < cols)
        {
            output[i][count++] = 0;
        }
    }
    return output;
}

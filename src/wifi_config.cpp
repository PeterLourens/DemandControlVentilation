#include "wifi_config.h"

// open config file and check if SSID and password are not empty
// if empty start WIFI as access point
// else connect to access point defined in SSID with either DHCP or fixed

void config_wifi(void)
{
    const char *path = "/json/settings_network.json";
    bool network_config_file_present = 0;

    String network_config_string;
    JsonDocument network_config;

    if (settings_network_mutex != NULL)
    {
        if (xSemaphoreTake(settings_network_mutex, (TickType_t)100) == pdTRUE)
        {
            network_config_file_present = check_file_exists(path);
            if (network_config_file_present == 1)
            {
                File file = LittleFS.open(path, "r");
                while (file.available())
                {
                    network_config_string = file.readString();
                }
                file.close();
                deserializeJson(network_config, network_config_string);
            }
            xSemaphoreGive(settings_network_mutex);
        }
    }

    // Print JsonDocument
    Serial.print("\nNetwork config: ");
    serializeJson(network_config, Serial);

    String enable_dhcp = network_config[String("enable_dhcp")];
    String ssid = network_config[String("ssid")];
    String wifi_password = network_config[String("wifi_password")];

    if (ssid != "" && wifi_password != "")
    {
        if (enable_dhcp == "On")
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, wifi_password);
            if (WiFi.waitForConnectResult() != WL_CONNECTED)
            {
                Serial.println("WiFi Failed!");
                return;
            }
            if (ap_active_mutex != NULL)
            {
                if (xSemaphoreTake(ap_active_mutex, (TickType_t)100) == pdTRUE)
                {
                    ap_active = 0;
                    xSemaphoreGive(ap_active_mutex);
                }
            }
        }
        else if (enable_dhcp == "Off")
        {

            // Configure connection manual
            String ip_address = network_config[String("ip_address")];
            String subnet_mask = network_config[String("subnet_mask")];
            String gateway = network_config[String("gateway")];
            String primary_dns = network_config[String("primary_dns")];
            String secondary_dns = network_config[String("secondary_dns")];

            // Create array of strings so it can be passed to a function to create an array with 5 ip addresses with each 4 ints for each IP address number
            String networksettings[5] = {ip_address, subnet_mask, gateway, primary_dns, secondary_dns};
            int **ip_address_numbers = splitStringsToInts(networksettings);

            // Make IPAddress objects from ip_address_numbers
            IPAddress local_IP(ip_address_numbers[0][0], ip_address_numbers[0][1], ip_address_numbers[0][2], ip_address_numbers[0][3]);
            IPAddress subnet_mask_IP(ip_address_numbers[1][0], ip_address_numbers[1][1], ip_address_numbers[1][2], ip_address_numbers[1][3]);
            IPAddress gateway_IP(ip_address_numbers[2][0], ip_address_numbers[2][1], ip_address_numbers[2][2], ip_address_numbers[2][3]);
            IPAddress primary_dns_IP(ip_address_numbers[3][0], ip_address_numbers[3][1], ip_address_numbers[3][2], ip_address_numbers[3][3]);
            IPAddress secondary_dns_IP(ip_address_numbers[4][0], ip_address_numbers[4][1], ip_address_numbers[4][2], ip_address_numbers[4][3]);

            // Free allocated memory
            for (int i = 0; i < 5; i++)
            {
                delete[] ip_address_numbers[i];
            }
            delete[] ip_address_numbers;

            if (ip_address != "" && subnet_mask != "" && gateway != "" && (primary_dns != "" || secondary_dns != ""))
            {
                WiFi.mode(WIFI_STA);
                if (!WiFi.config(local_IP, gateway_IP, subnet_mask_IP, primary_dns_IP, secondary_dns_IP))
                {
                    Serial.print("\nSTA Failed to configure");
                }
                WiFi.begin(ssid, wifi_password);
                if (WiFi.waitForConnectResult() != WL_CONNECTED)
                {
                    Serial.print("\nWiFi Failed!");
                    return;
                }
                if (ap_active_mutex != NULL)
                {
                    if (xSemaphoreTake(ap_active_mutex, (TickType_t)10) == pdTRUE)
                    {
                        ap_active = 0;
                        xSemaphoreGive(ap_active_mutex);
                    }
                }
            }
            else
            {
                Serial.print("\nConfiguration incomplete. Wifi cannot be configured");
            }
        }
        else
        {
            Serial.print("\nWifi configuration failed");
        }
    }
    else
    {
        // Configure here the ESP32 as accesspoint with default settings
        WiFi.mode(WIFI_AP);
        WiFi.softAP("OSVENTILATION-WIFI", NULL);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("\nAP IP address: ");
        Serial.print(IP);

        if (ap_active_mutex != NULL)
        {
            if (xSemaphoreTake(ap_active_mutex, (TickType_t)10) == pdTRUE)
            {
                ap_active = 1;
                xSemaphoreGive(ap_active_mutex);
            }
        }
    }

    // Sync RTC with NTP server
    if (WiFi.waitForConnectResult() == WL_CONNECTED && ap_active == 0)
    {
        sync_rtc_ntp();
    }
}

int **splitStringsToInts(String input[])
{
    // 5 string to convert in 4 ints each
    int rows = 5;
    int cols = 4;

    // Allocate a 2D integer array dynamically
    int **output = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        output[i] = new int[cols]; // Allocate columns
        int start = 0, end = input[i].indexOf('.');
        int count = 0;

        while (end != -1 && count < cols)
        {
            output[i][count++] = input[i].substring(start, end).toInt(); // Convert substring to int
            start = end + 1;
            end = input[i].indexOf('.', start);
        }

        // Capture the last token
        if (count < cols)
        {
            output[i][count++] = input[i].substring(start).toInt();
        }
    }

    return output; // Return the dynamically allocated array
}

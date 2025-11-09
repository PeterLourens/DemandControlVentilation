#include "task_wifi.h"

void start_task_wifi(void)
{

    xTaskCreate(task_wifi_code, "taskWifi", 10000, NULL, 1, &task_wifi);
}

void task_wifi_code(void *pvParameters)
{
    char msg[MSG_SIZE] = {};
    uint8_t baseMac[6];

    for (;;)
    {
        if (WiFi.status() != WL_CONNECTED && ap_active == 0)
        {
            snprintf(msg, sizeof(msg), "No Wifi connection. Trying to connect to Wifi.");
            printmessage(LOG_INFO, msg);
            esp_wifi_stop();
            vTaskDelay(1000);
            esp_wifi_deinit();
            vTaskDelay(1000);
            config_wifi();
        }
        else if (ap_active == 1)
        {
            snprintf(msg, sizeof(msg), "Wifi Access Point is active. Configure wifi on http://192.168.4.1");
            printmessage(LOG_INFO, msg);
        }
        else
        {
            snprintf(msg, sizeof(msg), "Wifi connection ok. Nothing to do.");
            printmessage(LOG_INFO, msg);
        }

        // message = "Wifi status: " + String(WiFi.status()) + ", Wifi SSID: " + WiFi.SSID();
        snprintf(msg, sizeof(msg), "Wifi status: %d", WiFi.status());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Wifi SSID: %s", WiFi.SSID().c_str());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Wifi BSSID: %s", WiFi.BSSIDstr().c_str());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Wifi RSSI: %d dBm", WiFi.RSSI());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "IP Address: %s", WiFi.localIP().toString().c_str());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Subnetmask: %s", WiFi.subnetMask().toString().c_str());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Gateway IP: %s", WiFi.gatewayIP().toString().c_str());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Primary DNS: %s", WiFi.dnsIP(0).toString().c_str());
        printmessage(LOG_INFO, msg);

        snprintf(msg, sizeof(msg), "Secondary DNS: %s", WiFi.dnsIP(1).toString().c_str());
        printmessage(LOG_INFO, msg);

        esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
        if (ret == ESP_OK)
        {
            snprintf(msg, sizeof(msg), "MAC: %02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
            printmessage(LOG_INFO, msg);
        }
        else
        {
            snprintf(msg, sizeof(msg), "Failed to read MAC address");
            printmessage(LOG_ERROR, msg);
        }
        vTaskDelay(120000);
    }
}

char* create_webserial_url(void)
{
    char msg[MSG_SIZE] = {};
    static char webserial_url[MSG_SIZE] = {};

    //webserial_url = "http://" + String(WiFi.localIP().toString()) + ":8080/webserial";
    snprintf(webserial_url, sizeof(webserial_url), "http://%s:8080/webserial", WiFi.localIP().toString().c_str());
    snprintf(msg, sizeof(msg), "Webserial URL: %s", webserial_url);
    printmessage(LOG_INFO, msg);

    return webserial_url;
}

/*
WL_IDLE_STATUS (0): WiFi is in the process of initializing.
WL_NO_SSID_AVAIL (1): No SSID (network name) is available.
WL_SCAN_COMPLETED (2): The network scan is complete.
WL_CONNECTED (3): Successfully connected to a WiFi network.
WL_CONNECT_FAILED (4): Connection attempt failed (e.g., wrong password).
WL_CONNECTION_LOST (5): Connection was lost after being established.
WL_DISCONNECTED (6): Disconnected from the network.
*/

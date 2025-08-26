#include "task_wifi.h"

void start_task_wifi(void)
{

    xTaskCreate(task_wifi_code, "taskWifi", 10000, NULL, 1, &task_wifi);
}

void task_wifi_code(void *pvParameters)
{
    String message = "";
    uint8_t baseMac[6];

    for (;;)
    {
        if (WiFi.status() != WL_CONNECTED && ap_active == 0)
        {
            message = "No Wifi connection. Trying to connect to Wifi.";
            print_message(message);
            config_wifi();
        }
        else if (ap_active == 1)
        {
            message = "Wifi Access Point is active. Configure wifi on http://192.168.4.1";
            print_message(message);
        }
        else
        {
            message = "Wifi connection ok. Nothing to do.";
            print_message(message);
        }

        message = "Wifi status: " + String(WiFi.status()) + ", Wifi SSID: " + WiFi.SSID();
        print_message(message);

        message = "Wifi BSSID: " + WiFi.BSSIDstr() + ", Wifi RSSI: " + String(WiFi.RSSI());
        print_message(message);

        message = "IP Address: " + String(WiFi.localIP().toString()) + ", Subnetmask: " + String(WiFi.subnetMask().toString());
        print_message(message);

        message = "Gateway IP: " + String(WiFi.gatewayIP().toString());
        print_message(message);

        message = "Primary DNS: " + String(WiFi.dnsIP(0).toString()) + ", Secondary DNS: " + String(WiFi.dnsIP(1).toString());
        print_message(message);

        esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
        if (ret == ESP_OK)
        {
            message = "MAC: " + String(baseMac[0], HEX) + ":" + String(baseMac[1], HEX) + ":" + String(baseMac[2], HEX) + ":" + String(baseMac[3], HEX) + ":" + String(baseMac[4], HEX) + ":" + String(baseMac[5], HEX);
            print_message(message);
        }
        else
        {
            message = "Failed to read MAC address";
            print_message(message);
        }
        vTaskDelay(30000);
    }
}

String create_webserial_url(void)
{
    String webserial_url;
    String message;

    webserial_url = "http://" + String(WiFi.localIP().toString()) + ":8080/webserial";
    message = "Webserial URL: " + webserial_url;
    print_message(message);

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

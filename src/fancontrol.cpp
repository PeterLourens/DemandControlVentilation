#include "fancontrol.h"

void set_fanspeed(String speed)
{

    int httpResponseCode = 0;
    bool settings_file_present = 0;

    char fan_control_url_high_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_medium_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_low_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_mode[LARGE_CONFIG_ITEM];
    char fan_control_mqtt_topic[XLARGE_CONFIG_ITEM];
    char msg[MSG_SIZE] = {};

    String fanspeed_temp = "";
    String http_payload = "";

    HTTPClient http;
    JsonDocument doc;

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)100) == pdTRUE)
    {
        fanspeed_temp = fanspeed;

        strncpy(fan_control_mode, fansettings.fan_control_mode, sizeof(fan_control_mode) - 1);
        fan_control_mode[sizeof(fan_control_mode) - 1] = '\0';

        strncpy(fan_control_mqtt_topic, fansettings.fan_control_mqtt_topic, sizeof(fan_control_mqtt_topic) - 1);
        fan_control_mqtt_topic[sizeof(fan_control_mqtt_topic) - 1] = '\0';

        strncpy(fan_control_url_high_speed, fansettings.fan_control_url_high_speed, sizeof(fan_control_url_high_speed) - 1);
        fan_control_url_high_speed[sizeof(fan_control_url_high_speed) - 1] = '\0';

        strncpy(fan_control_url_medium_speed, fansettings.fan_control_url_medium_speed, sizeof(fan_control_url_medium_speed) - 1);
        fan_control_url_medium_speed[sizeof(fan_control_url_medium_speed) - 1] = '\0';

        strncpy(fan_control_url_low_speed, fansettings.fan_control_url_low_speed, sizeof(fan_control_url_low_speed) - 1);
        fan_control_url_low_speed[sizeof(fan_control_url_low_speed) - 1] = '\0';

        xSemaphoreGive(fanspeed_mutex);
    }

    snprintf(msg, sizeof(msg), "Fanspeed %s", fanspeed_temp);
    printmessage(msg);

    if (strcmp(fan_control_mode, "MQTT publish") == 0)
    {
        snprintf(msg, sizeof(msg), "Using MQTT to set fan speed");
        printmessage(msg);
        // String fan_control_mqtt_topic = doc[String("fan_control_mqtt_topic")];
        //  Not yet implmented, subscribe to MQTT topic, create callback function
    }
    else if (strcmp(fan_control_mode, "HTTP API") == 0)
    {
        snprintf(msg, sizeof(msg), "Using HTTP API to set fan speed");
        printmessage(msg);

        if (fanspeed_temp == "Low")
        {
            snprintf(msg, sizeof(msg), "Low speed selected");
            printmessage(msg);
            http.begin(String(fan_control_url_low_speed));
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                http_payload = http.getString();
                snprintf(msg, sizeof(msg), "HTTP Response code: %s, HTTP payload: %s", httpResponseCode, http_payload);
                printmessage(msg);
            }
        }
        else if (fanspeed_temp == "Medium")
        {
            snprintf(msg, sizeof(msg), "Medium speed selected");
            printmessage(msg);
            http.begin(String(fan_control_url_medium_speed));
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                http_payload = http.getString();
                snprintf(msg, sizeof(msg), "HTTP Response code: %s, HTTP payload: %s", httpResponseCode, http_payload);
                printmessage(msg);
            }
        }
        else if (fanspeed_temp == "High")
        {
            snprintf(msg, sizeof(msg), "High speed selected");
            printmessage(msg);
            http.begin(String(fan_control_url_high_speed));
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                http_payload = http.getString();
                snprintf(msg, sizeof(msg), "HTTP Response code: %s, HTTP payload: %s", httpResponseCode, http_payload);
                printmessage(msg);
            }
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "No method to set fanspeed");
        printmessage(msg);
    }
}

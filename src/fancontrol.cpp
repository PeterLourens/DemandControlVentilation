#include "fancontrol.h"

void set_fanspeed(const char *speed)
{
    int httpResponseCode = 0;
    bool settings_file_present = 0;

    char fan_control_url_high_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_medium_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_low_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_mode[LARGE_CONFIG_ITEM];
    char fan_control_mqtt_topic[XLARGE_CONFIG_ITEM];
    char msg[MSG_SIZE] = {};
    char fanspeed_temp[MEDIUM_CONFIG_ITEM] = "";
    char http_payload[XLARGE_CONFIG_ITEM] = "";

    HTTPClient http;
    JsonDocument doc;

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)100) == pdTRUE)
    {
        // fanspeed_temp = fanspeed;
        strncpy(fanspeed_temp, fanspeed, sizeof(fanspeed_temp) - 1);
        fanspeed_temp[sizeof(fanspeed_temp) - 1] = '\0';

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
    printmessage(LOG_INFO, msg);

    if (strcmp(fan_control_mode, "MQTT publish") == 0)
    {
        snprintf(msg, sizeof(msg), "Using MQTT to set fan speed");
        printmessage(LOG_INFO, msg);
        // String fan_control_mqtt_topic = doc[String("fan_control_mqtt_topic")];
        //  Not yet implmented, subscribe to MQTT topic, create callback function
    }
    else if (strcmp(fan_control_mode, "HTTP API") == 0)
    {
        snprintf(msg, sizeof(msg), "Using HTTP API to set fan speed");
        printmessage(LOG_INFO, msg);

        if (strcmp(fanspeed_temp, "Low") == 0)
        {
            snprintf(msg, sizeof(msg), "Low speed selected");
            printmessage(LOG_INFO, msg);
            http.begin(fan_control_url_low_speed);
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                String payloadStr = http.getString();
                strncpy(http_payload, payloadStr.c_str(), sizeof(http_payload) - 1);
                http_payload[sizeof(http_payload) - 1] = '\0';
                snprintf(msg, sizeof(msg), "HTTP Response code: %d, HTTP payload: %s", httpResponseCode, http_payload);
                printmessage(LOG_INFO, msg);
            }
        }
        else if (strcmp(fanspeed_temp, "Medium") == 0)
        {
            snprintf(msg, sizeof(msg), "Medium speed selected");
            printmessage(LOG_INFO, msg);
            http.begin(fan_control_url_medium_speed);
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                String payloadStr = http.getString();
                strncpy(http_payload, payloadStr.c_str(), sizeof(http_payload) - 1);
                http_payload[sizeof(http_payload) - 1] = '\0';
                snprintf(msg, sizeof(msg), "HTTP Response code: %d, HTTP payload: %s", httpResponseCode, http_payload);
                printmessage(LOG_INFO, msg);
            }
        }
        else if (strcmp(fanspeed_temp, "High") == 0)
        {
            snprintf(msg, sizeof(msg), "High speed selected");
            printmessage(LOG_INFO, msg);
            http.begin(fan_control_url_high_speed);
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                String payloadStr = http.getString();
                strncpy(http_payload, payloadStr.c_str(), sizeof(http_payload) - 1);
                http_payload[sizeof(http_payload) - 1] = '\0';
                snprintf(msg, sizeof(msg), "HTTP Response code: %d, HTTP payload: %s", httpResponseCode, http_payload);
                printmessage(LOG_INFO, msg);
            }
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "No method to set fanspeed");
        printmessage(LOG_ERROR, msg);
    }
}

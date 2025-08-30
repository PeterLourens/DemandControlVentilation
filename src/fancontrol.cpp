#include "fancontrol.h"

void set_fanspeed(String speed)
{

    int httpResponseCode = 0;
    bool settings_file_present = 0;

    String fanspeed_temp = "";
    String message = "";
    String http_payload = "";

    char fan_control_url_high_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_medium_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_low_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_mode[LARGE_CONFIG_ITEM];
    char fan_control_mqtt_topic[XLARGE_CONFIG_ITEM];

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

    // String fan_control_mode = doc[String("fan_control_mode")];
    message = "Fanspeed: " + String(fanspeed_temp);
    print_message(message);

    if (strcmp(fan_control_mode, "MQTT publish") == 0)
    {
        message = "Using MQTT to set fan speed";
        print_message(message);
        // String fan_control_mqtt_topic = doc[String("fan_control_mqtt_topic")];
        //  Not yet implmented, subscribe to MQTT topic, create callback function
    }
    else if (strcmp(fan_control_mode, "HTTP API") == 0)
    {
        message = "Using HTTP API to set fan speed";
        print_message(message);

        if (fanspeed_temp == "Low")
        {
            message = "Low speed selected";
            print_message(message);
            http.begin(String(fan_control_url_low_speed));
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                http_payload = http.getString();
                message = "HTTP Response code: " + String(httpResponseCode) + ", HTTP payload: " + String(http_payload);
                print_message(message);
            }
        }
        else if (fanspeed_temp == "Medium")
        {
            message = "Medium speed selected";
            print_message(message);
            http.begin(String(fan_control_url_medium_speed));
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                http_payload = http.getString();
                message = "HTTP Response code: " + String(httpResponseCode) + ", HTTP payload: " + String(http_payload);
                print_message(message);
            }
        }
        else if (fanspeed_temp == "High")
        {
            message = "High speed selected";
            print_message(message);
            http.begin(String(fan_control_url_high_speed));
            httpResponseCode = http.GET();
            if (httpResponseCode > 0)
            {
                http_payload = http.getString();
                message = "HTTP Response code: " + String(httpResponseCode) + ", HTTP payload: " + String(http_payload);
                print_message(message);
            }
        }
    }
    else
    {
        message = "No method to set fan speed.";
        print_message(message);
    }
}

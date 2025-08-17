#include "fancontrol.h"

void set_fanspeed(String speed)
{

    int httpResponseCode = 0;
    bool settings_file_present = 0;

    String fanspeed_temp = "";
    String message = "";
    String http_payload = "";

    String fan_control_url_high_speed_temp = "";
    String fan_control_url_medium_speed_temp = "";
    String fan_control_url_low_speed_temp = "";
    String fan_control_mode_temp = "";
    String fan_control_mqtt_topic_temp = "";

    HTTPClient http;
    JsonDocument doc;

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)100) == pdTRUE)
        {
            fanspeed_temp = fanspeed;
            fan_control_mode_temp = fan_control_mode;
            fan_control_mqtt_topic_temp = fan_control_mqtt_topic;
            fan_control_url_high_speed_temp = fan_control_url_high_speed;
            fan_control_url_medium_speed_temp = fan_control_url_high_speed;
            fan_control_url_low_speed_temp = fan_control_url_high_speed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    // String fan_control_mode = doc[String("fan_control_mode")];
    message = "Fanspeed: " + String(fanspeed_temp);
    print_message(message);

    if (fan_control_mode_temp == "MQTT publish")
    {
        message = "Using MQTT to set fan speed";
        print_message(message);
        // String fan_control_mqtt_topic = doc[String("fan_control_mqtt_topic")];
        //  Not yet implmented, subscribe to MQTT topic, create callback function
    }
    else if (fan_control_mode_temp == "HTTP API")
    {
        message = "Using HTTP API to set fan speed";
        print_message(message);

        if (fanspeed_temp == "Low")
        {
            message = "Low speed selected";
            print_message(message);
            http.begin(fan_control_url_low_speed_temp.c_str());
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
            http.begin(fan_control_url_medium_speed_temp.c_str());
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
            http.begin(fan_control_url_high_speed_temp.c_str());
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

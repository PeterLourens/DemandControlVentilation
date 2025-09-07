#include "task_mqtt.h"

void start_task_mqtt(void)
{

    xTaskCreate(task_mqtt_code, "taskMQTT", 10000, NULL, 2, &task_mqtt);
}

void task_mqtt_code(void *pvParameters)
{
    bool ap_active_temp = 0;

    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_server[XLARGE_CONFIG_ITEM] = {};
    int mqtt_port = 0;
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};

    char msg[MSG_SIZE] = {};

    for (;;)
    {
        // Read basic connection settings for MQTT to check if MQTT connection can be made
        if (settings_mqtt_mutex != NULL)
        {
            if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
            {
                strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
                enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

                strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
                mqtt_server[sizeof(enable_mqtt) - 1] = '\0';

                strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
                mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

                mqtt_port = mqttsettings.mqtt_port;

                xSemaphoreGive(settings_mqtt_mutex);
            }
        }

        if (ap_active_mutex != NULL)
        {
            if (xSemaphoreTake(ap_active_mutex, (TickType_t)10) == pdTRUE)
            {
                ap_active_temp = ap_active;
                xSemaphoreGive(ap_active_mutex);
            }
        }

        // Check if MQTT functions can run
        if (WiFi.waitForConnectResult() == WL_CONNECTED && ap_active_temp == 0 && strcmp(enable_mqtt, "On") == 0 && strcmp(mqtt_server, "") != 0 && mqtt_port != 0)
        {
            snprintf(msg, sizeof(msg), "Update MQTT");
            printmessage(msg);

            // read_mqtt_config();
            publish_sensor_data();
            publish_avg_sensor_data();
            publish_valve_positions();
            publish_uptime();
            publish_state();
            publish_fanspeed();
        }
        else
        {
            snprintf(msg, sizeof(msg), "No WIFI connection, MQTT disabled or MQTT settings incomplete");
            printmessage(msg);
        }
        vTaskDelay(10000);
    }
}

// strcmp(mqtt_base_topic, "") == 0
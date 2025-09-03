#include "mqtt.h"

WiFiClient OSventilation;
PubSubClient client(OSventilation);

void publish_sensor_data(void)
{
    float queue_sensor_data[2][8][3];
    int mqtt_port = 0;

    char topic[200];
    char sensor_value[8];
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    const char *mqtt_server = {};

    String measurement;

    if (settings_mqtt_mutex != NULL)
    {
        if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
        {
            strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
            enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

            mqtt_server = mqttsettings.mqtt_server; // Because defined as pointer

            strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
            mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

            mqtt_port = mqttsettings.mqtt_port;

            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    // Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if (strcmp(mqtt_base_topic, "") == 0)
    {
        strncpy(mqtt_base_topic, "Change_topic_in_settings", sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
    }

    // Serial.print("\nPublish sensor data.");
    if (xQueuePeek(sensor_queue, &queue_sensor_data, 0) == pdTRUE)
    {
        client.setServer(mqtt_server, mqtt_port);

        if (client.connect("OSventilation"))
        {
            for (int bus = 0; bus < 2; bus++)
            {
                for (int slot = 0; slot < 8; slot++)
                {

                    if (queue_sensor_data[bus][slot][0] > 2)
                    {
                        measurement = "/temperature";
                        (String(mqtt_base_topic) + "/bus/" + String(bus) + "/sensor" + String(slot) + measurement).toCharArray(topic, 200);
                        String(queue_sensor_data[bus][slot][0]).toCharArray(sensor_value, 8);
                        client.publish(topic, sensor_value);
                    }

                    if (queue_sensor_data[bus][slot][1] > 2)
                    {
                        measurement = "/humidity";
                        (String(mqtt_base_topic) + "/bus/" + String(bus) + "/sensor" + String(slot) + measurement).toCharArray(topic, 200);
                        String(queue_sensor_data[bus][slot][1]).toCharArray(sensor_value, 8);
                        client.publish(topic, sensor_value);
                    }

                    if (queue_sensor_data[bus][slot][2] > 2)
                    {
                        measurement = "/CO2";
                        (String(mqtt_base_topic) + "/bus/" + String(bus) + "/sensor" + String(slot) + measurement).toCharArray(topic, 200);
                        String(queue_sensor_data[bus][slot][2]).toCharArray(sensor_value, 8);
                        client.publish(topic, sensor_value);
                    }
                }
            }
        }
        else
        {
            Serial.print("Could not connect to MQTT server");
        }
    }
}

void publish_avg_sensor_data(void)
{
    float queue_sensor_avg_data[2][8][3];
    int mqtt_port = 0;

    char topic[200];
    char sensor_avg_value[8];
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    const char *mqtt_server;

    String measurement;

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        mqtt_server = mqttsettings.mqtt_server;

        strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

        mqtt_port = mqttsettings.mqtt_port;

        xSemaphoreGive(settings_mqtt_mutex);
    }

    // Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if (strcmp(mqtt_base_topic, "") == 0)
    {
        strncpy(mqtt_base_topic, "Change_topic_in_settings", sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
    }

    if (xQueuePeek(sensor_avg_queue, &queue_sensor_avg_data, 0) == pdTRUE)
    {
        client.setServer(mqtt_server, mqtt_port);

        if (client.connect("OSventilation"))
        {
            for (int bus = 0; bus < 2; bus++)
            {
                for (int slot = 0; slot < 8; slot++)
                {

                    if (queue_sensor_avg_data[bus][slot][0] > 2)
                    {
                        measurement = "/temperature";
                        (String(mqtt_base_topic) + "/bus/" + String(bus) + "/sensor" + String(slot) + "_avg" + measurement).toCharArray(topic, 200);
                        String(queue_sensor_avg_data[bus][slot][0]).toCharArray(sensor_avg_value, 8);
                        client.publish(topic, sensor_avg_value);
                    }

                    if (queue_sensor_avg_data[bus][slot][1] > 2)
                    {
                        measurement = "/humidity";
                        (String(mqtt_base_topic) + "/bus/" + String(bus) + "/sensor" + String(slot) + "_avg" + measurement).toCharArray(topic, 200);
                        String(queue_sensor_avg_data[bus][slot][1]).toCharArray(sensor_avg_value, 8);
                        client.publish(topic, sensor_avg_value);
                    }

                    if (queue_sensor_avg_data[bus][slot][2] > 2)
                    {
                        measurement = "/CO2";
                        (String(mqtt_base_topic) + "/bus/" + String(bus) + "/sensor" + String(slot) + "_avg" + measurement).toCharArray(topic, 200);
                        String(queue_sensor_avg_data[bus][slot][2]).toCharArray(sensor_avg_value, 8);
                        client.publish(topic, sensor_avg_value);
                    }
                }
            }
        }
        else
        {
            Serial.print("Could not connect to MQTT server");
        }
    }
}

void publish_valve_positions(void)
{
    int mqtt_port = 0;
    bool status_file_present;

    char valve_pos[4];
    char valve_nr[10];
    char topic[100];
    char buffer[512];
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    const char *mqtt_server;

    String json = "";
    JsonDocument doc;
    String message = "";

    if (settings_mqtt_mutex != NULL)
    {
        if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
        {
            strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
            enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

            mqtt_server = mqttsettings.mqtt_server;

            strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
            mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

            mqtt_port = mqttsettings.mqtt_port;

            xSemaphoreGive(settings_mqtt_mutex);
        }
    }

    // Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if (strcmp(mqtt_base_topic, "") == 0)
    {
        strncpy(mqtt_base_topic, "Change_topic_in_settings", sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
    }

    client.setServer(mqtt_server, mqtt_port);

    if (read_settings(VALVE_POSITIONS_PATH, buffer, sizeof(buffer), valve_position_file_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            message = "[ERROR] Failed to parse: " + String(VALVE_POSITIONS_PATH) + " with error: " + String(error.c_str());
            print_message(message);
        }
    }

    if (client.connect("OSventilation"))
    {
        for (int i = 0; i < 12; i++)
        {
            String valve_nr_str = "valve" + String(i);
            String valve_pos_str = doc[String(valve_nr_str)];
            valve_nr_str.toCharArray(valve_nr, 10);
            valve_pos_str.toCharArray(valve_pos, 4);
            (String(mqtt_base_topic) + "/position/" + valve_nr_str).toCharArray(topic, 100);
            client.publish(topic, valve_pos);
        }
    }
    else
    {
        Serial.println("Could not connect to MQTT server");
    }
}

void publish_uptime(void)
{
    int mqtt_port = 0;

    char topic[100];
    char uptime_str[64];
    //char uptime[300];
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    const char *mqtt_server;
    
    String json;
    //String uptime_str;

    formatted_uptime(uptime_str, sizeof(uptime_str));

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        mqtt_server = mqttsettings.mqtt_server;
        // strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        // mqtt_server[sizeof(enable_mqtt) - 1] = '\0';

        strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

        mqtt_port = mqttsettings.mqtt_port;

        xSemaphoreGive(settings_mqtt_mutex);
    }

    // Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if (strcmp(mqtt_base_topic, "") == 0)
    {
        strncpy(mqtt_base_topic, "Change_topic_in_settings", sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
    }

    client.setServer(mqtt_server, mqtt_port);

    if (client.connect("OSventilation"))
    {
        (String(mqtt_base_topic) + "/system/uptime").toCharArray(topic, 100);
        //uptime_str = formatted_uptime();
        //uptime_str.toCharArray(uptime, 300);
        client.publish(topic, uptime_str);
    }
    else
    {
        Serial.print("\nCould not connect to MQTT server");
    }
}

void publish_fanspeed(void)
{
    int mqtt_port = 0;

    char topic[100];
    char fan[20];
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    const char *mqtt_server;

    String json;
    String temp_fanspeed;

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        mqtt_server = mqttsettings.mqtt_server;

        strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

        mqtt_port = mqttsettings.mqtt_port;

        xSemaphoreGive(settings_mqtt_mutex);
    }

    // Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if (strcmp(mqtt_base_topic, "") == 0)
    {
        strncpy(mqtt_base_topic, "Change_topic_in_settings", sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
    }

    client.setServer(mqtt_server, mqtt_port);

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_fanspeed = fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (client.connect("OSventilation"))
    {
        temp_fanspeed.toCharArray(fan, 20);
        (String(mqtt_base_topic) + "/status/fanspeed").toCharArray(topic, 100);
        client.publish(topic, fan);
    }
    else
    {
        Serial.println("Could not connect to MQTT server");
    }
}

void publish_state(void)
{
    int mqtt_port = 0;

    char topic[100];
    char temp_state[20];
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    const char *mqtt_server;

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        mqtt_server = mqttsettings.mqtt_server;

        strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';

        mqtt_port = mqttsettings.mqtt_port;

        xSemaphoreGive(settings_mqtt_mutex);
    }

    // Assign default MQTT base topic when setting is empty to prevent writing to MQTT root
    if (strcmp(mqtt_base_topic, "") == 0)
    {
        strncpy(mqtt_base_topic, "Change_topic_in_settings", sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state.toCharArray(temp_state, 20);
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    client.setServer(mqtt_server, mqtt_port);

    if (client.connect("OSventilation"))
    {
        (String(mqtt_base_topic) + "/status/state").toCharArray(topic, 100);
        client.publish(topic, temp_state);
    }
    else
    {
        Serial.println("Could not connect to MQTT server");
    }
}

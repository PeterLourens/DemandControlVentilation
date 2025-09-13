#include "mqtt.h"

/*void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
    // Changes the output state according to the message
    // if (String(topic) == "zigbee2mqtt/fan_button1/action")
    /*{
        Serial.print("Fan_button1 output: ");
        if (messageTemp == "on")
        {
            Serial.println("on");

        }
        else if (messageTemp == "off")
        {
            Serial.println("off");

        }
    }
}*/

WiFiClient OSventilation;
PubSubClient client(OSventilation);

void publish_sensor_data(void)
{
    int mqtt_port = 0;
    float queue_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

    char topic[XXLARGE_CONFIG_ITEM] = {};
    char sensor_value[SMALL_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char measurement[MEDIUM_CONFIG_ITEM] = {};
    char mqtt_server[LARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        mqtt_server[sizeof(mqtt_server) - 1] = '\0';

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
                        strncpy(measurement, "/temperature", sizeof(measurement));
                        measurement[sizeof(measurement) - 1] = '\0';

                        snprintf(topic, sizeof(topic), "%s/bus/%d/sensor%d%s", mqtt_base_topic, bus, slot, measurement);
                        snprintf(sensor_value, sizeof(sensor_value), "%.2f", queue_sensor_data[bus][slot][0]);
                        client.publish(topic, sensor_value);
                    }

                    if (queue_sensor_data[bus][slot][1] > 2)
                    {
                        strncpy(measurement, "/humidity", sizeof(measurement));
                        measurement[sizeof(measurement) - 1] = '\0';

                        snprintf(topic, sizeof(topic), "%s/bus/%d/sensor%d%s", mqtt_base_topic, bus, slot, measurement);
                        snprintf(sensor_value, sizeof(sensor_value), "%f", queue_sensor_data[bus][slot][1]);
                        client.publish(topic, sensor_value);
                    }

                    if (queue_sensor_data[bus][slot][2] > 2)
                    {
                        strncpy(measurement, "/CO2", sizeof(measurement));
                        measurement[sizeof(measurement) - 1] = '\0';

                        snprintf(topic, sizeof(topic), "%s/bus/%d/sensor%d%s", mqtt_base_topic, bus, slot, measurement);
                        snprintf(sensor_value, sizeof(sensor_value), "%.2f", queue_sensor_data[bus][slot][2]);
                        client.publish(topic, sensor_value);
                    }
                }
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "Could not connect to MQTT server.");
            printmessage(LOG_ERROR, msg);
        }
    }
}

void publish_avg_sensor_data(void)
{
    int mqtt_port = 0;
    float queue_sensor_avg_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

    char topic[XXLARGE_CONFIG_ITEM] = {};
    char sensor_avg_value[SMALL_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char measurement[MEDIUM_CONFIG_ITEM] = {};
    char mqtt_server[LARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        mqtt_server[sizeof(mqtt_server) - 1] = '\0';

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
                        strncpy(measurement, "/temperature", sizeof(measurement));
                        measurement[sizeof(measurement) - 1] = '\0';

                        snprintf(topic, sizeof(topic), "%s/bus/%d/sensor_avg%d%s", mqtt_base_topic, bus, slot, measurement);
                        snprintf(sensor_avg_value, sizeof(sensor_avg_value), "%.2f", queue_sensor_avg_data[bus][slot][0]);
                        client.publish(topic, sensor_avg_value);
                    }

                    if (queue_sensor_avg_data[bus][slot][1] > 2)
                    {
                        strncpy(measurement, "/humidity", sizeof(measurement));
                        measurement[sizeof(measurement) - 1] = '\0';

                        snprintf(topic, sizeof(topic), "%s/bus/%d/sensor_avg%d%s", mqtt_base_topic, bus, slot, measurement);
                        snprintf(sensor_avg_value, sizeof(sensor_avg_value), "%f", queue_sensor_avg_data[bus][slot][1]);
                        client.publish(topic, sensor_avg_value);
                    }

                    if (queue_sensor_avg_data[bus][slot][2] > 2)
                    {
                        strncpy(measurement, "/CO2", sizeof(measurement));
                        measurement[sizeof(measurement) - 1] = '\0';

                        snprintf(topic, sizeof(topic), "%s/bus/%d/sensor_avg%d%s", mqtt_base_topic, bus, slot, measurement);
                        snprintf(sensor_avg_value, sizeof(sensor_avg_value), "%.2f", queue_sensor_avg_data[bus][slot][2]);
                        client.publish(topic, sensor_avg_value);
                    }
                }
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "Could not connect to MQTT server.");
            printmessage(LOG_ERROR, msg);
        }
    }
}

void publish_valve_positions(void)
{
    int mqtt_port = 0;

    bool status_file_present = false;
    char valve_pos[SMALL_CONFIG_ITEM] = {};
    char valve_nr[SMALL_CONFIG_ITEM] = {};
    char topic[XLARGE_CONFIG_ITEM] = {};
    char buffer[512] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char mqtt_server[LARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    JsonDocument doc;

    if (settings_mqtt_mutex != NULL)
    {
        if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
        {
            strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
            enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

            strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
            mqtt_server[sizeof(mqtt_server) - 1] = '\0';

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

    if (client.connect("OSventilation"))
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
            {
                snprintf(valve_pos, sizeof(valve_pos), "%d", valvecontroldata.actual_valve_position[i]);
                xSemaphoreGive(valve_control_data_mutex);
            }
            snprintf(topic, sizeof(topic), "%s/position/valve%d", mqtt_base_topic, i);
            client.publish(topic, valve_pos);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "Could not connect to MQTT server.");
        printmessage(LOG_ERROR, msg);
    }
}

void publish_uptime(void)
{
    int mqtt_port = 0;

    char topic[100] = {};
    char uptime_str[64] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_server[LARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    formatted_uptime(uptime_str, sizeof(uptime_str));

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        mqtt_server[sizeof(mqtt_server) - 1] = '\0';

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
        snprintf(topic, sizeof(topic), "%s/system/uptime", mqtt_base_topic);
        client.publish(topic, uptime_str);
    }
    else
    {
        snprintf(msg, sizeof(msg), "Could not connect to MQTT server.");
        printmessage(LOG_ERROR, msg);
    }
}

void publish_fanspeed(void)
{
    int mqtt_port = 0;

    char topic[XLARGE_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char mqtt_server[LARGE_CONFIG_ITEM] = {};
    char temp_fanspeed[MEDIUM_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';

        strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        mqtt_server[sizeof(mqtt_server) - 1] = '\0';

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

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        // temp_fanspeed = fanspeed;
        strncpy(temp_fanspeed, fanspeed, sizeof(temp_fanspeed) - 1);
        temp_fanspeed[sizeof(temp_fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    if (client.connect("OSventilation"))
    {
        snprintf(topic, sizeof(topic), "%s/status/fanspeed", mqtt_base_topic);
        client.publish(topic, temp_fanspeed);
    }
    else
    {
        snprintf(msg, sizeof(msg), "Could not connect to MQTT server.");
        printmessage(LOG_ERROR, msg);
    }
}

void publish_state(void)
{
    int mqtt_port = 0;

    char topic[XLARGE_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char mqtt_server[MEDIUM_CONFIG_ITEM] = {};
    char temp_state[MEDIUM_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';
        strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        mqtt_server[sizeof(mqtt_server) - 1] = '\0';
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

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        // temp_state = state;
        strncpy(temp_state, state, sizeof(temp_state) - 1);
        temp_state[sizeof(temp_state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    client.setServer(mqtt_server, mqtt_port);

    if (client.connect("OSventilation"))
    {
        snprintf(topic, sizeof(topic), "%s/status/state", mqtt_base_topic);
        client.publish(topic, temp_state);
    }
    else
    {
        snprintf(msg, sizeof(msg), "Could not connect to MQTT server.");
        printmessage(LOG_ERROR, msg);
    }
}

/*void setup_mqtt_remotes(void) {

    int mqtt_port = 0;

    char topic[XLARGE_CONFIG_ITEM] = {};
    char enable_mqtt[SMALL_CONFIG_ITEM] = {};
    char mqtt_base_topic[LARGE_CONFIG_ITEM] = {};
    char mqtt_server[MEDIUM_CONFIG_ITEM] = {};
    char temp_state[MEDIUM_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_mqtt, mqttsettings.enable_mqtt, sizeof(enable_mqtt) - 1);
        enable_mqtt[sizeof(enable_mqtt) - 1] = '\0';
        strncpy(mqtt_server, mqttsettings.mqtt_server, sizeof(mqtt_server) - 1);
        mqtt_server[sizeof(mqtt_server) - 1] = '\0';
        strncpy(mqtt_base_topic, mqttsettings.mqtt_base_topic, sizeof(mqtt_base_topic) - 1);
        mqtt_base_topic[sizeof(mqtt_base_topic) - 1] = '\0';
        mqtt_port = mqttsettings.mqtt_port;
        xSemaphoreGive(settings_mqtt_mutex);
    }

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    client.subscribe("zigbee2mqtt/fan_button1/action");
    Serial.print("\nsubscribed to topic");
}*/

/*void loop_client(void) {
    client.loop();
}*/


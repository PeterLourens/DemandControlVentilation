#include "influxdb.h"

void write_sensor_data(void)
{
    float queue_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char tag[MEDIUM_CONFIG_ITEM] = {};
    char bus[SMALL_CONFIG_ITEM] = {};
    char sensor_valve[MEDIUM_CONFIG_ITEM] = {};
    char sensor_location[MEDIUM_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';
        xSemaphoreGive(settings_influxdb_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("Sensors");

    if (xQueuePeek(sensor_queue, &queue_sensor_data, 0) == pdTRUE)
    {
        // Check server connection. Only write data when server is available.
        if (client.validateConnection())
        {
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (queue_sensor_data[i][j][0] > 0)
                    {
                        sensor.clearFields();
                        sensor.clearTags();
                        if (i == 0)
                        {
                            if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10) == pdTRUE)
                            {
                                snprintf(sensor_valve, sizeof(sensor_valve), "%s", sensor1settings[j].wire_sensor_valve);
                                snprintf(sensor_location, sizeof(sensor_location), "%s", sensor1settings[j].wire_sensor_location);
                                xSemaphoreGive(settings_sensor1_mutex);
                            }

                            if (sensor_valve != NULL)
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (sensor_location != NULL)
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        else
                        {
                            if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10) == pdTRUE)
                            {
                                snprintf(sensor_valve, sizeof(sensor_valve), "%s", sensor2settings[j].wire1_sensor_valve);
                                snprintf(sensor_location, sizeof(sensor_location), "%s", sensor2settings[j].wire1_sensor_location);
                                xSemaphoreGive(settings_sensor2_mutex);
                            }
                            if (sensor_valve != NULL)
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (sensor_location != NULL)
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        snprintf(tag, sizeof(tag), "sensor%d", j);
                        snprintf(bus, sizeof(bus), "bus%d", i);
                        sensor.addTag("device", tag);
                        sensor.addTag("bus", bus);

                        if (queue_sensor_data[i][j][0] > 3)
                        {
                            sensor.addField("temperature", queue_sensor_data[i][j][0]);
                        }
                        if (queue_sensor_data[i][j][1] > 5)
                        {
                            sensor.addField("humidity", queue_sensor_data[i][j][1]);
                        }
                        if (queue_sensor_data[i][j][2] > 5)
                        {
                            sensor.addField("CO2", queue_sensor_data[i][j][2]);
                        }

                        client.pointToLineProtocol(sensor);

                        if (!client.writePoint(sensor))
                        {
                            snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
                            printmessage(LOG_ERROR, msg);
                        }
                        vTaskDelay(50);
                    }
                }
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
            printmessage(LOG_ERROR, msg);
        }
    }
}

void write_avg_sensor_data(void)
{
    float queue_avg_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};
    char tag[MEDIUM_CONFIG_ITEM] = {};
    char bus[SMALL_CONFIG_ITEM] = {};
    char sensor_valve[MEDIUM_CONFIG_ITEM] = {};
    char sensor_location[MEDIUM_CONFIG_ITEM] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';
        xSemaphoreGive(settings_influxdb_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("Sensors_avg");

    if (xQueuePeek(sensor_avg_queue, &queue_avg_sensor_data, 0) == pdTRUE)
    {
        if (client.validateConnection())
        {
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (queue_avg_sensor_data[i][j][0] > 0)
                    {
                        sensor.clearFields();
                        sensor.clearTags();
                        if (i == 0)
                        {
                            if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10) == pdTRUE)
                            {
                                snprintf(sensor_valve, sizeof(sensor_valve), "%s", sensor1settings[j].wire_sensor_valve);
                                snprintf(sensor_location, sizeof(sensor_location), "%s", sensor1settings[j].wire_sensor_location);
                                xSemaphoreGive(settings_sensor1_mutex);
                            }
                            if (sensor_valve != NULL)
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (sensor_location != NULL)
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        else
                        {
                            if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10) == pdTRUE)
                            {
                                snprintf(sensor_valve, sizeof(sensor_valve), "%s", sensor2settings[j].wire1_sensor_valve);
                                snprintf(sensor_location, sizeof(sensor_location), "%s", sensor2settings[j].wire1_sensor_location);
                                xSemaphoreGive(settings_sensor2_mutex);
                            }
                            if (sensor_valve != NULL)
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (sensor_location != NULL)
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        snprintf(tag, sizeof(tag), "sensor%d", j);
                        snprintf(bus, sizeof(bus), "bus%d", i);
                        sensor.addTag("device", tag);
                        sensor.addTag("bus", bus);
                        sensor.addField("temperature", queue_avg_sensor_data[i][j][0]);
                        sensor.addField("humidity", queue_avg_sensor_data[i][j][1]);
                        sensor.addField("CO2", queue_avg_sensor_data[i][j][2]);
                        client.pointToLineProtocol(sensor);
                        if (!client.writePoint(sensor))
                        {
                            snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
                            printmessage(LOG_ERROR, msg);
                        }
                        vTaskDelay(50);
                    }
                }
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
            printmessage(LOG_ERROR, msg);
        }
    }
}

void write_valve_position_data(void)
{
    int valve_pos_temp = 0;
    int valve_pos_sum = 0;

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char buffer[512] = {};
    char msg[MSG_SIZE] = {};
    char tag[SMALL_CONFIG_ITEM] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';
        xSemaphoreGive(settings_influxdb_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("Valves");

    if (client.validateConnection())
    {
        for (int i = 0; i < 12; i++)
        {
            if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
            {
                valve_pos_temp = valvecontroldata.actual_valve_position[i];
                xSemaphoreGive(valve_control_data_mutex);
            }
            valve_pos_sum = valve_pos_sum + valve_pos_temp;

            if (valve_pos_sum != 0)
            {
                sensor.clearFields();
                sensor.clearTags();
                snprintf(tag, sizeof(tag), "valve%d", i);
                sensor.addTag("device", tag);
                sensor.addField("position", valve_pos_temp);
                client.pointToLineProtocol(sensor);

                if (!client.writePoint(sensor))
                {
                    snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
                    printmessage(LOG_ERROR, msg);
                }
            }
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
        printmessage(LOG_ERROR, msg);
    }
}

void write_system_uptime(void)
{
    uint64_t uptime;

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';
        xSemaphoreGive(settings_influxdb_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("System");

    uptime = (esp_timer_get_time()) / 1000000; // in sec

    if (client.validateConnection())
    {
        sensor.clearFields();
        sensor.clearTags();
        sensor.addField("uptime", uptime);
        client.pointToLineProtocol(sensor);

        if (!client.writePoint(sensor))
        {
            snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
            printmessage(LOG_ERROR, msg);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
        printmessage(LOG_ERROR, msg);
    }
}

void write_state_info(void)
{
    int temp_state_nr = 0;

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char temp_state[MEDIUM_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';

        xSemaphoreGive(settings_influxdb_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("Status");

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(temp_state, state, sizeof(temp_state) - 1);
        temp_state[sizeof(temp_state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    // Need to translate state to number for easy processing in Grafana
    if (strcmp(temp_state, "init") == 0)
    {
        temp_state_nr = 1;
    }
    else if (strcmp(temp_state, "night") == 0)
    {
        temp_state_nr = 2;
    }
    else if (strcmp(temp_state, "day") == 0)
    {
        temp_state_nr = 3;
    }
    else if (strcmp(temp_state, "highco2night") == 0)
    {
        temp_state_nr = 4;
    }
    else if (strcmp(temp_state, "highco2day") == 0)
    {
        temp_state_nr = 5;
    }
    else if (strcmp(temp_state, "manualhighspeed") == 0)
    {
        temp_state_nr = 6;
    }
    else if (strcmp(temp_state, "cyclingnight") == 0)
    {
        temp_state_nr = 7;
    }
    else if (strcmp(temp_state, "cyclingday") == 0)
    {
        temp_state_nr = 8;
    }
    else if (strcmp(temp_state, "cooking") == 0)
    {
        temp_state_nr = 9;
    }
    else if (strcmp(temp_state, "highrhnight") == 0)
    {
        temp_state_nr = 10;
    }
    else if (strcmp(temp_state, "highrhday") == 0)
    {
        temp_state_nr = 11;
    }
    else if (strcmp(temp_state, "stopped") == 0)
    {
        temp_state_nr = 12;
    }
    else
    {
        temp_state_nr = 13;
    }

    if (client.validateConnection())
    {
        sensor.clearFields();
        sensor.clearTags();
        sensor.addField("state", temp_state_nr);

        if (!client.writePoint(sensor))
        {
            snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
            printmessage(LOG_ERROR, msg);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
        printmessage(LOG_ERROR, msg);
    }
}

void write_fanspeed(void)
{
    int temp_fanspeed_nr = 0;

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char temp_fanspeed[SMALL_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';
        xSemaphoreGive(settings_influxdb_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("Status");

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        // temp_fanspeed = fanspeed;
        strncpy(temp_fanspeed, fanspeed, sizeof(temp_fanspeed) - 1);
        temp_fanspeed[sizeof(temp_fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    // Need to translate fanspeed to number for easy processing in Grafana
    if (strcmp(temp_fanspeed, "Low") == 0)
    {
        temp_fanspeed_nr = 1;
    }
    else if (strcmp(temp_fanspeed, "Medium") == 0)
    {
        temp_fanspeed_nr = 2;
    }
    else if (strcmp(temp_fanspeed, "High") == 0)
    {
        temp_fanspeed_nr = 3;
    }
    else
    {
        temp_fanspeed_nr = 4;
    }

    if (client.validateConnection())
    {
        sensor.clearFields();
        sensor.clearTags();
        sensor.addField("fanspeed", temp_fanspeed_nr);
        if (!client.writePoint(sensor))
        {
            snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
            printmessage(LOG_ERROR, msg);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
        printmessage(LOG_ERROR, msg);
    }
}

void write_heap_info(void)
{
    int free_heap_size = 0;
    int minimum_ever_free_heap_size = 0;
    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};
    char msg[MSG_SIZE] = {};

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
        enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';

        strncpy(influxdb_url, influxdbsettings.influxdb_url, sizeof(influxdb_url) - 1);
        influxdb_url[sizeof(influxdb_url) - 1] = '\0';

        strncpy(influxdb_org, influxdbsettings.influxdb_org, sizeof(influxdb_org) - 1);
        influxdb_org[sizeof(influxdb_org) - 1] = '\0';

        strncpy(influxdb_bucket, influxdbsettings.influxdb_bucket, sizeof(influxdb_bucket) - 1);
        influxdb_bucket[sizeof(influxdb_bucket) - 1] = '\0';

        strncpy(influxdb_token, influxdbsettings.influxdb_token, sizeof(influxdb_token) - 1);
        influxdb_token[sizeof(influxdb_token) - 1] = '\0';
        xSemaphoreGive(settings_influxdb_mutex);
    }

    free_heap_size = get_free_heap_size();
    minimum_ever_free_heap_size = get_min_ever_heap_size();

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("System_stats");

    if (client.validateConnection())
    {
        sensor.clearFields();
        sensor.clearTags();
        sensor.addField("min_free_heap_size_ever", minimum_ever_free_heap_size);
        sensor.addField("free_heap_size", free_heap_size);

        if (!client.writePoint(sensor))
        {
            snprintf(msg, sizeof(msg), "InfluxDB write failed with %s.", client.getLastErrorMessage());
            printmessage(LOG_ERROR, msg);
        }
    }
    else
    {
        snprintf(msg, sizeof(msg), "InfluxDB connection failed with %s.", client.getLastErrorMessage());
        printmessage(LOG_ERROR, msg);
    }
}

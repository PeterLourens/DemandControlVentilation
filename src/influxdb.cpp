#include "influxdb.h"

void write_sensor_data(void)
{
    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};

    String state_tmp = "";
    String message = "";
    String sensor_valve = "";
    String sensor_location = "";

    float queue_sensor_data[2][8][3];

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

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state_tmp = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    InfluxDBClient client(influxdb_url, influxdb_org, influxdb_bucket, influxdb_token);
    Point sensor("Sensors");

    if (xQueuePeek(sensor_queue, &queue_sensor_data, 0) == pdTRUE)
    {

        // Check server connection. Only write data when server is available.
        if (client.validateConnection())
        {
            // message = "Connection to influxDB validated. Writing sensor data to influxDB.";
            // print_message(message);
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
                                sensor_valve = sensor1settings[j].wire_sensor_valve;
                                sensor_location = sensor1settings[j].wire_sensor_location;
                                xSemaphoreGive(settings_sensor1_mutex);
                            }

                            if (!sensor_valve.isEmpty())
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (!sensor_location.isEmpty())
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        else
                        {
                            if (settings_sensor2_mutex != NULL)
                            {
                                if (xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10) == pdTRUE)
                                {
                                    sensor_valve = sensor2settings[j].wire1_sensor_valve;
                                    sensor_location = sensor2settings[j].wire1_sensor_location;
                                    xSemaphoreGive(settings_sensor2_mutex);
                                }
                            }
                            if (!sensor_valve.isEmpty())
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (!sensor_location.isEmpty())
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        String tag = "sensor" + String(j);
                        String bus = "bus" + String(i);
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
                            message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
                            print_message(message);
                        }
                        vTaskDelay(50);
                    }
                }
            }
        }
        else
        {
            message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
            print_message(message);
        }
    }
}

void write_avg_sensor_data(void)
{
    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};

    String message = "";
    String sensor_valve = "";
    String sensor_location = "";

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

    float queue_avg_sensor_data[2][8][3];

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
                            if (settings_sensor1_mutex != NULL)
                            {
                                if (xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10) == pdTRUE)
                                {
                                    sensor_valve = sensor1settings[j].wire_sensor_valve;
                                    sensor_location = sensor1settings[j].wire_sensor_location;
                                    xSemaphoreGive(settings_sensor1_mutex);
                                }
                            }
                            if (!sensor_valve.isEmpty())
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (!sensor_location.isEmpty())
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        else
                        {
                            if (settings_sensor2_mutex != NULL)
                            {
                                if (xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10) == pdTRUE)
                                {
                                    sensor_valve = sensor2settings[j].wire1_sensor_valve;
                                    sensor_location = sensor2settings[j].wire1_sensor_location;
                                    xSemaphoreGive(settings_sensor2_mutex);
                                }
                            }
                            if (!sensor_valve.isEmpty())
                            {
                                sensor.addTag("valve", sensor_valve);
                            }
                            if (!sensor_location.isEmpty())
                            {
                                sensor.addTag("location", sensor_location);
                            }
                        }
                        String tag = "sensor" + String(j);
                        String bus = "bus" + String(i);
                        sensor.addTag("device", tag);
                        sensor.addTag("bus", bus);
                        sensor.addField("temperature", queue_avg_sensor_data[i][j][0]);
                        sensor.addField("humidity", queue_avg_sensor_data[i][j][1]);
                        sensor.addField("CO2", queue_avg_sensor_data[i][j][2]);
                        client.pointToLineProtocol(sensor);
                        if (!client.writePoint(sensor))
                        {
                            message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
                            print_message(message);
                        }
                        vTaskDelay(50);
                    }
                }
            }
        }
        else
        {
            message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
            print_message(message);
        }
    }
}

void write_valve_position_data(void)
{
    bool status_file_present = false;
    int valve_pos_temp = 0;
    int valve_pos_sum = 0;

    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};

    char buffer[512];

    String json = "";
    String message = "";

    JsonDocument doc;

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

    if (read_settings(VALVE_POSITIONS_PATH, buffer, sizeof(buffer), valve_position_file_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            message = "[ERROR] Failed to parse: " + String(VALVE_POSITIONS_PATH) + " with error: " + String(error.c_str());
            print_message(message);
        }
    }

    if (client.validateConnection())
    {
        for (int i = 0; i < 12; i++)
        {

            valve_pos_temp = doc["valve" + String(i)];
            valve_pos_sum = valve_pos_sum + valve_pos_temp;

            if (valve_pos_sum != 0)
            {
                sensor.clearFields();
                sensor.clearTags();
                String tag = "valve" + String(i);
                sensor.addTag("device", tag);
                sensor.addField("position", valve_pos_temp);

                client.pointToLineProtocol(sensor);

                if (!client.writePoint(sensor))
                {
                    message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
                    print_message(message);
                }
            }
        }
    }
    else
    {
        message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
        print_message(message);
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

    String message = "";

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
            message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
            print_message(message);
        }
    }
    else
    {
        message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
        print_message(message);
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

    String temp_state = "";
    String message = "";

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

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_state = state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    // Need to translate state to number for easy processing in Grafana
    if (temp_state == "init")
    {
        temp_state_nr = 1;
    }
    else if (temp_state == "night")
    {
        temp_state_nr = 2;
    }
    else if (temp_state == "day")
    {
        temp_state_nr = 3;
    }
    else if (temp_state == "highco2night")
    {
        temp_state_nr = 4;
    }
    else if (temp_state == "highco2day")
    {
        temp_state_nr = 5;
    }
    else if (temp_state == "manualhighspeed")
    {
        temp_state_nr = 6;
    }
    else if (temp_state == "cyclingnight")
    {
        temp_state_nr = 7;
    }
    else if (temp_state == "cyclingday")
    {
        temp_state_nr = 8;
    }
    else if (temp_state == "cooking")
    {
        temp_state_nr = 9;
    }
    else if (temp_state == "highrhnight")
    {
        temp_state_nr = 10;
    }
    else if (temp_state == "highrhday")
    {
        temp_state_nr = 11;
    }
    else if (temp_state == "stopped")
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
            message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
            print_message(message);
        }
    }
    else
    {
        message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
        print_message(message);
    }
}

void write_fanspeed(void)
{
    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    char influxdb_url[XXLARGE_CONFIG_ITEM] = {};
    char influxdb_org[LARGE_CONFIG_ITEM] = {};
    char influxdb_bucket[LARGE_CONFIG_ITEM] = {};
    char influxdb_token[XXLARGE_CONFIG_ITEM] = {};

    String temp_fanspeed = "";
    String message = "";

    int temp_fanspeed_nr = 0;

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

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_fanspeed = fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    // Need to translate fanspeed to number for easy processing in Grafana
    if (temp_fanspeed == "Low")
    {
        temp_fanspeed_nr = 1;
    }
    else if (temp_fanspeed == "Medium")
    {
        temp_fanspeed_nr = 2;
    }
    else if (temp_fanspeed == "High")
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
            message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
            print_message(message);
        }
    }
    else
    {
        message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
        print_message(message);
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

    String message = "";

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

    free_heap_size = xPortGetFreeHeapSize();
    message = "Free heap size: " + String(free_heap_size);
    print_message(message);

    minimum_ever_free_heap_size = xPortGetMinimumEverFreeHeapSize();
    message = "Minimum ever free heap size: " + String(minimum_ever_free_heap_size);
    print_message(message);

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
            message = "InfluxDB write failed: " + String(client.getLastErrorMessage());
            print_message(message);
        }
    }
    else
    {
        message = "InfluxDB connection failed: " + String(client.getLastErrorMessage());
        print_message(message);
    }
}

#include "websocket.h"

/*
1. Get data
    a. Valve positions from file
    b. sensor data from queue
    c. time from global var
    d.
2. Merge everything in one JsonDocument
3. Serialise JsonDocument
4. Send string over websocket
*/

String create_index_json()
{
    const char *path = "/json/valvepositions.json";
    bool status_valve_file_present = 0;
    float temp_sensor_data[2][8][3];

    String json = "";
    String json_valves = "";
    String message = "";
    String state_tmp = "";
    String fanspeed_tmp = "";
    String date_time = "";

    JsonDocument doc;
    JsonDocument wire_sensor_data_temp;
    JsonDocument wire1_sensor_data_temp;

    // Read from sensor queue
    if (sensor_queue != 0)
    {
        if (xQueuePeek(sensor_queue, &temp_sensor_data, (TickType_t)10))
        {
        }
    }

    /// Read setting for valve and valve name
    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            wire_sensor_data_temp = wire_sensor_data;
            wire1_sensor_data_temp = wire1_sensor_data;
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }

    // Valve positions
    if (valve_position_file_mutex != NULL)
    {
        if (xSemaphoreTake(valve_position_file_mutex, (TickType_t)10) == pdTRUE)
        {
            status_valve_file_present = check_file_exists(path);
            if (status_valve_file_present == 1)
            {
                json_valves = read_config_file(path);
            }
            xSemaphoreGive(valve_position_file_mutex);
        }
    }

    DeserializationError err = deserializeJson(doc, json_valves);
    if (err)
    {
        message = "[ERROR] Failed to parse valvepositions.json: " + String(path) + ": " + String(err.c_str());
        print_message(message);
        return "";
    }

    // General
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state_tmp = state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed_tmp = fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    date_time = formatted_datetime();

    doc["statemachine_state"] = state_tmp;
    doc["fanspeed"] = fanspeed_tmp;
    doc["uptime"] = String(esp_timer_get_time() / 1000000 / 60);
    doc["date_time"] = date_time;

    for (int i = 0; i < 12; i++)
    {
        // Bus0 sensor data
        doc["bus0_sensor" + String(i) + "_type"] = wire_sensor_data_temp["wire_sensor" + String(i)]["type"];
        doc["bus0_sensor" + String(i) + "_valve"] = wire_sensor_data_temp["wire_sensor" + String(i)]["valve"];
        doc["bus0_sensor" + String(i) + "_location"] = wire_sensor_data_temp["wire_sensor" + String(i)]["location"];
        doc["bus0_sensor" + String(i) + "_rhs"] = wire_sensor_data_temp["wire_sensor" + String(i)]["rhs"];
        doc["bus0_sensor" + String(i) + "_co2s"] = wire_sensor_data_temp["wire_sensor" + String(i)]["co2s"];
        doc["bus0_sensor" + String(i) + "_temp"] = temp_sensor_data[0][i][0];
        doc["bus0_sensor" + String(i) + "_temp"] = temp_sensor_data[0][i][1];
        doc["bus0_sensor" + String(i) + "_temp"] = temp_sensor_data[0][i][2];

        // Bus1 sensor data
        doc["bus1_sensor" + String(i) + "_type"] = wire1_sensor_data_temp["wire1_sensor" + String(i)]["type"];
        doc["bus1_sensor" + String(i) + "_valve"] = wire1_sensor_data_temp["wire1_sensor" + String(i)]["valve"];
        doc["bus1_sensor" + String(i) + "_location"] = wire1_sensor_data_temp["wire1_sensor" + String(i)]["location"];
        doc["bus1_sensor" + String(i) + "_rhs"] = wire1_sensor_data_temp["wire1_sensor" + String(i)]["rhs"];
        doc["bus1_sensor" + String(i) + "_co2s"] = wire1_sensor_data_temp["wire1_sensor" + String(i)]["co2s"];
        doc["bus1_sensor" + String(i) + "_temp"] = temp_sensor_data[1][i][0];
        doc["bus1_sensor" + String(i) + "_temp"] = temp_sensor_data[1][i][1];
        doc["bus1_sensor" + String(i) + "_temp"] = temp_sensor_data[1][i][2];
    }

    // convert to JSON string
    serializeJson(doc, json);
    return json;
}

String create_settings_json()
{
    const char *path_rtc = "/json/settings_rtc.json";
    const char *path_influxdb = "/json/settings_influxdb.json";
    const char *path_i2c = "/json/settings_i2c.json";
    const char *path_mqtt = "/json/settings_mqtt.json";
    const char *path_fan = "/json/settings_fan.json";
    const char *path_network = "/json/settings_network.json";

    bool settings_rtc_file_present = 0;
    bool settings_influxdb_file_present = 0;
    bool settings_i2c_file_present = 0;
    bool settings_mqtt_file_present = 0;
    bool settings_fan_file_present = 0;
    bool network_config_file_present = 0;

    String settings_json = "";
    String settings_rtc_string = "";
    String settings_influxdb_string = "";
    String settings_i2c_string = "";
    String settings_mqtt_string = "";
    String settings_fan_string = "";
    String settings_network_string = "";
    String message = "";

    // RTC settings
    if (settings_rtc_mutex != NULL)
    {
        if (xSemaphoreTake(settings_rtc_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_rtc_file_present = check_file_exists(path_rtc);
            if (settings_rtc_file_present == 1)
            {
                settings_rtc_string = read_config_file(path_rtc);
            }
            xSemaphoreGive(settings_rtc_mutex);
        }
    }
    if (settings_rtc_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = settings_rtc_string;
    }

    // Influxdb settings
    if (settings_influxdb_mutex != NULL)
    {
        if (xSemaphoreTake(settings_influxdb_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_influxdb_file_present = check_file_exists(path_influxdb);
            if (settings_influxdb_file_present == 1)
            {
                settings_influxdb_string = read_config_file(path_influxdb);
            }
            xSemaphoreGive(settings_influxdb_mutex);
        }
    }
    if (settings_influxdb_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_influxdb_string);
    }

    // I2C config
    if (settings_i2c_mutex != NULL)
    {
        if (xSemaphoreTake(settings_i2c_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_i2c_file_present = check_file_exists(path_i2c);
            if (settings_i2c_file_present == 1)
            {
                settings_i2c_string = read_config_file(path_i2c);
            }
            xSemaphoreGive(settings_i2c_mutex);
        }
    }
    if (settings_i2c_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_i2c_string);
    }

    // MQTT Config
    if (settings_mqtt_mutex != NULL)
    {
        if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_mqtt_file_present = check_file_exists(path_mqtt);
            if (settings_mqtt_file_present == 1)
            {
                settings_mqtt_string = read_config_file(path_mqtt);
            }
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }
    if (settings_mqtt_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_mqtt_string);
    }

    // Fan config
    if (settings_fan_mutex != NULL)
    {
        if (xSemaphoreTake(settings_fan_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_fan_file_present = check_file_exists(path_fan);
            if (settings_fan_file_present == 1)
            {
                settings_fan_string = read_config_file(path_fan);
            }
            xSemaphoreGive(settings_fan_mutex);
        }
    }
    if (settings_fan_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_fan_string);
    }

    // Network config
    if (settings_network_mutex != NULL)
    {
        if (xSemaphoreTake(settings_network_mutex, (TickType_t)100) == pdTRUE)
        {
            network_config_file_present = check_file_exists(path_network);
            if (network_config_file_present == 1)
            {
                settings_network_string = read_config_file(path_network);
            }
            xSemaphoreGive(settings_network_mutex);
        }
    }
    if (settings_fan_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_network_string);
    }

    return settings_json;
}

String create_valvecontrol_json()
{
    String valvecontrol_json = "";
    return valvecontrol_json;
}

String create_sensors_json()
{
    String sensors_json = "";
    return sensors_json;
}

String create_statemachine_json()
{
    String statemachine_json = "";
    return statemachine_json;
}

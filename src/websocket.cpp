#include "websocket.h"

/*
1. Get data
    a. Valve positions from file
    b. sensor data from queue
    c. time from global var
2. Merge everything in one JsonDocument
3. Serialise JsonDocument
4. Send string over websocket
*/

String create_index_json()
{
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
            status_valve_file_present = check_file_exists(VALVE_POSITIONS_PATH);
            if (status_valve_file_present == 1)
            {
                json_valves = read_config_file(VALVE_POSITIONS_PATH);
            }
            xSemaphoreGive(valve_position_file_mutex);
        }
    }

    DeserializationError err = deserializeJson(doc, json_valves);
    if (err)
    {
        message = "[ERROR] Failed to parse: " + String(VALVE_POSITIONS_PATH) + " with error: " + String(err.c_str());
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
    doc["uptime"] = formatted_uptime();
    doc["date_time"] = date_time;

    for (int i = 0; i < 12; i++)
    {
        // Bus0 sensor data
        doc["bus0_sensor" + String(i) + "_type"] = wire_sensor_data_temp["wire_sensor" + String(i) + "_type"];
        doc["bus0_sensor" + String(i) + "_valve"] = wire_sensor_data_temp["wire_sensor" + String(i) + "_valve"];
        doc["bus0_sensor" + String(i) + "_location"] = wire_sensor_data_temp["wire_sensor" + String(i) + "_location"];
        doc["bus0_sensor" + String(i) + "_rhs"] = wire_sensor_data_temp["wire_sensor" + String(i) + "_rh"];
        doc["bus0_sensor" + String(i) + "_co2s"] = wire_sensor_data_temp["wire_sensor" + String(i) + "_co2"];
        doc["bus0_sensor" + String(i) + "_temp"] = roundToTwoDecimals(temp_sensor_data[0][i][0]);
        doc["bus0_sensor" + String(i) + "_hum"] = roundToTwoDecimals(temp_sensor_data[0][i][1]);
        doc["bus0_sensor" + String(i) + "_co2"] = roundToTwoDecimals(temp_sensor_data[0][i][2]);

        // Bus1 sensor data
        doc["bus1_sensor" + String(i) + "_type"] = wire1_sensor_data_temp["wire1_sensor" + String(i) + "_type"];
        doc["bus1_sensor" + String(i) + "_valve"] = wire1_sensor_data_temp["wire1_sensor" + String(i) + "_valve"];
        doc["bus1_sensor" + String(i) + "_location"] = wire1_sensor_data_temp["wire1_sensor" + String(i) + "_location"];
        doc["bus1_sensor" + String(i) + "_rhs"] = wire1_sensor_data_temp["wire1_sensor" + String(i) + "_rh"];
        doc["bus1_sensor" + String(i) + "_co2s"] = wire1_sensor_data_temp["wire1_sensor" + String(i) + "_co2"];
        doc["bus1_sensor" + String(i) + "_temp"] = roundToTwoDecimals(temp_sensor_data[1][i][0]);
        doc["bus1_sensor" + String(i) + "_hum"] = roundToTwoDecimals(temp_sensor_data[1][i][1]);
        doc["bus1_sensor" + String(i) + "_co2"] = roundToTwoDecimals(temp_sensor_data[1][i][2]);
    }

    // convert to JSON string
    serializeJson(doc, json);
    return json;
}

String create_settings_json()
{
    String settings_json = "";
    String settings_rtc_string = "";
    String settings_influxdb_string = "";
    String settings_i2c_string = "";
    String settings_mqtt_string = "";
    String settings_fan_string = "";
    String settings_network_string = "";
    String message = "";

    // RTC settings
    settings_rtc_string = read_rtc_config();

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
    settings_influxdb_string = read_influxdb_config();

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
    settings_i2c_string = read_i2c_config();
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
    settings_mqtt_string = read_mqtt_config();
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
    settings_fan_string = read_fan_config();
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
    settings_network_string = read_network_config();
    if (settings_network_string == "")
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

String create_sensors_json()
{
    bool sensor_config1_file_present = 0;
    bool sensor_config2_file_present = 0;

    String sensor_config1_string = "";
    String sensor_config2_string = "";
    String message = "";
    String sensors_json = "";

    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            sensor_config1_file_present = check_file_exists(SENSOR_CONFIG1_PATH);
            if (sensor_config1_file_present == 1)
            {
                sensor_config1_string = read_config_file(SENSOR_CONFIG1_PATH);
            }
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }
    if (sensor_config1_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        sensors_json = sensor_config1_string;
    }

    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            sensor_config2_file_present = check_file_exists(SENSOR_CONFIG2_PATH);
            if (sensor_config2_file_present == 1)
            {
                sensor_config2_string = read_config_file(SENSOR_CONFIG2_PATH);
            }
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }
    if (sensor_config2_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        sensors_json = concatJson(sensors_json, sensor_config2_string);
    }

    return sensors_json;
}

String create_statemachine_json()
{
    bool settings_statemachine_present = 0;
    bool settings_state_day_present = 0;
    bool settings_state_night_present = 0;
    bool settings_state_highco2day_present = 0;
    bool settings_state_highco2night_present = 0;
    bool settings_state_highrhday_present = 0;
    bool settings_state_highrhnight_present = 0;
    bool settings_state_cooking_present = 0;
    bool settings_state_cyclingday_present = 0;
    bool settings_state_cyclingnight_present = 0;

    String settings_statemachine_str = "";
    String settings_state_day_str = "";
    String settings_state_night_str = "";
    String settings_state_highco2day_str = "";
    String settings_state_highco2night_str = "";
    String settings_state_highrhday_str = "";
    String settings_state_highrhnight_str = "";
    String settings_state_cooking_str = "";
    String settings_state_cyclingday_str = "";
    String settings_state_cyclingnight_str = "";
    String statemachine_json = "";
    String message = "";

    if (settings_statemachine_mutex != NULL)
    {
        if (xSemaphoreTake(settings_statemachine_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_statemachine_present = check_file_exists(SETTINGS_STATEMACHINE_PATH);
            if (settings_statemachine_present == 1)
            {
                settings_statemachine_str = read_config_file(SETTINGS_STATEMACHINE_PATH);
            }
            xSemaphoreGive(settings_statemachine_mutex);
        }
    }
    if (settings_statemachine_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = settings_statemachine_str;
    }

    if (settings_state_day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_day_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_day_present = check_file_exists(SETTINGS_STATE_DAY_PATH);
            if (settings_state_day_present == 1)
            {
                settings_state_day_str = read_config_file(SETTINGS_STATE_DAY_PATH);
            }
            xSemaphoreGive(settings_state_day_mutex);
        }
    }
    if (settings_state_day_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_day_str);
    }

    if (settings_state_night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_night_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_night_present = check_file_exists(SETTINGS_STATE_NIGHT_PATH);
            if (settings_state_night_present == 1)
            {
                settings_state_night_str = read_config_file(SETTINGS_STATE_NIGHT_PATH);
            }
            xSemaphoreGive(settings_state_night_mutex);
        }
    }
    if (settings_state_night_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_night_str);
    }

    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highco2day_present = check_file_exists(SETTINGS_STATE_HIGHCO2DAY_PATH);
            if (settings_state_highco2day_present == 1)
            {
                settings_state_highco2day_str = read_config_file(SETTINGS_STATE_HIGHCO2DAY_PATH);
            }
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }
    if (settings_state_highco2day_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highco2day_str);
    }

    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highco2night_present = check_file_exists(SETTINGS_STATE_HIGHCO2NIGHT_PATH);
            if (settings_state_highco2night_present == 1)
            {
                settings_state_highco2night_str = read_config_file(SETTINGS_STATE_HIGHCO2NIGHT_PATH);
            }
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }
    if (settings_state_highco2night_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highco2night_str);
    }

    if (settings_state_highrhday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highrhday_present = check_file_exists(SETTINGS_STATE_HIGHRHDAY_PATH);
            if (settings_state_highrhday_present == 1)
            {
                settings_state_highrhday_str = read_config_file(SETTINGS_STATE_HIGHRHDAY_PATH);
            }
            xSemaphoreGive(settings_state_highrhday_mutex);
        }
    }
    if (settings_state_highrhday_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highrhday_str);
    }

    if (settings_state_highrhnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highrhnight_present = check_file_exists(SETTINGS_STATE_HIGHRHNIGHT_PATH);
            if (settings_state_highrhnight_present == 1)
            {
                settings_state_highrhnight_str = read_config_file(SETTINGS_STATE_HIGHRHNIGHT_PATH);
            }
            xSemaphoreGive(settings_state_highrhnight_mutex);
        }
    }
    if (settings_state_highrhnight_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highrhnight_str);
    }

    if (settings_state_cooking_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_cooking_present = check_file_exists(SETTINGS_STATE_COOKING_PATH);
            if (settings_state_cooking_present == 1)
            {
                settings_state_cooking_str = read_config_file(SETTINGS_STATE_COOKING_PATH);
            }
            xSemaphoreGive(settings_state_cooking_mutex);
        }
    }
    if (settings_state_cooking_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_cooking_str);
    }

    if (settings_state_cyclingday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_cyclingday_present = check_file_exists(SETTINGS_STATE_CYCLINGDAY_PATH);
            if (settings_state_cyclingday_present == 1)
            {
                settings_state_cyclingday_str = read_config_file(SETTINGS_STATE_CYCLINGDAY_PATH);
            }
            xSemaphoreGive(settings_state_cyclingday_mutex);
        }
    }
    if (settings_state_cyclingday_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_cyclingday_str);
    }

    if (settings_state_cyclingnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_cyclingnight_present = check_file_exists(SETTINGS_STATE_CYCLINGNIGHT_PATH);
            if (settings_state_cyclingnight_present == 1)
            {
                settings_state_cyclingnight_str = read_config_file(SETTINGS_STATE_CYCLINGNIGHT_PATH);
            }
            xSemaphoreGive(settings_state_cyclingnight_mutex);
        }
    }
    if (settings_state_cyclingnight_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_cyclingnight_str);
    }

    return statemachine_json;
}

String create_valvecontrol_json()
{

    bool status_file_present;

    String json;
    String valve_status_file_state = "";
    String temp_state = "";
    String valvecontrol_json = "";

    JsonDocument doc;

    status_file_present = check_file_exists(VALVE_POSITIONS_PATH);

    if (status_file_present == 1)
    {
        valve_status_file_state = "<font color=\"green\">Valve status file found.</font>";
        doc["status_valve_position_file"] = valve_status_file_state;
    }
    else
    {
        valve_status_file_state = "<font color=\"red\">Valve status file not found. Create a file with button below.</font>";
        doc["status_valve_position_file"] = valve_status_file_state;
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_state = state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    doc["statemachine_state"] = temp_state;
    serializeJson(doc, valvecontrol_json);

    return valvecontrol_json;
}
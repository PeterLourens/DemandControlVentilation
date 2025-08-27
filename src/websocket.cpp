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
    // JsonDocument wire_sensor_data_temp;
    // JsonDocument wire1_sensor_data_temp;

    // Read from sensor queue
    if (sensor_queue != 0)
    {
        if (xQueuePeek(sensor_queue, &temp_sensor_data, (TickType_t)10))
        {
        }
    }

    // Read setting for valve and valve name
    /*if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            wire_sensor_data_temp = wire_sensor_data;
            wire1_sensor_data_temp = wire1_sensor_data;
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }*/

    /*for (int i = 0; i < 8; i++)
    {
        wire_sensor_data_temp["wire_sensor" + String(i) + "_type"] = sensor1settings[i].wire_sensor_type;
        wire_sensor_data_temp["wire_sensor" + String(i) + "_valve"] = sensor1settings[i].wire_sensor_valve;
        wire_sensor_data_temp["wire_sensor" + String(i) + "_location"] = sensor1settings[i].wire_sensor_location;
        wire_sensor_data_temp["wire_sensor" + String(i) + "_rh"] = sensor1settings[i].wire_sensor_rh;
        wire_sensor_data_temp["wire_sensor" + String(i) + "_co2"] = sensor1settings[i].wire_sensor_co2;

        wire1_sensor_data_temp["wire1_sensor" + String(i) + "_type"] = sensor2settings[i].wire1_sensor_type;
        wire1_sensor_data_temp["wire1_sensor" + String(i) + "_valve"] = sensor2settings[i].wire1_sensor_valve;
        wire1_sensor_data_temp["wire1_sensor" + String(i) + "_location"] = sensor2settings[i].wire1_sensor_location;
        wire1_sensor_data_temp["wire1_sensor" + String(i) + "_rh"] = sensor2settings[i].wire1_sensor_rh;
        wire1_sensor_data_temp["wire1_sensor" + String(i) + "_co2"] = sensor2settings[i].wire1_sensor_co2;
    }*/

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

    for (int i = 0; i < 8; i++)
    {
        // Bus0 sensor data
        /*doc["bus0_sensor" + String(i) + "_type"] = wire_sensor_data_temp["wire_sensor" + String(i) + "_type"];
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
        doc["bus1_sensor" + String(i) + "_co2"] = roundToTwoDecimals(temp_sensor_data[1][i][2]);*/

        // Bus 0 sensor data
        if (settings_sensor1_mutex != NULL)
        {
            if (xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10))
            {
                doc["bus0_sensor" + String(i) + "_type"] = sensor1settings[i].wire_sensor_type;
                doc["bus0_sensor" + String(i) + "_valve"] = sensor1settings[i].wire_sensor_valve;
                doc["bus0_sensor" + String(i) + "_location"] = sensor1settings[i].wire_sensor_location;
                doc["bus0_sensor" + String(i) + "_rhs"] = sensor1settings[i].wire_sensor_rh;
                doc["bus0_sensor" + String(i) + "_co2s"] = sensor1settings[i].wire_sensor_co2;
                xSemaphoreGive(settings_sensor1_mutex);
            }
        }
        doc["bus0_sensor" + String(i) + "_temp"] = roundToTwoDecimals(temp_sensor_data[0][i][0]);
        doc["bus0_sensor" + String(i) + "_hum"] = roundToTwoDecimals(temp_sensor_data[0][i][1]);
        doc["bus0_sensor" + String(i) + "_co2"] = roundToTwoDecimals(temp_sensor_data[0][i][2]);

        // Bus1 sensor data
        if (settings_sensor2_mutex != NULL)
        {
            if (xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10))
            {
                doc["bus1_sensor" + String(i) + "_type"] = sensor2settings[i].wire1_sensor_type;
                doc["bus1_sensor" + String(i) + "_valve"] = sensor2settings[i].wire1_sensor_valve;
                doc["bus1_sensor" + String(i) + "_location"] = sensor2settings[i].wire1_sensor_location;
                doc["bus1_sensor" + String(i) + "_rhs"] = sensor2settings[i].wire1_sensor_rh;
                doc["bus1_sensor" + String(i) + "_co2s"] = sensor2settings[i].wire1_sensor_co2;
                xSemaphoreGive(settings_sensor2_mutex);
            }
        }
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
        message = "[ERROR] RTC Config string is empty or failed to read file";
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
        message = "[ERROR] Influxdb config string is empty or failed to read file";
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
        message = "[ERROR] I2C config string is empty or failed to read file";
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
        message = "[ERROR] MQTT config string is empty or failed to read file";
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
        message = "[ERROR] Fan config string is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_fan_string);
    }

    // Network config
    settings_network_string = read_network_config();
    // Serial.print("Settings in websocket: " + settings_network_string);
    if (settings_network_string == "")
    {
        message = "[ERROR] Network config string is empty or failed to read file";
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
    JsonDocument doc1;
    JsonDocument doc2;
    String sensor_config1_str = "";
    String sensor_config2_str = "";
    String message = "";
    String sensors_json = "";

    if (settings_sensor1_mutex != NULL)
    {
        if (xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10))
        {
            for (int i = 0; i < 8; i++)
            {
                doc1["wire_sensor" + String(i) + "_type"] = sensor1settings[i].wire_sensor_type;
                doc1["wire_sensor" + String(i) + "_valve"] = sensor1settings[i].wire_sensor_valve;
                doc1["wire_sensor" + String(i) + "_location"] = sensor1settings[i].wire_sensor_location;
                doc1["wire_sensor" + String(i) + "_rh"] = sensor1settings[i].wire_sensor_rh;
                doc1["wire_sensor" + String(i) + "_co2"] = sensor1settings[i].wire_sensor_co2;
            }
            xSemaphoreGive(settings_sensor1_mutex);
        }
    }

    doc1.shrinkToFit(); // optional
    serializeJson(doc1, sensor_config1_str);

    if (settings_sensor2_mutex != NULL)
    {
        if (xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10))
        {
            for (int i = 0; i < 8; i++)
            {
                doc2["wire1_sensor" + String(i) + "_type"] = sensor2settings[i].wire1_sensor_type;
                doc2["wire1_sensor" + String(i) + "_valve"] = sensor2settings[i].wire1_sensor_valve;
                doc2["wire1_sensor" + String(i) + "_location"] = sensor2settings[i].wire1_sensor_location;
                doc2["wire1_sensor" + String(i) + "_rh"] = sensor2settings[i].wire1_sensor_rh;
                doc2["wire1_sensor" + String(i) + "_co2"] = sensor2settings[i].wire1_sensor_co2;
            }
            xSemaphoreGive(settings_sensor2_mutex);
        }
    }

    doc2.shrinkToFit();
    serializeJson(doc2, sensor_config2_str);

    if (sensor_config1_str == "" || sensor_config2_str == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return "";
    }
    else
    {
        sensors_json = concatJson(sensor_config1_str, sensor_config2_str);
        // Serial.print("Sensors json: " + sensors_json + "\n");
    }

    return sensors_json;
}

String create_statemachine_json()
{
    // bool settings_statemachine_present = 0;
    //  bool settings_state_day_present = 0;
    //  bool settings_state_night_present = 0;
    // bool settings_state_highco2day_present = 0;
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

    JsonDocument doc_statemachine;
    JsonDocument doc_day;
    JsonDocument doc_night;
    JsonDocument doc_highco2day;
    JsonDocument doc_highco2night;
    JsonDocument doc_highrhday;
    JsonDocument doc_highrhnight;
    JsonDocument doc_cooking;
    JsonDocument doc_cyclingday;
    JsonDocument doc_cyclingnight;

    // Statemachine settings
    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_statemachine["weekday_day_hour_start"] = statemachinesettings.weekday_day_hour_start;
        doc_statemachine["weekday_day_minute_start"] = statemachinesettings.weekday_day_minute_start;
        doc_statemachine["weekday_night_hour_start"] = statemachinesettings.weekday_night_hour_start;
        doc_statemachine["weekday_night_minute_start"] = statemachinesettings.weekday_night_minute_start;
        doc_statemachine["weekend_day_hour_start"] = statemachinesettings.weekend_day_hour_start;
        doc_statemachine["weekend_day_minute_start"] = statemachinesettings.weekend_day_minute_start;
        doc_statemachine["weekend_night_hour_start"] = statemachinesettings.weekend_night_hour_start;
        doc_statemachine["weekend_night_minute_start"] = statemachinesettings.weekend_night_minute_start;
        doc_statemachine["weekend_day_1"] = statemachinesettings.weekend_day_1;
        doc_statemachine["weekend_day_2"] = statemachinesettings.weekend_day_2;
        doc_statemachine["minimum_state_time"] = statemachinesettings.minimum_state_time;
        xSemaphoreGive(settings_statemachine_mutex);
    }

    doc_statemachine.shrinkToFit();
    serializeJson(doc_statemachine, settings_statemachine_str);

    if (settings_statemachine_str == "")
    {
        message = "[ERROR] String is empty. Failed to read statemachine settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = settings_statemachine_str;
    }

    // State day settings
    if (settings_state_day_mutex && xSemaphoreTake(settings_state_day_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_day["enable_state_day"] = statedaysettings.enable_state_day;
        doc_day["state_day_fanspeed"] = statedaysettings.state_day_fanspeed;
        doc_day["name_state_day"] = statedaysettings.name_state_day;
        doc_day["state_day_highco2"] = statedaysettings.state_day_highco2;
        doc_day["state_day_highrh"] = statedaysettings.state_day_highrh;
        doc_day["valve0_position_day"] = statedaysettings.valve0_position_day;
        doc_day["valve1_position_day"] = statedaysettings.valve1_position_day;
        doc_day["valve2_position_day"] = statedaysettings.valve2_position_day;
        doc_day["valve3_position_day"] = statedaysettings.valve3_position_day;
        doc_day["valve4_position_day"] = statedaysettings.valve4_position_day;
        doc_day["valve5_position_day"] = statedaysettings.valve5_position_day;
        doc_day["valve6_position_day"] = statedaysettings.valve6_position_day;
        doc_day["valve7_position_day"] = statedaysettings.valve7_position_day;
        doc_day["valve8_position_day"] = statedaysettings.valve8_position_day;
        doc_day["valve9_position_day"] = statedaysettings.valve9_position_day;
        doc_day["valve10_position_day"] = statedaysettings.valve10_position_day;
        doc_day["valve11_position_day"] = statedaysettings.valve11_position_day;
        xSemaphoreGive(settings_state_day_mutex);
    }

    doc_day.shrinkToFit();
    serializeJson(doc_day, settings_state_day_str);

    if (settings_state_day_str == "")
    {
        message = "[ERROR] String is empty. Failed to read day state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_day_str);
    }

    // State night settings
    if (settings_state_night_mutex && xSemaphoreTake(settings_state_night_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_night["enable_state_night"] = statenightsettings.enable_state_night;
        doc_night["state_night_fanspeed"] = statenightsettings.state_night_fanspeed;
        doc_night["name_state_night"] = statenightsettings.name_state_night;
        doc_night["state_night_highco2"] = statenightsettings.state_night_highco2;
        doc_night["state_night_highrh"] = statenightsettings.state_night_highrh;
        doc_night["valve0_position_night"] = statenightsettings.valve0_position_night;
        doc_night["valve1_position_night"] = statenightsettings.valve1_position_night;
        doc_night["valve2_position_night"] = statenightsettings.valve2_position_night;
        doc_night["valve3_position_night"] = statenightsettings.valve3_position_night;
        doc_night["valve4_position_night"] = statenightsettings.valve4_position_night;
        doc_night["valve5_position_night"] = statenightsettings.valve5_position_night;
        doc_night["valve6_position_night"] = statenightsettings.valve6_position_night;
        doc_night["valve7_position_night"] = statenightsettings.valve7_position_night;
        doc_night["valve8_position_night"] = statenightsettings.valve8_position_night;
        doc_night["valve9_position_night"] = statenightsettings.valve9_position_night;
        doc_night["valve10_position_night"] = statenightsettings.valve10_position_night;
        doc_night["valve11_position_night"] = statenightsettings.valve11_position_night;
        xSemaphoreGive(settings_state_night_mutex);
    }

    doc_night.shrinkToFit();
    serializeJson(doc_night, settings_state_night_str);

    if (settings_state_night_str == "")
    {
        message = "[ERROR] String is empty. Failed to read night state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_night_str);
    }

    // State highco2day settings
    if (settings_state_highco2day_mutex && xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_highco2day["enable_state_highco2day"] = statehighco2daysettings.enable_state_highco2day;
        doc_highco2day["state_highco2day_fanspeed"] = statehighco2daysettings.state_highco2day_fanspeed;
        doc_highco2day["name_state_highco2day"] = statehighco2daysettings.name_state_highco2day;
        doc_highco2day["co2_high_state_highco2day"] = statehighco2daysettings.co2_high_state_highco2day;
        doc_highco2day["co2_low_state_highco2day"] = statehighco2daysettings.co2_low_state_highco2day;
        doc_highco2day["valve0_position_highco2day"] = statehighco2daysettings.valve0_position_highco2day;
        doc_highco2day["valve1_position_highco2day"] = statehighco2daysettings.valve1_position_highco2day;
        doc_highco2day["valve2_position_highco2day"] = statehighco2daysettings.valve2_position_highco2day;
        doc_highco2day["valve3_position_highco2day"] = statehighco2daysettings.valve3_position_highco2day;
        doc_highco2day["valve4_position_highco2day"] = statehighco2daysettings.valve4_position_highco2day;
        doc_highco2day["valve5_position_highco2day"] = statehighco2daysettings.valve5_position_highco2day;
        doc_highco2day["valve6_position_highco2day"] = statehighco2daysettings.valve6_position_highco2day;
        doc_highco2day["valve7_position_highco2day"] = statehighco2daysettings.valve7_position_highco2day;
        doc_highco2day["valve8_position_highco2day"] = statehighco2daysettings.valve8_position_highco2day;
        doc_highco2day["valve9_position_highco2day"] = statehighco2daysettings.valve9_position_highco2day;
        doc_highco2day["valve10_position_highco2day"] = statehighco2daysettings.valve10_position_highco2day;
        doc_highco2day["valve11_position_highco2day"] = statehighco2daysettings.valve11_position_highco2day;
        xSemaphoreGive(settings_state_highco2day_mutex);
    }
    doc_highco2day.shrinkToFit();
    serializeJson(doc_highco2day, settings_state_highco2day_str);
    if (settings_state_highco2day_str == "")
    {
        message = "[ERROR] String is empty. Failed to read high CO2 day state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highco2day_str);
    }

    /*if (settings_statemachine_mutex != NULL)
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
    }*/

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

    // Serial.print("Statemachine json: " + statemachine_json + "\n");
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
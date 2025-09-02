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
    char daydatetime_buffer[50];
    char buffer[512];

    String json = "";
    String json_valves = "";
    String message = "";
    String state_tmp = "";
    String fanspeed_tmp = "";

    JsonDocument doc;

    // Read from sensor queue
    if (sensor_queue != 0)
    {
        if (xQueuePeek(sensor_queue, &temp_sensor_data, (TickType_t)10))
        {
        }
    }

    if (read_settings(VALVE_POSITIONS_PATH, buffer, sizeof(buffer), valve_position_file_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            message = "[ERROR] Failed to parse: " + String(VALVE_POSITIONS_PATH) + " with error: " + String(error.c_str());
            print_message(message);
        }
    }

    // General
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state_tmp = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed_tmp = fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    formatted_daydatetime(daydatetime_buffer, sizeof(daydatetime_buffer));

    doc["statemachine_state"] = state_tmp;
    doc["fanspeed"] = fanspeed_tmp;
    doc["uptime"] = formatted_uptime();
    doc["date_time"] = daydatetime_buffer;

    for (int i = 0; i < 8; i++)
    {
        // Bus 0 sensor data
        if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10))
        {
            doc["bus0_sensor" + String(i) + "_type"] = sensor1settings[i].wire_sensor_type;
            doc["bus0_sensor" + String(i) + "_valve"] = sensor1settings[i].wire_sensor_valve;
            doc["bus0_sensor" + String(i) + "_location"] = sensor1settings[i].wire_sensor_location;
            doc["bus0_sensor" + String(i) + "_rhs"] = sensor1settings[i].wire_sensor_rh;
            doc["bus0_sensor" + String(i) + "_co2s"] = sensor1settings[i].wire_sensor_co2;
            xSemaphoreGive(settings_sensor1_mutex);
        }

        doc["bus0_sensor" + String(i) + "_temp"] = roundToTwoDecimals(temp_sensor_data[0][i][0]);
        doc["bus0_sensor" + String(i) + "_hum"] = roundToTwoDecimals(temp_sensor_data[0][i][1]);
        doc["bus0_sensor" + String(i) + "_co2"] = roundToTwoDecimals(temp_sensor_data[0][i][2]);

        // Bus1 sensor data

        if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10))
        {
            doc["bus1_sensor" + String(i) + "_type"] = sensor2settings[i].wire1_sensor_type;
            doc["bus1_sensor" + String(i) + "_valve"] = sensor2settings[i].wire1_sensor_valve;
            doc["bus1_sensor" + String(i) + "_location"] = sensor2settings[i].wire1_sensor_location;
            doc["bus1_sensor" + String(i) + "_rhs"] = sensor2settings[i].wire1_sensor_rh;
            doc["bus1_sensor" + String(i) + "_co2s"] = sensor2settings[i].wire1_sensor_co2;
            xSemaphoreGive(settings_sensor2_mutex);
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
    String settings_rtc_str = "";
    String settings_influxdb_str = "";
    String settings_i2c_str = "";
    String settings_mqtt_str = "";
    String settings_fan_str = "";
    String settings_network_str = "";
    String message = "";

    JsonDocument settings_network_doc;
    JsonDocument settings_rtc_doc;
    JsonDocument settings_influxdb_doc;
    JsonDocument settings_i2c_doc;
    JsonDocument settings_mqtt_doc;
    JsonDocument settings_fan_doc;

    // Network settings
    if (settings_network_mutex && xSemaphoreTake(settings_network_mutex, (TickType_t)10))
    {
        settings_network_doc["enable_dhcp"] = networksettings.enable_dhcp;
        settings_network_doc["ssid"] = networksettings.ssid;
        settings_network_doc["wifi_password"] = networksettings.wifi_password;
        settings_network_doc["ip_address"] = networksettings.ip_address;
        settings_network_doc["subnet_mask"] = networksettings.subnet_mask;
        settings_network_doc["gateway"] = networksettings.gateway;
        settings_network_doc["primary_dns"] = networksettings.primary_dns;
        settings_network_doc["secondary_dns"] = networksettings.secondary_dns;
        xSemaphoreGive(settings_network_mutex);
    }

    serializeJson(settings_network_doc, settings_network_str);

    if (settings_network_str == "")
    {
        message = "[ERROR] Network settings string is empty.";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = settings_network_str;
    }

    // MQTT settings
    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10))
    {
        settings_mqtt_doc["enable_mqtt"] = mqttsettings.enable_mqtt;
        settings_mqtt_doc["mqtt_server"] = mqttsettings.mqtt_server;
        settings_mqtt_doc["mqtt_port"] = mqttsettings.mqtt_port;
        settings_mqtt_doc["mqtt_base_topic"] = mqttsettings.mqtt_base_topic;
        xSemaphoreGive(settings_mqtt_mutex);
    }

    serializeJson(settings_mqtt_doc, settings_mqtt_str);

    if (settings_mqtt_str == "")
    {
        message = "[ERROR] mqtt settings string is empty.";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_mqtt_str);
    }

    // I2C settings
    if (settings_i2c_mutex && xSemaphoreTake(settings_i2c_mutex, (TickType_t)10))
    {
        settings_i2c_doc["bus0_multiplexer_address"] = i2csettings.bus0_multiplexer_address;
        settings_i2c_doc["bus1_multiplexer_address"] = i2csettings.bus1_multiplexer_address;
        settings_i2c_doc["enable_lcd"] = i2csettings.enable_lcd;
        settings_i2c_doc["display_i2c_address"] = i2csettings.display_i2c_address;
        xSemaphoreGive(settings_i2c_mutex);
    }

    serializeJson(settings_i2c_doc, settings_i2c_str);

    if (settings_i2c_str == "")
    {
        message = "[ERROR] I2C settings string is empty.";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_i2c_str);
    }

    // Fan settings
    if (settings_fan_mutex && xSemaphoreTake(settings_fan_mutex, (TickType_t)10))
    {
        settings_fan_doc["fan_control_mode"] = fansettings.fan_control_mode;
        settings_fan_doc["fan_control_mqtt_server"] = fansettings.fan_control_mqtt_server;
        settings_fan_doc["fan_control_mqtt_port"] = fansettings.fan_control_mqtt_port;
        settings_fan_doc["fan_control_mqtt_topic"] = fansettings.fan_control_mqtt_topic;
        settings_fan_doc["fan_control_url_high_speed"] = fansettings.fan_control_url_high_speed;
        settings_fan_doc["fan_control_url_medium_speed"] = fansettings.fan_control_url_medium_speed;
        settings_fan_doc["fan_control_url_low_speed"] = fansettings.fan_control_url_low_speed;
        xSemaphoreGive(settings_fan_mutex);
    }

    serializeJson(settings_fan_doc, settings_fan_str);

    if (settings_fan_str == "")
    {
        message = "[ERROR] fan settings string is empty.";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_fan_str);
    }

    // Influxdb settings
    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10))
    {
        settings_influxdb_doc["enable_influxdb"] = influxdbsettings.enable_influxdb;
        settings_influxdb_doc["influxdb_url"] = influxdbsettings.influxdb_url;
        settings_influxdb_doc["influxdb_org"] = influxdbsettings.influxdb_org;
        settings_influxdb_doc["influxdb_bucket"] = influxdbsettings.influxdb_bucket;
        settings_influxdb_doc["influxdb_token"] = influxdbsettings.influxdb_token;
        xSemaphoreGive(settings_influxdb_mutex);
    }

    serializeJson(settings_influxdb_doc, settings_influxdb_str);

    if (settings_influxdb_str == "")
    {
        message = "[ERROR] influxdb settings string is empty.";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_influxdb_str);
    }

    // RTC settings
    if (settings_rtc_mutex && xSemaphoreTake(settings_rtc_mutex, (TickType_t)10))
    {
        settings_rtc_doc["ntp_server"] = rtcsettings.ntp_server;
        settings_rtc_doc["timezone"] = rtcsettings.timezone;
        xSemaphoreGive(settings_rtc_mutex);
    }

    serializeJson(settings_rtc_doc, settings_rtc_str);

    if (settings_rtc_str == "")
    {
        message = "[ERROR] RTC settings string is empty.";
        print_message(message);
        return "";
    }
    else
    {
        settings_json = concatJson(settings_json, settings_rtc_str);
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

    if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10))
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

    doc1.shrinkToFit(); // optional
    serializeJson(doc1, sensor_config1_str);

    if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10))
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

        for (int i = 0; i < 12; i++)
        {
            doc_day["valve" + String(i) + "_position_day"] = statedaysettings.valve_position_day[i];
        }
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

        for (int i = 0; i < 12; i++)
        {
            doc_night["valve" + String(i) + "_position_night"] = statenightsettings.valve_position_night[i];
        }
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

        for (int i = 0; i < 12; i++)
        {
            doc_highco2day["valve" + String(i) + "_position_highco2day"] = statehighco2daysettings.valve_position_highco2day[i];
        }
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

    // State highco2night settings
    if (settings_state_highco2night_mutex && xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_highco2night["enable_state_highco2night"] = statehighco2nightsettings.enable_state_highco2night;
        doc_highco2night["state_highco2night_fanspeed"] = statehighco2nightsettings.state_highco2night_fanspeed;
        doc_highco2night["name_state_highco2night"] = statehighco2nightsettings.name_state_highco2night;
        doc_highco2night["co2_high_state_highco2night"] = statehighco2nightsettings.co2_high_state_highco2night;
        doc_highco2night["co2_low_state_highco2night"] = statehighco2nightsettings.co2_low_state_highco2night;

        for (int i = 0; i < 12; i++)
        {
            doc_highco2night["valve" + String(i) + "_position_highco2night"] = statehighco2nightsettings.valve_position_highco2night[i];
        }
        xSemaphoreGive(settings_state_highco2night_mutex);
    }
    doc_highco2night.shrinkToFit();
    serializeJson(doc_highco2night, settings_state_highco2night_str);
    if (settings_state_highco2night_str == "")
    {
        message = "[ERROR] String is empty. Failed to read high CO2 night state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highco2night_str);
    }

    if (settings_state_highrhday_mutex && xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_highrhday["enable_state_highrhday"] = statehighrhdaysettings.enable_state_highrhday;
        doc_highrhday["state_highrhday_fanspeed"] = statehighrhdaysettings.state_highrhday_fanspeed;
        doc_highrhday["name_state_highrhday"] = statehighrhdaysettings.name_state_highrhday;
        doc_highrhday["maximum_state_time_highrhday"] = statehighrhdaysettings.maximum_state_time_highrhday;
        doc_highrhday["rh_low_state_highrhday"] = statehighrhdaysettings.rh_low_state_highrhday;

        for (int i = 0; i < 12; i++)
        {
            doc_highrhday["valve" + String(i) + "_position_highrhday"] = statehighrhdaysettings.valve_position_highrhday[i];
        }
        xSemaphoreGive(settings_state_highrhday_mutex);
    }
    doc_highrhday.shrinkToFit();
    serializeJson(doc_highrhday, settings_state_highrhday_str);
    if (settings_state_highrhday_str == "")
    {
        message = "[ERROR] String is empty. Failed to read high RH day state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highrhday_str);
    }

    if (settings_state_highrhnight_mutex && xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_highrhnight["enable_state_highrhnight"] = statehighrhnightsettings.enable_state_highrhnight;
        doc_highrhnight["state_highrhnight_fanspeed"] = statehighrhnightsettings.state_highrhnight_fanspeed;
        doc_highrhnight["name_state_highrhnight"] = statehighrhnightsettings.name_state_highrhnight;
        doc_highrhnight["maximum_state_time_highrhnight"] = statehighrhnightsettings.maximum_state_time_highrhnight;
        doc_highrhnight["rh_low_state_highrhnight"] = statehighrhnightsettings.rh_low_state_highrhnight;

        for (int i = 0; i < 12; i++)
        {
            doc_highrhnight["valve" + String(i) + "position_highrhnight"] = statehighrhnightsettings.valve_position_highrhnight[i];
        }
        xSemaphoreGive(settings_state_highrhnight_mutex);
    }
    doc_highrhnight.shrinkToFit();
    serializeJson(doc_highrhnight, settings_state_highrhnight_str);
    if (settings_state_highrhnight_str == "")
    {
        message = "[ERROR] String is empty. Failed to read high RH night state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_highrhnight_str);
    }

    // Cooking state settings
    if (settings_state_cooking_mutex && xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)10) == pdTRUE)
    {

        doc_cooking["enable_state_cooking"] = statecookingsettings.enable_state_cooking;
        doc_cooking["state_cooking_fanspeed"] = statecookingsettings.state_cooking_fanspeed;
        doc_cooking["name_state_cooking"] = statecookingsettings.name_state_cooking;
        doc_cooking["start_hour_state_cooking"] = statecookingsettings.start_hour_state_cooking;
        doc_cooking["start_minute_state_cooking"] = statecookingsettings.start_minute_state_cooking;
        doc_cooking["stop_hour_state_cooking"] = statecookingsettings.stop_hour_state_cooking;
        doc_cooking["stop_minute_state_cooking"] = statecookingsettings.stop_minute_state_cooking;
        for (int i = 0; i < 12; i++)
        {
            doc_cooking["valve" + String(i) + "_position_cooking"] = statecookingsettings.valve_position_cooking[i];
        }
        xSemaphoreGive(settings_state_cooking_mutex);
    }
    doc_cooking.shrinkToFit();
    serializeJson(doc_cooking, settings_state_cooking_str);
    if (settings_state_cooking_str == "")
    {
        message = "[ERROR] String is empty. Failed to read cooking state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_cooking_str);
    }

    // Cycling day state settings
    if (settings_state_cyclingday_mutex && xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_cyclingday["enable_state_cyclingday"] = statecyclingdaysettings.enable_state_cyclingday;
        doc_cyclingday["state_cyclingday_fanspeed"] = statecyclingdaysettings.state_cyclingday_fanspeed;
        doc_cyclingday["name_state_cyclingday"] = statecyclingdaysettings.name_state_cyclingday;

        for (int i = 0; i < 12; i++)
        {
            doc_cyclingday["valve" + String(i) + "_position_cyclingday"] = statecyclingdaysettings.valve_position_cyclingday[i];
        }
        xSemaphoreGive(settings_state_cyclingday_mutex);
    }
    doc_cyclingday.shrinkToFit();
    serializeJson(doc_cyclingday, settings_state_cyclingday_str);
    if (settings_state_cyclingday_str == "")
    {
        message = "[ERROR] String is empty. Failed to read cycling day state settings.";
        print_message(message);
        return "";
    }
    else
    {
        statemachine_json = concatJson(statemachine_json, settings_state_cyclingday_str);
    }

    // Cycling night state settings
    if (settings_state_cyclingnight_mutex && xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)10) == pdTRUE)
    {
        doc_cyclingnight["enable_state_cyclingnight"] = statecyclingnightsettings.enable_state_cyclingnight;
        doc_cyclingnight["state_cyclingnight_fanspeed"] = statecyclingnightsettings.state_cyclingnight_fanspeed;
        doc_cyclingnight["name_state_cyclingnight"] = statecyclingnightsettings.name_state_cyclingnight;

        for (int i = 0; i < 12; i++)
        {
            doc_cyclingnight["valve" + String(i) + "_position_cyclingnight"] = statecyclingnightsettings.valve_position_cyclingnight[i];
        }
        xSemaphoreGive(settings_state_cyclingnight_mutex);

        doc_cyclingnight.shrinkToFit();
        serializeJson(doc_cyclingnight, settings_state_cyclingnight_str);
        if (settings_state_cyclingnight_str == "")
        {
            message = "[ERROR] String is empty. Failed to read cycling night state settings.";
            print_message(message);
            return "";
        }
        else
        {
            statemachine_json = concatJson(statemachine_json, settings_state_cyclingnight_str);
        }
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

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    doc["statemachine_state"] = temp_state;
    serializeJson(doc, valvecontrol_json);

    return valvecontrol_json;
}
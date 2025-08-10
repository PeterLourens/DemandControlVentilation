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

String create_index_page_json()
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

    ///Read setting for valve and valve name
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

    // Bus0 sensors
    doc["bus0_sensor0_type"] = wire_sensor_data_temp["wire_sensor0"]["type"];

    // Bus1 sensors

    // convert to JSON string
    serializeJson(doc, json);
    print_message(json);

    return json;
}

String create_settings_json()
{
    String settings_json = "";
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

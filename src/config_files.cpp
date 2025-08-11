#include "config_files.h"

// Read time settings
void read_time_settings(void)
{

    bool settings_rtc_file_present = 0;
    const char *path = "/json/settings_rtc.json";

    String settings_rtc_string = "";
    String message = "";
    JsonDocument settings_rtc_doc;

    if (settings_rtc_mutex != NULL)
    {
        if (xSemaphoreTake(settings_rtc_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_rtc_file_present = check_file_exists(path);
            if (settings_rtc_file_present == 1)
            {
                settings_rtc_string = read_config_file(path);
            }
            xSemaphoreGive(settings_rtc_mutex);
        }
    }
    if (settings_rtc_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err = deserializeJson(settings_rtc_doc, settings_rtc_string);
        if (err)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path) + ": " + String(err.c_str());
            print_message(message);
            return;
        }
    }

    String ntp_server_tmp = settings_rtc_doc[String("ntp_server")];
    String timezone_tmp = settings_rtc_doc[String("timezone")];

    if (settings_rtc_mutex != NULL)
    {
        if (xSemaphoreTake(settings_rtc_mutex, (TickType_t)10) == pdTRUE)
        {
            ntp_server = ntp_server_tmp;
            timezone = timezone_tmp;
            xSemaphoreGive(settings_rtc_mutex);
        }
    }
}

// Read Influxdb config file and update global variables
void read_influxdb_config(void)
{

    bool settings_influxdb_file_present = 0;
    const char *path = "/json/settings_influxdb.json";

    String settings_influxdb_string = "";
    String message = "";
    JsonDocument settings_influxdb_doc;

    if (settings_influxdb_mutex != NULL)
    {
        if (xSemaphoreTake(settings_influxdb_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_influxdb_file_present = check_file_exists(path);
            if (settings_influxdb_file_present == 1)
            {
                settings_influxdb_string = read_config_file(path);
            }
            xSemaphoreGive(settings_influxdb_mutex);
        }
    }
    if (settings_influxdb_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err = deserializeJson(settings_influxdb_doc, settings_influxdb_string);
        if (err)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path) + ": " + String(err.c_str());
            print_message(message);
            return;
        }
    }

    String enable_influxdb_tmp = settings_influxdb_doc[String("enable_influxdb")];
    String influxdb_url_tmp = settings_influxdb_doc[String("influxdb_url")];
    String influxdb_org_tmp = settings_influxdb_doc[String("influxdb_org")];
    String influxdb_bucket_tmp = settings_influxdb_doc[String("influxdb_bucket")];
    String influxdb_token_tmp = settings_influxdb_doc[String("influxdb_token")];

    if (settings_influxdb_mutex != NULL)
    {
        if (xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
        {
            enable_influxdb = enable_influxdb_tmp;
            influxdb_url = influxdb_url_tmp;
            influxdb_org = influxdb_org_tmp;
            influxdb_bucket = influxdb_bucket_tmp;
            influxdb_token = influxdb_token_tmp;
            xSemaphoreGive(settings_influxdb_mutex);
        }
    }
}

// Read I2C hardware settings
void read_i2c_config(void)
{

    bool settings_i2c_file_present = 0;
    const char *path = "/json/settings_i2c.json";

    String settings_i2c_string = "";
    String message = "";
    JsonDocument settings_i2c_doc;

    if (settings_i2c_mutex != NULL)
    {
        if (xSemaphoreTake(settings_i2c_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_i2c_file_present = check_file_exists(path);
            if (settings_i2c_file_present == 1)
            {
                settings_i2c_string = read_config_file(path);
            }
            xSemaphoreGive(settings_i2c_mutex);
        }
    }
    if (settings_i2c_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err = deserializeJson(settings_i2c_doc, settings_i2c_string);
        if (err)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path) + ": " + String(err.c_str());
            print_message(message);
            return;
        }
    }

    int bus0_multiplexer_addr_tmp = settings_i2c_doc["bus0_multiplexer_address"];
    int bus1_multiplexer_addr_tmp = settings_i2c_doc["bus1_multiplexer_address"];
    int display_i2c_addr_tmp = settings_i2c_doc["display_i2c_address"];
    String enable_lcd_str = settings_i2c_doc[String("enable_lcd")];

    if (settings_i2c_mutex != NULL)
    {
        if (xSemaphoreTake(settings_i2c_mutex, (TickType_t)10) == pdTRUE)
        {
            bus0_multiplexer_addr = bus0_multiplexer_addr_tmp;
            bus1_multiplexer_addr = bus1_multiplexer_addr_tmp;
            enable_lcd = enable_lcd_str;
            display_i2c_addr = display_i2c_addr_tmp;
            xSemaphoreGive(settings_i2c_mutex);
        }
    }
}

// Read mqtt config file and update global variables
void read_mqtt_config(void)
{

    bool settings_mqtt_file_present = 0;
    const char *path = "/json/settings_mqtt.json";

    String settings_mqtt_string = "";
    String message = "";
    JsonDocument settings_mqtt_doc;

    if (settings_mqtt_mutex != NULL)
    {
        if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_mqtt_file_present = check_file_exists(path);
            if (settings_mqtt_file_present == 1)
            {
                settings_mqtt_string = read_config_file(path);
            }
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }
    if (settings_mqtt_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err = deserializeJson(settings_mqtt_doc, settings_mqtt_string);
        if (err)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path) + ": " + String(err.c_str());
            print_message(message);
            return;
        }
    }

    String enable_mqtt_tmp = settings_mqtt_doc[String("enable_mqtt")];
    String mqtt_server_tmp = settings_mqtt_doc[String("mqtt_server")];
    String mqtt_port_tmp = settings_mqtt_doc[String("mqtt_port")];
    String mqtt_base_topic_tmp = settings_mqtt_doc[String("mqtt_base_topic")];

    if (settings_mqtt_mutex != NULL)
    {
        if (xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10) == pdTRUE)
        {
            enable_mqtt = enable_mqtt_tmp;
            mqtt_server = mqtt_server_tmp;
            mqtt_port = mqtt_port_tmp.toInt();
            mqtt_base_topic = mqtt_base_topic_tmp;
            xSemaphoreGive(settings_mqtt_mutex);
        }
    }
}

void read_fan_config()
{

    bool settings_fan_file_present = 0;
    const char *path = "/json/settings_fan.json";

    String settings_fan_string = "";
    String message = "";
    JsonDocument settings_fan_doc;

    if (settings_fan_mutex != NULL)
    {
        if (xSemaphoreTake(settings_fan_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_fan_file_present = check_file_exists(path);
            if (settings_fan_file_present == 1)
            {
                settings_fan_string = read_config_file(path);
            }
            xSemaphoreGive(settings_fan_mutex);
        }
    }
    if (settings_fan_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err = deserializeJson(settings_fan_data, settings_fan_string);
        if (err)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path) + ": " + String(err.c_str());
            print_message(message);
            return;
        }
    }
}

// Read both sensor config files an place contents in global variable
void sensor_config_data_read()
{

    bool sensor_config1_file_present = 0;
    bool sensor_config2_file_present = 0;

    const char *path1 = "/json/sensor_config1.json";
    const char *path2 = "/json/sensor_config2.json";

    String sensor_config1_string = "";
    String sensor_config2_string = "";
    String message = "";

    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            sensor_config1_file_present = check_file_exists(path1);
            if (sensor_config1_file_present == 1)
            {
                sensor_config1_string = read_config_file(path1);
            }
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }
    if (sensor_config1_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err1 = deserializeJson(wire_sensor_data, sensor_config1_string);
        if (err1)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path1) + ": " + String(err1.c_str());
            print_message(message);
            return;
        }
    }

    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            sensor_config2_file_present = check_file_exists(path2);
            if (sensor_config2_file_present == 1)
            {
                sensor_config2_string = read_config_file(path2);
            }
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }
    if (sensor_config2_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err2 = deserializeJson(wire1_sensor_data, sensor_config2_string);
        if (err2)
        {
            message = "[ERROR] Failed to parse valvepositions.json: " + String(path2) + ": " + String(err2.c_str());
            print_message(message);
            return;
        }
    }
}

// Function to read settings (e.g. valve positions) for each state and put these in the global variable
void valve_settings_config_read()
{

    const char *settings_state_day_path = "/json/settings_state_day.json";
    const char *settings_state_night_path = "/json/settings_state_night.json";
    const char *settings_state_highco2day_path = "/json/settings_state_highco2day.json";
    const char *settings_state_highco2night_path = "/json/settings_state_highco2night.json";
    const char *settings_state_highrhday_path = "/json/settings_state_highrhday.json";
    const char *settings_state_highrhnight_path = "/json/settings_state_highrhnight.json";
    const char *settings_state_cooking_path = "/json/settings_state_cooking.json";
    const char *settings_state_cyclingday_path = "/json/settings_state_cyclingday.json";
    const char *settings_state_cyclingnight_path = "/json/settings_state_cyclingnight.json";
    bool settings_state_day_present = 0;
    bool settings_state_night_present = 0;
    bool settings_state_highco2day_present = 0;
    bool settings_state_highco2night_present = 0;
    bool settings_state_highrhday_present = 0;
    bool settings_state_highrhnight_present = 0;
    bool settings_state_cooking_present = 0;
    bool settings_state_cyclingday_present = 0;
    bool settings_state_cyclingnight_present = 0;
    String settings_state_day_str = "";
    String settings_state_night_str = "";
    String settings_state_highco2day_str = "";
    String settings_state_highco2night_str = "";
    String settings_state_highrhday_str = "";
    String settings_state_highrhnight_str = "";
    String settings_state_cooking_str = "";
    String settings_state_cyclingday_str = "";
    String settings_state_cyclingnight_str = "";
    String message = "";

    if (settings_state_day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_day_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_day_present = check_file_exists(settings_state_day_path);
            if (settings_state_day_present == 1)
            {
                settings_state_day_str = read_config_file(settings_state_day_path);
                if (settings_state_day_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_day, settings_state_day_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_day_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_day_mutex);
        }
    }

    if (settings_state_night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_night_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_night_present = check_file_exists(settings_state_night_path);
            if (settings_state_night_present == 1)
            {
                settings_state_night_str = read_config_file(settings_state_night_path);
                if (settings_state_night_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_night, settings_state_night_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_night_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_night_mutex);
        }
    }

    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highco2day_present = check_file_exists(settings_state_highco2day_path);
            if (settings_state_highco2day_present == 1)
            {
                settings_state_highco2day_str = read_config_file(settings_state_highco2day_path);
                if (settings_state_highco2day_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_highco2day, settings_state_highco2day_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_highco2day_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }

    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highco2night_present = check_file_exists(settings_state_highco2night_path);
            if (settings_state_highco2night_present == 1)
            {
                settings_state_highco2night_str = read_config_file(settings_state_highco2night_path);
                if (settings_state_highco2night_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_highco2night, settings_state_highco2night_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_highco2night_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }

    if (settings_state_highrhday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highrhday_present = check_file_exists(settings_state_highrhday_path);
            if (settings_state_highrhday_present == 1)
            {
                settings_state_highrhday_str = read_config_file(settings_state_highrhday_path);
                if (settings_state_highrhday_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_highrhday, settings_state_highrhday_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_highrhday_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_highrhday_mutex);
        }
    }

    if (settings_state_highrhnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_highrhnight_present = check_file_exists(settings_state_highrhnight_path);
            if (settings_state_highrhnight_present == 1)
            {
                settings_state_highrhnight_str = read_config_file(settings_state_highrhnight_path);
                if (settings_state_highrhnight_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_highrhnight, settings_state_highrhnight_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_highrhnight_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_highrhnight_mutex);
        }
    }

    if (settings_state_cooking_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_cooking_present = check_file_exists(settings_state_cooking_path);
            if (settings_state_cooking_present == 1)
            {
                settings_state_cooking_str = read_config_file(settings_state_cooking_path);
                if (settings_state_cooking_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_cooking, settings_state_cooking_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_cooking_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_cooking_mutex);
        }
    }

    if (settings_state_cyclingday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_cyclingday_present = check_file_exists(settings_state_cyclingday_path);
            if (settings_state_cyclingday_present == 1)
            {
                settings_state_cyclingday_str = read_config_file(settings_state_cyclingday_path);
                if (settings_state_cooking_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_cyclingday, settings_state_cyclingday_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_cyclingday_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_cyclingday_mutex);
        }
    }

    if (settings_state_cyclingnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)100) == pdTRUE)
        {
            settings_state_cyclingnight_present = check_file_exists(settings_state_cyclingnight_path);
            if (settings_state_cyclingnight_present == 1)
            {
                settings_state_cyclingnight_str = read_config_file(settings_state_cyclingnight_path);
                if (settings_state_cyclingnight_str == "")
                {
                    message = "[ERROR] String is empty or failed to read file";
                    print_message(message);
                    return;
                }
                else
                {
                    DeserializationError err = deserializeJson(settings_state_cyclingnight, settings_state_cyclingnight_str);
                    if (err)
                    {
                        message = "[ERROR] Failed to parse valvepositions.json: " + String(settings_state_cyclingnight_path) + ": " + String(err.c_str());
                        print_message(message);
                        return;
                    }
                }
            }
            xSemaphoreGive(settings_state_cyclingnight_mutex);
        }
    }
}

// Write valve status file with all valve positions 0
void valve_status_file_create()
{

    const char *default_valve_position_file;
    File file;
    String message = "";

    default_valve_position_file = "{\"valve0\":0, \"valve1\":0, \"valve2\":0, \"valve3\":0, \"valve4\":0, \"valve5\":0, \"valve6\":0, \"valve7\":0, \"valve8\":0, \"valve9\":0, \"valve10\":0, \"valve11\":0}";

    if (valve_position_file_mutex != NULL)
    {
        if (xSemaphoreTake(valve_position_file_mutex, (TickType_t)100) == pdTRUE)
        {
            file = LittleFS.open("/json/valvepositions.json", "w");
            if (!file)
            {
                message = "[ERROR] Failed to open file for writing: /json/valvepositions.json";
                print_message(message);
                xSemaphoreGive(valve_position_file_mutex);
                return;
            }
            if (file.print(default_valve_position_file) == 0)
            {
                message = "[ERROR] Write failed: /json/valvepositions.json";
                print_message(message);
                return;
            }
            else
            {
                message = "Valve status file written";
                print_message(message);
            }
            file.close();
            xSemaphoreGive(valve_position_file_mutex);
        }
    }
}

// Delete file with path as input variable
void delete_file(const char *path)
{

    String message = "";

    if (path == NULL)
    {
        message = "[ERROR] Path is NULL, cannot delete file";
        print_message(message);
        return;
    }
    else if (LittleFS.remove(path))
    {
        message = "[INF] File deleted: " + String(path);
        print_message(message);
    }
    else
    {
        message = "[ERROR] Delete failed: " + String(path);
        print_message(message);
    }
}

bool check_file_exists(const char *path)
{

    if (LittleFS.exists(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Functions read config file from file with file path as input and returns the contents of the file as a string.
// Assumes presents of file was checked before calling this function.
String read_config_file(const char *path)
{

    String string_from_file;
    String message = "";

    if (path == NULL)
    {
        message = "[ERROR] Path is NULL, cannot read file";
        print_message(message);
        return "";
    }

    File file = LittleFS.open(path, "r");

    if (!file)
    {
        message = "[ERROR] Failed to open file for reading: " + String(path);
        print_message(message);
        return "";
    }
    if (file.size() == 0)
    {
        message = "[ERROR] File is empty: " + String(path);
        print_message(message);
        file.close();
        return "";
    }

    string_from_file = file.readString();
    file.close();
    return string_from_file;
}

// Write string to file, path and contents as string as parameters
bool write_config_file(const char *path, String file_contents)
{

    String message = "";

    if (path == NULL)
    {
        message = "[ERROR] Path is NULL, cannot write file";
        print_message(message);
        return false;
    }

    File file = LittleFS.open(path, "w");

    if (!file)
    {
        message = "[ERROR] Failed to open file for writing: " + String(path);
        print_message(message);
        return false;
    }

    bool ok = file.print(file_contents) != 0;
    file.close();

    if (!ok)
    {
        message = "[ERROR] Failed to write to file: " + String(path);
        print_message(message);
        return false;
    }
    else
    {
        message = "Config file written: " + String(path);
        print_message(message);
        return true;
    }
}

#include "config_files.h"

// Write string to file, path and contents as string as parameters
bool write_settings(const char *path, char *file_contents, SemaphoreHandle_t mutex)
{
    bool ok = false;
    char msg[MSG_SIZE] = {};

    if (path == NULL)
    {
        snprintf(msg, sizeof(msg), "[ERROR] Path is NULL, cannot write file");
        printmessage(msg);
        return false;
    }

    if (mutex && xSemaphoreTake(mutex, (TickType_t)50))
    {
        File file = LittleFS.open(path, "w");

        if (!file)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to open file for writing with path %s", path);
            printmessage(msg);
            xSemaphoreGive(mutex);
            return false;
        }
        ok = file.print(file_contents) != 0;
        file.close();
        xSemaphoreGive(mutex);
    }
    else
    {
        snprintf(msg, sizeof(msg), "[ERROR] Mutex is NULL, cannot write file with path %s", path);
        printmessage(msg);
        return false;
    }

    if (!ok)
    {
        snprintf(msg, sizeof(msg), "[ERROR] Failed to write file with path %s", path);
        printmessage(msg);
        return false;
    }
    else
    {
        snprintf(msg, sizeof(msg), "[INFO] Config file written with path %s", path);
        printmessage(msg);
        return true;
    }
    return false;
}

bool read_settings(const char *path, char *buffer, size_t bufferSize, SemaphoreHandle_t mutex)
{
    char msg[MSG_SIZE] = {};

    if (mutex && xSemaphoreTake(mutex, (TickType_t)50))
    {
        File file = LittleFS.open(path, "r");
        if (!file)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to open %s", path);
            printmessage(msg);
            xSemaphoreGive(mutex);
            return false;
        }
        size_t len = file.readBytes(buffer, bufferSize - 1);
        buffer[len] = '\0'; // Null-terminate
        file.close();
        xSemaphoreGive(mutex);
        return true;
    }
    snprintf(msg, sizeof(msg), "[ERROR] Failed to  acquire file %s", mutex);
    printmessage(msg);
    return false;
}

bool parse_network_settings(void)
{
    char buffer[512];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_NETWORK_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            // message = "[ERROR] Failed to parse: " + String(SETTINGS_NETWORK_PATH) + " with error: " + String(error.c_str());
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_NETWORK_PATH, error);
            printmessage(msg);
            return false;
        }

        const char *enable_dhcp = doc["enable_dhcp"];
        const char *ssid = doc["ssid"];
        const char *wifi_password = doc["wifi_password"];
        const char *ip_address = doc["ip_address"];
        const char *subnet_mask = doc["subnet_mask"];
        const char *gateway = doc["gateway"];
        const char *primary_dns = doc["primary_dns"];
        const char *secondary_dns = doc["secondary_dns"];

        if (settings_network_mutex && xSemaphoreTake(settings_network_mutex, (TickType_t)10))
        {
            if (enable_dhcp)
            {
                strncpy(networksettings.enable_dhcp, enable_dhcp, sizeof(networksettings.enable_dhcp) - 1);
                networksettings.enable_dhcp[sizeof(networksettings.enable_dhcp) - 1] = '\0';
            }
            if (ssid)
            {
                strncpy(networksettings.ssid, ssid, sizeof(networksettings.ssid) - 1);
                networksettings.ssid[sizeof(networksettings.ssid) - 1] = '\0';
            }
            if (wifi_password)
            {
                strncpy(networksettings.wifi_password, wifi_password, sizeof(networksettings.wifi_password) - 1);
                networksettings.wifi_password[sizeof(networksettings.wifi_password) - 1] = '\0';
            }
            if (subnet_mask)
            {
                strncpy(networksettings.subnet_mask, subnet_mask, sizeof(networksettings.subnet_mask) - 1);
                networksettings.subnet_mask[sizeof(networksettings.subnet_mask) - 1] = '\0';
            }
            if (gateway)
            {
                strncpy(networksettings.gateway, gateway, sizeof(networksettings.gateway) - 1);
                networksettings.gateway[sizeof(networksettings.gateway) - 1] = '\0';
            }
            if (primary_dns)
            {
                strncpy(networksettings.primary_dns, primary_dns, sizeof(networksettings.primary_dns) - 1);
                networksettings.primary_dns[sizeof(networksettings.primary_dns) - 1] = '\0';
            }
            if (secondary_dns)
            {
                strncpy(networksettings.secondary_dns, secondary_dns, sizeof(networksettings.secondary_dns) - 1);
                networksettings.secondary_dns[sizeof(networksettings.secondary_dns) - 1] = '\0';
            }
            xSemaphoreGive(settings_network_mutex);
            return true;
        }
    }
    return false;
}

bool parse_rtc_settings(void)
{
    char buffer[128];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_RTC_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            // message = "[ERROR] Failed to parse: " + String(SETTINGS_RTC_PATH) + " with error: " + String(error.c_str());
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_RTC_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *ntp_server = doc["ntp_server"];
    const char *timezone = doc["timezone"];

    if (settings_rtc_mutex && xSemaphoreTake(settings_rtc_mutex, (TickType_t)10))
    {
        if (ntp_server)
        {
            strncpy(rtcsettings.ntp_server, ntp_server, sizeof(rtcsettings.ntp_server) - 1);
            rtcsettings.ntp_server[sizeof(rtcsettings.ntp_server) - 1] = '\0';
        }
        if (timezone)
        {
            strncpy(rtcsettings.timezone, timezone, sizeof(rtcsettings.timezone) - 1);
            rtcsettings.timezone[sizeof(rtcsettings.timezone) - 1] = '\0';
        }
        xSemaphoreGive(settings_rtc_mutex);
        return true;
    }
    return false;
}

bool parse_influxdb_settings(void)
{
    char buffer[512];
    char msg[MSG_SIZE];
    JsonDocument doc;

    if (read_settings(SETTINGS_INFLUXDB_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_INFLUXDB_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_influxdb = doc["enable_influxdb"];
    const char *influxdb_url = doc["influxdb_url"];
    const char *influxdb_org = doc["influxdb_org"];
    const char *influxdb_bucket = doc["influxdb_bucket"];
    const char *influxdb_token = doc["influxdb_token"];

    if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10))
    {
        if (enable_influxdb)
        {
            strncpy(influxdbsettings.enable_influxdb, enable_influxdb, sizeof(influxdbsettings.enable_influxdb) - 1);
            influxdbsettings.enable_influxdb[sizeof(influxdbsettings.enable_influxdb) - 1] = '\0';
        }
        if (influxdb_url)
        {
            strncpy(influxdbsettings.influxdb_url, influxdb_url, sizeof(influxdbsettings.influxdb_url) - 1);
            influxdbsettings.influxdb_url[sizeof(influxdbsettings.influxdb_url) - 1] = '\0';
        }
        if (influxdb_org)
        {
            strncpy(influxdbsettings.influxdb_org, influxdb_org, sizeof(influxdbsettings.influxdb_org) - 1);
            influxdbsettings.influxdb_org[sizeof(influxdbsettings.influxdb_org) - 1] = '\0';
        }
        if (influxdb_bucket)
        {
            strncpy(influxdbsettings.influxdb_bucket, influxdb_bucket, sizeof(influxdbsettings.influxdb_bucket) - 1);
            influxdbsettings.influxdb_bucket[sizeof(influxdbsettings.influxdb_bucket) - 1] = '\0';
        }
        if (influxdb_token)
        {
            strncpy(influxdbsettings.influxdb_token, influxdb_token, sizeof(influxdbsettings.influxdb_token) - 1);
            influxdbsettings.influxdb_token[sizeof(influxdbsettings.influxdb_token) - 1] = '\0';
        }
        xSemaphoreGive(settings_influxdb_mutex);
        return true;
    }
    return false;
}

bool parse_i2c_settings(void)
{
    char buffer[512];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_I2C_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_I2C_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_lcd = doc["enable_lcd"];

    if (settings_i2c_mutex && xSemaphoreTake(settings_i2c_mutex, (TickType_t)10))
    {
        i2csettings.bus0_multiplexer_address = doc["bus0_multiplexer_address"];
        i2csettings.bus1_multiplexer_address = doc["bus1_multiplexer_address"];
        i2csettings.display_i2c_address = doc["display_i2c_address"];
        if (enable_lcd)
        {
            strncpy(i2csettings.enable_lcd, enable_lcd, sizeof(i2csettings.enable_lcd) - 1);
            i2csettings.enable_lcd[sizeof(i2csettings.enable_lcd) - 1] = '\0';
        }
        xSemaphoreGive(settings_i2c_mutex);
        return true;
    }
    return false;
}

bool parse_mqtt_settings(void)
{
    char buffer[512];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_MQTT_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_MQTT_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_mqtt = doc["enable_mqtt"];
    const char *mqtt_server = doc["mqtt_server"];
    const char *mqtt_base_topic = doc["mqtt_base_topic"];

    if (settings_mqtt_mutex && xSemaphoreTake(settings_mqtt_mutex, (TickType_t)10))
    {
        mqttsettings.mqtt_port = doc["mqtt_port"];

        if (enable_mqtt)
        {
            strncpy(mqttsettings.enable_mqtt, enable_mqtt, sizeof(mqttsettings.enable_mqtt) - 1);
            mqttsettings.enable_mqtt[sizeof(mqttsettings.enable_mqtt) - 1] = '\0';
        }
        if (mqtt_server)
        {
            strncpy(mqttsettings.mqtt_server, mqtt_server, sizeof(mqttsettings.mqtt_server) - 1);
            mqttsettings.mqtt_server[sizeof(mqttsettings.mqtt_server) - 1] = '\0';
        }
        if (mqtt_base_topic)
        {
            strncpy(mqttsettings.mqtt_base_topic, mqtt_base_topic, sizeof(mqttsettings.mqtt_base_topic) - 1);
            mqttsettings.mqtt_base_topic[sizeof(mqttsettings.mqtt_base_topic) - 1] = '\0';
        }
        xSemaphoreGive(settings_mqtt_mutex);
        return true;
    }
    return false;
}

bool parse_fan_settings(void)
{
    char buffer[512];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_FAN_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_FAN_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *fan_control_mode = doc["fan_control_mode"];
    const char *fan_control_mqtt_server = doc["fan_control_mqtt_server"];
    const char *fan_control_mqtt_port = doc["fan_control_mqtt_port"];
    const char *fan_control_mqtt_topic = doc["fan_control_mqtt_topic"];
    const char *fan_control_url_high_speed = doc["fan_control_url_high_speed"];
    const char *fan_control_url_medium_speed = doc["fan_control_url_medium_speed"];
    const char *fan_control_url_low_speed = doc["fan_control_url_high_speed"];

    if (settings_fan_mutex && xSemaphoreTake(settings_fan_mutex, (TickType_t)10))
    {
        if (fan_control_mode)
        {
            strncpy(fansettings.fan_control_mode, fan_control_mode, sizeof(fansettings.fan_control_mode) - 1);
            fansettings.fan_control_mode[sizeof(fansettings.fan_control_mode) - 1] = '\0';
        }
        if (fan_control_mqtt_server)
        {
            strncpy(fansettings.fan_control_mqtt_server, fan_control_mqtt_server, sizeof(fansettings.fan_control_mqtt_server) - 1);
            fansettings.fan_control_mqtt_server[sizeof(fansettings.fan_control_mqtt_server) - 1] = '\0';
        }
        if (fan_control_mqtt_port)
        {
            strncpy(fansettings.fan_control_mqtt_port, fan_control_mqtt_port, sizeof(fansettings.fan_control_mqtt_port) - 1);
            fansettings.fan_control_mqtt_port[sizeof(fansettings.fan_control_mqtt_port) - 1] = '\0';
        }
        if (fan_control_mqtt_topic)
        {
            strncpy(fansettings.fan_control_mqtt_topic, fan_control_mqtt_topic, sizeof(fansettings.fan_control_mqtt_topic) - 1);
            fansettings.fan_control_mqtt_topic[sizeof(fansettings.fan_control_mqtt_topic) - 1] = '\0';
        }
        if (fan_control_url_high_speed)
        {
            strncpy(fansettings.fan_control_url_high_speed, fan_control_url_high_speed, sizeof(fansettings.fan_control_url_high_speed) - 1);
            fansettings.fan_control_url_high_speed[sizeof(fansettings.fan_control_url_high_speed) - 1] = '\0';
        }
        if (fan_control_url_medium_speed)
        {
            strncpy(fansettings.fan_control_url_medium_speed, fan_control_url_medium_speed, sizeof(fansettings.fan_control_url_medium_speed) - 1);
            fansettings.fan_control_url_medium_speed[sizeof(fansettings.fan_control_url_medium_speed) - 1] = '\0';
        }
        if (fan_control_url_low_speed)
        {
            strncpy(fansettings.fan_control_url_low_speed, fan_control_url_low_speed, sizeof(fansettings.fan_control_url_low_speed) - 1);
            fansettings.fan_control_url_low_speed[sizeof(fansettings.fan_control_url_low_speed) - 1] = '\0';
        }
        xSemaphoreGive(settings_fan_mutex);
        return true;
    }
    return false;
}

bool parse_statemachine_settings(void)
{
    char buffer[512];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATEMACHINE_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);

        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATEMACHINE_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *weekend_day_1 = doc["weekend_day_1"];
    const char *weekend_day_2 = doc["weekend_day_2"];

    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10))
    {
        statemachinesettings.weekday_day_hour_start = doc["weekday_day_hour_start"];
        statemachinesettings.weekday_day_minute_start = doc["weekday_day_minute_start"];
        statemachinesettings.weekday_night_hour_start = doc["weekday_night_hour_start"];
        statemachinesettings.weekday_night_minute_start = doc["weekday_night_minute_start"];
        statemachinesettings.weekend_day_hour_start = doc["weekend_day_hour_start"];
        statemachinesettings.weekend_day_minute_start = doc["weekend_day_minute_start"];
        statemachinesettings.weekend_night_hour_start = doc["weekend_night_hour_start"];
        statemachinesettings.weekend_night_minute_start = doc["weekend_night_minute_start"];
        statemachinesettings.minimum_state_time = doc["minimum_state_time"];

        if (weekend_day_1)
        {
            strncpy(statemachinesettings.weekend_day_1, weekend_day_1, sizeof(statemachinesettings.weekend_day_1) - 1);
            statemachinesettings.weekend_day_1[sizeof(statemachinesettings.weekend_day_1) - 1] = '\0';
        }
        if (weekend_day_2)
        {
            strncpy(statemachinesettings.weekend_day_2, weekend_day_2, sizeof(statemachinesettings.weekend_day_2) - 1);
            statemachinesettings.weekend_day_2[sizeof(statemachinesettings.weekend_day_2) - 1] = '\0';
        }
        xSemaphoreGive(settings_statemachine_mutex);
        return true;
    }
    return false;
}

bool parse_sensor1_settings(void)
{
    char buffer[1500];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SENSOR_CONFIG1_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SENSOR_CONFIG1_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    // Read all 7 sensors settings
    if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10))
    {
        for (int i = 0; i < 8; i++)
        {
            if (doc["wire_sensor" + String(i) + "_type"])
            {
                strncpy(sensor1settings[i].wire_sensor_type, doc["wire_sensor" + String(i) + "_type"], sizeof(sensor1settings[i].wire_sensor_type) - 1);
                sensor1settings[i].wire_sensor_type[sizeof(sensor1settings[i].wire_sensor_type) - 1] = '\0';
            }
            if (doc["wire_sensor" + String(i) + "_valve"])
            {
                strncpy(sensor1settings[i].wire_sensor_valve, doc["wire_sensor" + String(i) + "_valve"], sizeof(sensor1settings[i].wire_sensor_valve) - 1);
                sensor1settings[i].wire_sensor_valve[sizeof(sensor1settings[i].wire_sensor_valve) - 1] = '\0';
            }
            if (doc["wire_sensor" + String(i) + "_location"])
            {
                strncpy(sensor1settings[i].wire_sensor_location, doc["wire_sensor" + String(i) + "_location"], sizeof(sensor1settings[i].wire_sensor_location) - 1);
                sensor1settings[i].wire_sensor_location[sizeof(sensor1settings[i].wire_sensor_location) - 1] = '\0';
            }
            if (doc["wire_sensor" + String(i) + "_rh"])
            {
                strncpy(sensor1settings[i].wire_sensor_rh, doc["wire_sensor" + String(i) + "_rh"], sizeof(sensor1settings[i].wire_sensor_rh) - 1);
                sensor1settings[i].wire_sensor_rh[sizeof(sensor1settings[i].wire_sensor_rh) - 1] = '\0';
            }
            if (doc["wire_sensor" + String(i) + "_co2"])
            {
                strncpy(sensor1settings[i].wire_sensor_co2, doc["wire_sensor" + String(i) + "_co2"], sizeof(sensor1settings[i].wire_sensor_co2) - 1);
                sensor1settings[i].wire_sensor_co2[sizeof(sensor1settings[i].wire_sensor_co2) - 1] = '\0';
            }
        }
        xSemaphoreGive(settings_sensor1_mutex);
        return true;
    }
    return false;
}

bool parse_sensor2_settings(void)
{
    char buffer[1500];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SENSOR_CONFIG2_PATH, buffer, sizeof(buffer), settings_files_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SENSOR_CONFIG2_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    // Read all 7 sensors settings
    if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10))
    {
        for (int i = 0; i < 8; i++)
        {
            if (doc["wire1_sensor" + String(i) + "_type"])
            {
                strncpy(sensor2settings[i].wire1_sensor_type, doc["wire1_sensor" + String(i) + "_type"], sizeof(sensor2settings[i].wire1_sensor_type) - 1);
                sensor2settings[i].wire1_sensor_type[sizeof(sensor2settings[i].wire1_sensor_type) - 1] = '\0';
            }
            if (doc["wire1_sensor" + String(i) + "_valve"])
            {
                strncpy(sensor2settings[i].wire1_sensor_valve, doc["wire1_sensor" + String(i) + "_valve"], sizeof(sensor2settings[i].wire1_sensor_valve) - 1);
                sensor2settings[i].wire1_sensor_valve[sizeof(sensor2settings[i].wire1_sensor_valve) - 1] = '\0';
            }
            if (doc["wire1_sensor" + String(i) + "_location"])
            {
                strncpy(sensor2settings[i].wire1_sensor_location, doc["wire1_sensor" + String(i) + "_location"], sizeof(sensor2settings[i].wire1_sensor_location) - 1);
                sensor2settings[i].wire1_sensor_location[sizeof(sensor2settings[i].wire1_sensor_location) - 1] = '\0';
            }
            if (doc["wire1_sensor" + String(i) + "_rh"])
            {
                strncpy(sensor2settings[i].wire1_sensor_rh, doc["wire1_sensor" + String(i) + "_rh"], sizeof(sensor2settings[i].wire1_sensor_rh) - 1);
                sensor2settings[i].wire1_sensor_rh[sizeof(sensor2settings[i].wire1_sensor_rh) - 1] = '\0';
            }
            if (doc["wire1_sensor" + String(i) + "_co2"])
            {
                strncpy(sensor2settings[i].wire1_sensor_co2, doc["wire1_sensor" + String(i) + "_co2"], sizeof(sensor2settings[i].wire1_sensor_co2) - 1);
                sensor2settings[i].wire1_sensor_co2[sizeof(sensor2settings[i].wire1_sensor_co2) - 1] = '\0';
            }
        }
        xSemaphoreGive(settings_sensor2_mutex);
        return true;
    }
    return false;
}

bool parse_state_day_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_DAY_PATH, buffer, sizeof(buffer), settings_state_day_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_DAY_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_day = doc["enable_state_day"];
    const char *state_day_fanspeed = doc["state_day_fanspeed"];
    const char *name_state_day = doc["name_state_day"];

    if (settings_state_day_mutex && (xSemaphoreTake(settings_state_day_mutex, (TickType_t)10)) == pdTRUE)
    {
        statedaysettings.state_day_highco2 = doc["state_day_highco2"];
        statedaysettings.state_day_highrh = doc["state_day_highrh"];

        for (int i = 0; i < 12; i++)
        {
            statedaysettings.valve_position_day[i] = doc["valve" + String(i) + "_position_day"];
        }

        if (enable_state_day)
        {
            strncpy(statedaysettings.enable_state_day, enable_state_day, sizeof(statedaysettings.enable_state_day) - 1);
            statedaysettings.enable_state_day[sizeof(statedaysettings.enable_state_day) - 1] = '\0';
        }
        if (state_day_fanspeed)
        {
            strncpy(statedaysettings.state_day_fanspeed, state_day_fanspeed, sizeof(statedaysettings.state_day_fanspeed) - 1);
            statedaysettings.state_day_fanspeed[sizeof(statedaysettings.state_day_fanspeed) - 1] = '\0';
        }
        if (name_state_day)
        {
            strncpy(statedaysettings.name_state_day, name_state_day, sizeof(statedaysettings.name_state_day) - 1);
            statedaysettings.name_state_day[sizeof(statedaysettings.name_state_day) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_day_mutex);
        return true;
    }
    return false;
}

bool parse_state_night_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_NIGHT_PATH, buffer, sizeof(buffer), settings_state_night_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_NIGHT_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_night = doc["enable_state_night"];
    const char *state_night_fanspeed = doc["state_night_fanspeed"];
    const char *name_state_night = doc["name_state_night"];

    if (settings_state_night_mutex && (xSemaphoreTake(settings_state_night_mutex, (TickType_t)10)) == pdTRUE)
    {
        statenightsettings.state_night_highco2 = doc["state_night_highco2"];
        statenightsettings.state_night_highrh = doc["state_night_highrh"];

        for (int i = 0; i < 12; i++)
        {
            statenightsettings.valve_position_night[i] = doc["valve" + String(i) + "_position_night"];
        }

        if (enable_state_night)
        {
            strncpy(statenightsettings.enable_state_night, enable_state_night, sizeof(statenightsettings.enable_state_night) - 1);
            statenightsettings.enable_state_night[sizeof(statenightsettings.enable_state_night) - 1] = '\0';
        }
        if (state_night_fanspeed)
        {
            strncpy(statenightsettings.state_night_fanspeed, state_night_fanspeed, sizeof(statenightsettings.state_night_fanspeed) - 1);
            statenightsettings.state_night_fanspeed[sizeof(statenightsettings.state_night_fanspeed) - 1] = '\0';
        }
        if (name_state_night)
        {
            strncpy(statenightsettings.name_state_night, name_state_night, sizeof(statenightsettings.name_state_night) - 1);
            statenightsettings.name_state_night[sizeof(statenightsettings.name_state_night) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_night_mutex);
        return true;
    }
    return false;
}

bool parse_state_highco2day_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_HIGHCO2DAY_PATH, buffer, sizeof(buffer), settings_state_highco2day_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_HIGHCO2DAY_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_highco2day = doc["enable_state_highco2day"];
    const char *state_highco2day_fanspeed = doc["state_highco2day_fanspeed"];
    const char *name_state_highco2day = doc["name_state_highco2day"];

    if (settings_state_highco2day_mutex && (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)10)) == pdTRUE)
    {
        statehighco2daysettings.co2_high_state_highco2day = doc["co2_high_state_highco2day"];
        statehighco2daysettings.co2_low_state_highco2day = doc["co2_low_state_highco2day"];

        for (int i = 0; i < 12; i++)
        {
            statehighco2daysettings.valve_position_highco2day[i] = doc["valve" + String(i) + "_position_highco2day"];
        }

        if (enable_state_highco2day)
        {
            strncpy(statehighco2daysettings.enable_state_highco2day, enable_state_highco2day, sizeof(statehighco2daysettings.enable_state_highco2day) - 1);
            statehighco2daysettings.enable_state_highco2day[sizeof(statehighco2daysettings.enable_state_highco2day) - 1] = '\0';
        }
        if (state_highco2day_fanspeed)
        {
            strncpy(statehighco2daysettings.state_highco2day_fanspeed, state_highco2day_fanspeed, sizeof(statehighco2daysettings.state_highco2day_fanspeed) - 1);
            statehighco2daysettings.state_highco2day_fanspeed[sizeof(statehighco2daysettings.state_highco2day_fanspeed) - 1] = '\0';
        }
        if (name_state_highco2day)
        {
            strncpy(statehighco2daysettings.name_state_highco2day, name_state_highco2day, sizeof(statehighco2daysettings.name_state_highco2day) - 1);
            statehighco2daysettings.name_state_highco2day[sizeof(statehighco2daysettings.name_state_highco2day) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_highco2day_mutex);
        return true;
    }
    return false;
}

bool parse_state_highco2night_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_HIGHCO2NIGHT_PATH, buffer, sizeof(buffer), settings_state_highco2night_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_HIGHCO2NIGHT_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_highco2night = doc["enable_state_highco2night"];
    const char *state_highco2night_fanspeed = doc["state_highco2night_fanspeed"];
    const char *name_state_highco2night = doc["name_state_highco2night"];

    if (settings_state_highco2night_mutex && (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10)) == pdTRUE)
    {
        statehighco2nightsettings.co2_high_state_highco2night = doc["co2_high_state_highco2night"];
        statehighco2nightsettings.co2_low_state_highco2night = doc["co2_low_state_highco2night"];

        for (int i = 0; i < 12; i++)
        {
            statehighco2nightsettings.valve_position_highco2night[i] = doc["valve" + String(i) + "_position_highco2night"];
        }

        if (enable_state_highco2night)
        {
            strncpy(statehighco2nightsettings.enable_state_highco2night, enable_state_highco2night, sizeof(statehighco2nightsettings.enable_state_highco2night) - 1);
            statehighco2nightsettings.enable_state_highco2night[sizeof(statehighco2nightsettings.enable_state_highco2night) - 1] = '\0';
        }
        if (state_highco2night_fanspeed)
        {
            strncpy(statehighco2nightsettings.state_highco2night_fanspeed, state_highco2night_fanspeed, sizeof(statehighco2nightsettings.state_highco2night_fanspeed) - 1);
            statehighco2nightsettings.state_highco2night_fanspeed[sizeof(statehighco2nightsettings.state_highco2night_fanspeed) - 1] = '\0';
        }
        if (name_state_highco2night)
        {
            strncpy(statehighco2nightsettings.name_state_highco2night, name_state_highco2night, sizeof(statehighco2nightsettings.name_state_highco2night) - 1);
            statehighco2nightsettings.name_state_highco2night[sizeof(statehighco2nightsettings.name_state_highco2night) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_highco2night_mutex);
        return true;
    }
    return false;
}

bool parse_state_highrhday_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_HIGHRHDAY_PATH, buffer, sizeof(buffer), settings_state_highrhday_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_HIGHRHDAY_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_highrhday = doc["enable_state_highrhday"];
    const char *state_highrhday_fanspeed = doc["state_highrhday_fanspeed"];
    const char *name_state_highrhday = doc["name_state_highrhday"];

    if (settings_state_highrhday_mutex && (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)10)) == pdTRUE)
    {
        statehighrhdaysettings.maximum_state_time_highrhday = doc["maximum_state_time_highrhday"];
        statehighrhdaysettings.rh_low_state_highrhday = doc["rh_low_state_highrhday"];

        for (int i = 0; i < 12; i++)
        {
            statehighrhdaysettings.valve_position_highrhday[i] = doc["valve" + String(i) + "_position_highrhday"];
        }

        if (enable_state_highrhday)
        {
            strncpy(statehighrhdaysettings.enable_state_highrhday, enable_state_highrhday, sizeof(statehighrhdaysettings.enable_state_highrhday) - 1);
            statehighrhdaysettings.enable_state_highrhday[sizeof(statehighrhdaysettings.enable_state_highrhday) - 1] = '\0';
        }
        if (state_highrhday_fanspeed)
        {
            strncpy(statehighrhdaysettings.state_highrhday_fanspeed, state_highrhday_fanspeed, sizeof(statehighrhdaysettings.state_highrhday_fanspeed) - 1);
            statehighrhdaysettings.state_highrhday_fanspeed[sizeof(statehighrhdaysettings.state_highrhday_fanspeed) - 1] = '\0';
        }
        if (name_state_highrhday)
        {
            strncpy(statehighrhdaysettings.name_state_highrhday, name_state_highrhday, sizeof(statehighrhdaysettings.name_state_highrhday) - 1);
            statehighrhdaysettings.name_state_highrhday[sizeof(statehighrhdaysettings.name_state_highrhday) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_highrhday_mutex);
        return true;
    }
    return false;
}

bool parse_state_highrhnight_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_HIGHRHNIGHT_PATH, buffer, sizeof(buffer), settings_state_highrhnight_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_HIGHRHNIGHT_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_highrhnight = doc["enable_state_highrhnight"];
    const char *state_highrhnight_fanspeed = doc["state_highrhnight_fanspeed"];
    const char *name_state_highrhnight = doc["name_state_highrhnight"];

    if (settings_state_highrhnight_mutex && (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)10)) == pdTRUE)
    {
        statehighrhnightsettings.maximum_state_time_highrhnight = doc["maximum_state_time_highrhnight"];
        statehighrhnightsettings.rh_low_state_highrhnight = doc["rh_low_state_highrhnight"];

        for (int i = 0; i < 12; i++)
        {
            statehighrhnightsettings.valve_position_highrhnight[i] = doc["valve" + String(i) + "_position_highrhnight"];
        }

        if (enable_state_highrhnight)
        {
            strncpy(statehighrhnightsettings.enable_state_highrhnight, enable_state_highrhnight, sizeof(statehighrhnightsettings.enable_state_highrhnight) - 1);
            statehighrhnightsettings.enable_state_highrhnight[sizeof(statehighrhnightsettings.enable_state_highrhnight) - 1] = '\0';
        }
        if (state_highrhnight_fanspeed)
        {
            strncpy(statehighrhnightsettings.state_highrhnight_fanspeed, state_highrhnight_fanspeed, sizeof(statehighrhnightsettings.state_highrhnight_fanspeed) - 1);
            statehighrhnightsettings.state_highrhnight_fanspeed[sizeof(statehighrhnightsettings.state_highrhnight_fanspeed) - 1] = '\0';
        }
        if (name_state_highrhnight)
        {
            strncpy(statehighrhnightsettings.name_state_highrhnight, name_state_highrhnight, sizeof(statehighrhnightsettings.name_state_highrhnight) - 1);
            statehighrhnightsettings.name_state_highrhnight[sizeof(statehighrhnightsettings.name_state_highrhnight) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_highrhnight_mutex);
        return true;
    }
    return false;
}

bool parse_state_cooking_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_COOKING_PATH, buffer, sizeof(buffer), settings_state_cooking_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_COOKING_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_cooking = doc["enable_state_cooking"];
    const char *state_cooking_fanspeed = doc["state_cooking_fanspeed"];
    const char *name_state_cooking = doc["name_state_cooking"];

    if (settings_state_cooking_mutex && (xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)10)) == pdTRUE)
    {
        statecookingsettings.start_hour_state_cooking = doc["start_hour_state_cooking"];
        statecookingsettings.start_minute_state_cooking = doc["start_minute_state_cooking"];
        statecookingsettings.stop_hour_state_cooking = doc["stop_hour_state_cooking"];
        statecookingsettings.stop_minute_state_cooking = doc["stop_minute_state_cooking"];

        for (int i = 0; i < 12; i++)
        {
            statecookingsettings.valve_position_cooking[i] = doc["valve" + String(i) + "_position_cooking"];
        }

        if (enable_state_cooking)
        {
            strncpy(statecookingsettings.enable_state_cooking, enable_state_cooking, sizeof(statecookingsettings.enable_state_cooking) - 1);
            statecookingsettings.enable_state_cooking[sizeof(statecookingsettings.enable_state_cooking) - 1] = '\0';
        }
        if (state_cooking_fanspeed)
        {
            strncpy(statecookingsettings.state_cooking_fanspeed, state_cooking_fanspeed, sizeof(statecookingsettings.state_cooking_fanspeed) - 1);
            statecookingsettings.state_cooking_fanspeed[sizeof(statecookingsettings.state_cooking_fanspeed) - 1] = '\0';
        }
        if (name_state_cooking)
        {
            strncpy(statecookingsettings.name_state_cooking, name_state_cooking, sizeof(statecookingsettings.name_state_cooking) - 1);
            statecookingsettings.name_state_cooking[sizeof(statecookingsettings.name_state_cooking) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_cooking_mutex);
        return true;
    }
    return false;
}

bool parse_state_cyclingday_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_CYCLINGDAY_PATH, buffer, sizeof(buffer), settings_state_cyclingday_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_CYCLINGDAY_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_cyclingday = doc["enable_state_cyclingday"];
    const char *state_cyclingday_fanspeed = doc["state_cyclingday_fanspeed"];
    const char *name_state_cyclingday = doc["name_state_cyclingday"];

    if (settings_state_cyclingday_mutex && (xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)10)) == pdTRUE)
    {
        for (int i = 0; i < 12; i++)
        {
            statecyclingdaysettings.valve_position_cyclingday[i] = doc["valve" + String(i) + "_position_cyclingday"];
        }

        if (enable_state_cyclingday)
        {
            strncpy(statecyclingdaysettings.enable_state_cyclingday, enable_state_cyclingday, sizeof(statecyclingdaysettings.enable_state_cyclingday) - 1);
            statecyclingdaysettings.enable_state_cyclingday[sizeof(statecyclingdaysettings.enable_state_cyclingday) - 1] = '\0';
        }
        if (state_cyclingday_fanspeed)
        {
            strncpy(statecyclingdaysettings.state_cyclingday_fanspeed, state_cyclingday_fanspeed, sizeof(statecyclingdaysettings.state_cyclingday_fanspeed) - 1);
            statecyclingdaysettings.state_cyclingday_fanspeed[sizeof(statecyclingdaysettings.state_cyclingday_fanspeed) - 1] = '\0';
        }
        if (name_state_cyclingday)
        {
            strncpy(statecyclingdaysettings.name_state_cyclingday, name_state_cyclingday, sizeof(statecyclingdaysettings.name_state_cyclingday) - 1);
            statecyclingdaysettings.name_state_cyclingday[sizeof(statecyclingdaysettings.name_state_cyclingday) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_cyclingday_mutex);
        return true;
    }
    return false;
}

bool parse_state_cyclingnight_settings(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(SETTINGS_STATE_CYCLINGNIGHT_PATH, buffer, sizeof(buffer), settings_state_cyclingnight_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", SETTINGS_STATE_CYCLINGNIGHT_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    const char *enable_state_cyclingnight = doc["enable_state_cyclingnight"];
    const char *state_cyclingnight_fanspeed = doc["state_cyclingnight_fanspeed"];
    const char *name_state_cyclingnight = doc["name_state_cyclingnight"];

    if (settings_state_cyclingnight_mutex && (xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)10)) == pdTRUE)
    {
        for (int i = 0; i < 12; i++)
        {
            statecyclingnightsettings.valve_position_cyclingnight[i] = doc["valve" + String(i) + "_position_cyclingnight"];
        }

        if (enable_state_cyclingnight)
        {
            strncpy(statecyclingnightsettings.enable_state_cyclingnight, enable_state_cyclingnight, sizeof(statecyclingnightsettings.enable_state_cyclingnight) - 1);
            statecyclingnightsettings.enable_state_cyclingnight[sizeof(statecyclingnightsettings.enable_state_cyclingnight) - 1] = '\0';
        }
        if (state_cyclingnight_fanspeed)
        {
            strncpy(statecyclingnightsettings.state_cyclingnight_fanspeed, state_cyclingnight_fanspeed, sizeof(statecyclingnightsettings.state_cyclingnight_fanspeed) - 1);
            statecyclingnightsettings.state_cyclingnight_fanspeed[sizeof(statecyclingnightsettings.state_cyclingnight_fanspeed) - 1] = '\0';
        }
        if (name_state_cyclingnight)
        {
            strncpy(statecyclingnightsettings.name_state_cyclingnight, name_state_cyclingnight, sizeof(statecyclingnightsettings.name_state_cyclingnight) - 1);
            statecyclingnightsettings.name_state_cyclingnight[sizeof(statecyclingnightsettings.name_state_cyclingnight) - 1] = '\0';
        }
        xSemaphoreGive(settings_state_cyclingnight_mutex);
        return true;
    }
    return false;
}

bool parse_actual_valve_positions(void)
{
    char buffer[700];
    char msg[MSG_SIZE] = {};
    JsonDocument doc;

    if (read_settings(VALVE_POSITIONS_PATH, buffer, sizeof(buffer), valve_position_file_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error %s", VALVE_POSITIONS_PATH, error);
            printmessage(msg);
            return false;
        }
    }

    if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            valvecontroldata.actual_valve_position[i] = doc["valve" + String(i)];
        }
        xSemaphoreGive(valve_control_data_mutex);
    }
    return true;
}

// Write valve status file with all valve positions 0
void valve_status_file_create()
{
    const char *default_valve_position_file;
    char msg[MSG_SIZE] = {};
    File file;

    default_valve_position_file = "{\"valve0\":0, \"valve1\":0, \"valve2\":0, \"valve3\":0, \"valve4\":0, \"valve5\":0, \"valve6\":0, \"valve7\":0, \"valve8\":0, \"valve9\":0, \"valve10\":0, \"valve11\":0}";

    if (valve_position_file_mutex && xSemaphoreTake(valve_position_file_mutex, (TickType_t)100) == pdTRUE)
    {
        file = LittleFS.open(VALVE_POSITIONS_PATH, "w");
        if (!file)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to open file for writing for %s", VALVE_POSITIONS_PATH);
            printmessage(msg);
            xSemaphoreGive(valve_position_file_mutex);
            return;
        }
        if (file.print(default_valve_position_file) == 0)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Write failed for %s", VALVE_POSITIONS_PATH);
            printmessage(msg);
            return;
        }
        else
        {
            snprintf(msg, sizeof(msg), "[INFO] Valve status file written");
            printmessage(msg);
        }
        file.close();
        xSemaphoreGive(valve_position_file_mutex);
    }
}

// Delete file with path as input variable
void delete_file(const char *path)
{
    char msg[MSG_SIZE] = {};

    if (path == NULL)
    {
        snprintf(msg, sizeof(msg), "[ERROR] Path is NULL, cannot delete file");
        printmessage(msg);
        return;
    }
    else if (LittleFS.remove(path))
    {
        snprintf(msg, sizeof(msg), "[INFO] File deleted for %s", path);
        printmessage(msg);
    }
    else
    {
        snprintf(msg, sizeof(msg), "[ERROR] Delete failed for %s", path);
        printmessage(msg);
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

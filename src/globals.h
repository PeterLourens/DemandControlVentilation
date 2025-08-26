#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <RTClib.h>

#define I2C_SCL1 38 // Wire I/O settings (SDA1/SCL1) and
#define I2C_SDA1 39
#define I2C_SCL2 40 // Wire1 I/O settings (SDA2/SCL2)
#define I2C_SDA2 41

#define LCD_ROWS 4     // LCD display rows
#define LCD_COLUMNS 20 // LCD display columns
#define LCDADDR 0x27   // LCD I2C address

// File path definitions
#define SETTINGS_STATEMACHINE_PATH "/json/settings_statemachine.json"
#define SETTINGS_STATE_DAY_PATH "/json/settings_state_day.json"
#define SETTINGS_STATE_NIGHT_PATH "/json/settings_state_night.json"
#define SETTINGS_STATE_HIGHCO2DAY_PATH "/json/settings_state_highco2day.json"
#define SETTINGS_STATE_HIGHCO2NIGHT_PATH "/json/settings_state_highco2night.json"
#define SETTINGS_STATE_HIGHRHDAY_PATH "/json/settings_state_highrhday.json"
#define SETTINGS_STATE_HIGHRHNIGHT_PATH "/json/settings_state_highrhnight.json"
#define SETTINGS_STATE_COOKING_PATH "/json/settings_state_cooking.json"
#define SETTINGS_STATE_CYCLINGDAY_PATH "/json/settings_state_cyclingday.json"
#define SETTINGS_STATE_CYCLINGNIGHT_PATH "/json/settings_state_cyclingnight.json"

#define SETTINGS_NETWORK_PATH "/json/settings_network.json"
#define SETTINGS_RTC_PATH "/json/settings_rtc.json"
#define SETTINGS_INFLUDB_PATH "/json/settings_influxdb.json"
#define SETTINGS_I2C_PATH "/json/settings_i2c.json"
#define SETTINGS_MQTT_PATH "/json/settings_mqtt.json"
#define SETTINGS_FAN_PATH "/json/settings_fan.json"

#define SENSOR_CONFIG1_PATH "/json/sensor_config1.json"
#define SENSOR_CONFIG2_PATH "/json/sensor_config2.json"

#define VALVE_POSITIONS_PATH "/json/valvepositions.json"

#define SMALL_CONFIG_ITEM 8
#define MEDIUM_CONFIG_ITEM 16
#define LARGE_CONFIG_ITEM 36
#define XLARGE_CONFIG_ITEM 64
#define XXLARGE_CONFIG_ITEM 128

#define SENSOR_I2C_BUSSES 2  // Number of I2C busses
#define SENSOR_COUNT 8       // Number of sensors per I2C bus
#define SENSOR_DATA_FIELDS 3 // 0=Temp, 1=RH, 2=CO2

#define MAX_SENSORS 16 // Maximum number of sensors supported

extern const char *settings_state_day_path;
extern const char *settings_state_night_path;
extern const char *settings_state_highco2day_path;
extern const char *settings_state_highco2night_path;
extern const char *settings_state_highrhday_path;
extern const char *settings_state_highrhnight_path;
extern const char *settings_state_cooking_path;
extern const char *settings_state_cyclingday_path;
extern const char *settings_state_cyclingnight_path;
extern const char *sensor_config1_path;
extern const char *sensor_config2_path;
extern const char *valve_positions_path;

extern TaskHandle_t task_influxdb;
extern TaskHandle_t task_i2c;
extern TaskHandle_t task_mqtt;
extern TaskHandle_t task_np;
extern TaskHandle_t task_statemach;
extern TaskHandle_t task_sys;
extern TaskHandle_t task_valvectrl;
extern TaskHandle_t h_Task_web;
extern TaskHandle_t task_wifi;
extern TaskHandle_t task_espnow;
extern TaskHandle_t task_websocket;

extern SemaphoreHandle_t sensor_config_file_mutex;
extern SemaphoreHandle_t valve_position_file_mutex;
extern SemaphoreHandle_t settings_files_mutex;

extern SemaphoreHandle_t settings_sensor1_mutex;
extern SemaphoreHandle_t settings_sensor2_mutex;
extern SemaphoreHandle_t valve_control_data_mutex;
extern SemaphoreHandle_t settings_network_mutex;
extern SemaphoreHandle_t settings_mqtt_mutex;
extern SemaphoreHandle_t settings_i2c_mutex;
extern SemaphoreHandle_t settings_fan_mutex;
extern SemaphoreHandle_t settings_statemachine_mutex;
extern SemaphoreHandle_t settings_influxdb_mutex;
extern SemaphoreHandle_t settings_rtc_mutex;

extern SemaphoreHandle_t settings_state_day_mutex;
extern SemaphoreHandle_t settings_state_night_mutex;
extern SemaphoreHandle_t settings_state_highco2day_mutex;
extern SemaphoreHandle_t settings_state_highco2night_mutex;
extern SemaphoreHandle_t settings_state_highrhday_mutex;
extern SemaphoreHandle_t settings_state_highrhnight_mutex;
extern SemaphoreHandle_t settings_state_cooking_mutex;
extern SemaphoreHandle_t settings_state_cyclingday_mutex;
extern SemaphoreHandle_t settings_state_cyclingnight_mutex;
extern SemaphoreHandle_t settings_state_temp_mutex;

extern SemaphoreHandle_t date_time_mutex;
extern SemaphoreHandle_t statemachine_state_mutex;
extern SemaphoreHandle_t fanspeed_mutex;
extern SemaphoreHandle_t lock_valve_move_mutex;
extern SemaphoreHandle_t ap_active_mutex;

extern QueueHandle_t sensor_queue;
extern QueueHandle_t sensor_avg_queue;
extern QueueHandle_t webserial_queue;

typedef struct
{
    char enable_dhcp[SMALL_CONFIG_ITEM];
    char ssid[SMALL_CONFIG_ITEM];
    char wifi_password[XLARGE_CONFIG_ITEM]; // WPA3 has max 63 characters
    char ip_address[LARGE_CONFIG_ITEM];
    char subnet_mask[LARGE_CONFIG_ITEM];
    char gateway[LARGE_CONFIG_ITEM];
    char primary_dns[LARGE_CONFIG_ITEM];
    char secondary_dns[LARGE_CONFIG_ITEM];
} Network_settings;

typedef struct
{
    char ntp_server[LARGE_CONFIG_ITEM];
    char timezone[LARGE_CONFIG_ITEM];
} RTC_settings;

typedef struct
{
    char enable_influxdb[SMALL_CONFIG_ITEM];
    char influxdb_url[XXLARGE_CONFIG_ITEM];
    char influxdb_org[LARGE_CONFIG_ITEM];
    char influxdb_bucket[LARGE_CONFIG_ITEM];
    char influxdb_token[XXLARGE_CONFIG_ITEM];
} Influxdb_settings;

typedef struct
{
    char bus0_multiplexer_address[SMALL_CONFIG_ITEM];
    char bus1_multiplexer_address[SMALL_CONFIG_ITEM];
    char enable_lcd[SMALL_CONFIG_ITEM];
    char display_i2c_address[SMALL_CONFIG_ITEM];
} I2C_settings;

typedef struct
{
    char enable_mqtt[SMALL_CONFIG_ITEM];
    char mqtt_server[XLARGE_CONFIG_ITEM];
    char mqtt_port[SMALL_CONFIG_ITEM];
    char mqtt_base_topic[LARGE_CONFIG_ITEM];
} MQTT_settings;

typedef struct
{
    char fan_control_mode[LARGE_CONFIG_ITEM];
    char fan_control_mqtt_server[LARGE_CONFIG_ITEM];
    char fan_control_mqtt_port[SMALL_CONFIG_ITEM];
    char fan_control_mqtt_topic[XLARGE_CONFIG_ITEM];
    char fan_control_url_high_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_medium_speed[XXLARGE_CONFIG_ITEM];
    char fan_control_url_low_speed[XXLARGE_CONFIG_ITEM];
} Fan_settings;

typedef struct
{
    int weekday_day_hour_start;
    int weekday_day_minute_start;
    int weekday_night_hour_start;
    int weekday_night_minute_start;
    int weekend_day_hour_start;
    int weekend_day_minute_start;
    int weekend_night_hour_start;
    int weekend_night_minute_start;
    char weekend_day_1[SMALL_CONFIG_ITEM];
    char weekend_day_2[SMALL_CONFIG_ITEM];
    int minimum_state_time;
} Statemachine_settings;

typedef struct
{
    char wire_sensor_type[SMALL_CONFIG_ITEM];
    char wire_sensor_valve[MEDIUM_CONFIG_ITEM];
    char wire_sensor_location[LARGE_CONFIG_ITEM];
    char wire_sensor_rh[SMALL_CONFIG_ITEM];
    char wire_sensor_co2[SMALL_CONFIG_ITEM];
} Sensor1_settings;

typedef struct
{
    char wire1_sensor_type[SMALL_CONFIG_ITEM];
    char wire1_sensor_valve[MEDIUM_CONFIG_ITEM];
    char wire1_sensor_location[LARGE_CONFIG_ITEM];
    char wire1_sensor_rh[SMALL_CONFIG_ITEM];
    char wire1_sensor_co2[SMALL_CONFIG_ITEM];
} Sensor2_settings;

extern Network_settings networksettings;
extern RTC_settings rtcsettings;
extern Influxdb_settings influxdbsettings;
extern I2C_settings i2csettings;
extern MQTT_settings mqttsettings;
extern Fan_settings fansettings;
extern Statemachine_settings statemachinesettings;
extern Sensor1_settings sensor1settings[SENSOR_COUNT];
extern Sensor2_settings sensor2settings[SENSOR_COUNT];

//
//
//
//
//
//
//
//
//
//
// Old style
extern JsonDocument valve_control_data;
// extern JsonDocument wire_sensor_data;
// extern JsonDocument wire1_sensor_data;
extern JsonDocument settings_fan_data;
//extern JsonDocument settings_statemachine_data;

extern JsonDocument settings_state_day;
extern JsonDocument settings_state_night;
extern JsonDocument settings_state_highco2day;
extern JsonDocument settings_state_highco2night;
extern JsonDocument settings_state_highrhday;
extern JsonDocument settings_state_highrhnight;
extern JsonDocument settings_state_cooking;
extern JsonDocument settings_state_cyclingday;
extern JsonDocument settings_state_cyclingnight;
extern JsonDocument settings_state_temp;

extern String wire_sensor_config_string;
extern String wire1_sensor_config_string;

// Statemachine variables
/*extern int weekday_day_hour_start;
extern int weekday_day_minute_start;
extern int weekday_night_hour_start;
extern int weekday_night_minute_start;
extern int weekend_day_hour_start;
extern int weekend_day_minute_start;
extern int weekend_night_hour_start;
extern int weekend_night_minute_start;
extern String weekend_day_1;
extern String weekend_day_2;
extern int minimum_state_time;*/

// MQTT settings
extern String enable_mqtt;
extern String mqtt_server;
extern int mqtt_port;
extern String mqtt_base_topic;

// InfluxDB settings
extern String enable_influxdb;
extern String influxdb_url;
extern String influxdb_org;
extern String influxdb_bucket;
extern String influxdb_token;

// Settings i2C
extern int bus0_multiplexer_addr;
extern int bus1_multiplexer_addr;
extern String enable_lcd;
extern int display_i2c_addr;

// Settings RTC
extern String ntp_server;
extern String timezone;

// Fan speed control settings
extern String fan_control_mode;
extern String fan_control_mqtt_server;
extern String fan_control_mqtt_port;
extern String fan_control_mqtt_topic;
extern String fan_control_url_high_speed;
extern String fan_control_url_medium_speed;
extern String fan_control_url_low_speed;

// Date time data from RTC
extern String yearStr;
extern String monthStr;
extern String dayStr;
extern String hourStr;
extern String minuteStr;
extern String secondStr;
extern String dayOfWeek;

extern String state;
extern String fanspeed;

extern bool lock_valve_move;
extern bool pb_toggle;
extern bool ap_active;
extern bool debug_mode;

// Data pins for 74HC595
extern int clockPin1;
extern int latchPin1;
extern int dataPin1;

// Data pins for 74HC595
extern int clockPin2;
extern int latchPin2;
extern int dataPin2;

// Function definitions

#endif
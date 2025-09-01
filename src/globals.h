#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <RTClib.h>

#define DEVICE_ESP32S3
// #define DEVICE_ESP32

#if defined(DEVICE_ESP32S3)

#define I2C_SCL1 38 // Wire I/O settings (SDA1/SCL1) and
#define I2C_SDA1 39
#define I2C_SCL2 40 // Wire1 I/O settings (SDA2/SCL2)
#define I2C_SDA2 41

#elif defined(DEVICE_ESP32)

#define I2C_SCL1 17 // Wire I/O settings (SDA1/SCL1) and
#define I2C_SDA1 16
#define I2C_SCL2 18 // Wire1 I/O settings (SDA2/SCL2)
#define I2C_SDA2 19

#else
#error "No device type defined! Please define DEVICE_ESP32S3 or DEVICE_ESP32."
#endif

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

#define MAX_SENSORS 16 // Maximum number of sensors supported by the hardware
#define MAX_VALVES 12  // Maximum number of valves

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
    int bus0_multiplexer_address;
    int bus1_multiplexer_address;
    char enable_lcd[SMALL_CONFIG_ITEM];
    int display_i2c_address;
} I2C_settings;

typedef struct
{
    char enable_mqtt[SMALL_CONFIG_ITEM];
    char mqtt_server[XLARGE_CONFIG_ITEM];
    int mqtt_port;
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
    char weekend_day_1[MEDIUM_CONFIG_ITEM];
    char weekend_day_2[MEDIUM_CONFIG_ITEM];
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

typedef struct
{
    char enable_state_day[SMALL_CONFIG_ITEM];
    char state_day_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_day[LARGE_CONFIG_ITEM];
    int state_day_highco2;
    int state_day_highrh;
    int valve0_position_day;
    int valve1_position_day;
    int valve2_position_day;
    int valve3_position_day;
    int valve4_position_day;
    int valve5_position_day;
    int valve6_position_day;
    int valve7_position_day;
    int valve8_position_day;
    int valve9_position_day;
    int valve10_position_day;
    int valve11_position_day;
} State_day_settings;

typedef struct
{
    char enable_state_night[SMALL_CONFIG_ITEM];
    char state_night_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_night[LARGE_CONFIG_ITEM];
    int state_night_highco2;
    int state_night_highrh;
    int valve0_position_night;
    int valve1_position_night;
    int valve2_position_night;
    int valve3_position_night;
    int valve4_position_night;
    int valve5_position_night;
    int valve6_position_night;
    int valve7_position_night;
    int valve8_position_night;
    int valve9_position_night;
    int valve10_position_night;
    int valve11_position_night;
} State_night_settings;

typedef struct
{
    char enable_state_highco2day[SMALL_CONFIG_ITEM];
    char state_highco2day_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highco2day[LARGE_CONFIG_ITEM];
    int co2_high_state_highco2day;
    int co2_low_state_highco2day;
    int valve0_position_highco2day;
    int valve1_position_highco2day;
    int valve2_position_highco2day;
    int valve3_position_highco2day;
    int valve4_position_highco2day;
    int valve5_position_highco2day;
    int valve6_position_highco2day;
    int valve7_position_highco2day;
    int valve8_position_highco2day;
    int valve9_position_highco2day;
    int valve10_position_highco2day;
    int valve11_position_highco2day;
} State_highco2day_settings;

typedef struct
{
    char enable_state_highco2night[SMALL_CONFIG_ITEM];
    char state_highco2night_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highco2night[LARGE_CONFIG_ITEM];
    int co2_high_state_highco2night;
    int co2_low_state_highco2night;
    int valve0_position_highco2night;
    int valve1_position_highco2night;
    int valve2_position_highco2night;
    int valve3_position_highco2night;
    int valve4_position_highco2night;
    int valve5_position_highco2night;
    int valve6_position_highco2night;
    int valve7_position_highco2night;
    int valve8_position_highco2night;
    int valve9_position_highco2night;
    int valve10_position_highco2night;
    int valve11_position_highco2night;
} State_highco2night_settings;

typedef struct
{
    char enable_state_highrhday[SMALL_CONFIG_ITEM];
    char state_highrhday_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highrhday[LARGE_CONFIG_ITEM];
    int rh_low_state_highrhday;
    int maximum_state_time_highrhday;
    int valve0_position_highrhday;
    int valve1_position_highrhday;
    int valve2_position_highrhday;
    int valve3_position_highrhday;
    int valve4_position_highrhday;
    int valve5_position_highrhday;
    int valve6_position_highrhday;
    int valve7_position_highrhday;
    int valve8_position_highrhday;
    int valve9_position_highrhday;
    int valve10_position_highrhday;
    int valve11_position_highrhday;
} State_highrhday_settings;

typedef struct
{
    char enable_state_highrhnight[SMALL_CONFIG_ITEM];
    char state_highrhnight_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highrhnight[LARGE_CONFIG_ITEM];
    int rh_low_state_highrhnight;
    int maximum_state_time_highrhnight;
    int valve0_position_highrhnight;
    int valve1_position_highrhnight;
    int valve2_position_highrhnight;
    int valve3_position_highrhnight;
    int valve4_position_highrhnight;
    int valve5_position_highrhnight;
    int valve6_position_highrhnight;
    int valve7_position_highrhnight;
    int valve8_position_highrhnight;
    int valve9_position_highrhnight;
    int valve10_position_highrhnight;
    int valve11_position_highrhnight;
} State_highrhnight_settings;

typedef struct
{
    char enable_state_cooking[SMALL_CONFIG_ITEM];
    char state_cooking_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_cooking[LARGE_CONFIG_ITEM];
    int start_hour_state_cooking;
    int start_minute_state_cooking;
    int stop_hour_state_cooking;
    int stop_minute_state_cooking;
    int valve0_position_cooking;
    int valve1_position_cooking;
    int valve2_position_cooking;
    int valve3_position_cooking;
    int valve4_position_cooking;
    int valve5_position_cooking;
    int valve6_position_cooking;
    int valve7_position_cooking;
    int valve8_position_cooking;
    int valve9_position_cooking;
    int valve10_position_cooking;
    int valve11_position_cooking;
} State_cooking_settings;

typedef struct
{
    char enable_state_cyclingday[SMALL_CONFIG_ITEM];
    char state_cyclingday_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_cyclingday[LARGE_CONFIG_ITEM];
    int valve0_position_cyclingday;
    int valve1_position_cyclingday;
    int valve2_position_cyclingday;
    int valve3_position_cyclingday;
    int valve4_position_cyclingday;
    int valve5_position_cyclingday;
    int valve6_position_cyclingday;
    int valve7_position_cyclingday;
    int valve8_position_cyclingday;
    int valve9_position_cyclingday;
    int valve10_position_cyclingday;
    int valve11_position_cyclingday;
} State_cyclingday_settings;

typedef struct
{
    char enable_state_cyclingnight[SMALL_CONFIG_ITEM];
    char state_cyclingnight_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_cyclingnight[LARGE_CONFIG_ITEM];
    int valve0_position_cyclingnight;
    int valve1_position_cyclingnight;
    int valve2_position_cyclingnight;
    int valve3_position_cyclingnight;
    int valve4_position_cyclingnight;
    int valve5_position_cyclingnight;
    int valve6_position_cyclingnight;
    int valve7_position_cyclingnight;
    int valve8_position_cyclingnight;
    int valve9_position_cyclingnight;
    int valve10_position_cyclingnight;
    int valve11_position_cyclingnight;
} State_cyclingnight_settings;

typedef struct
{
    int valve0_position_temp;
    int valve1_position_temp;
    int valve2_position_temp;
    int valve3_position_temp;
    int valve4_position_temp;
    int valve5_position_temp;
    int valve6_position_temp;
    int valve7_position_temp;
    int valve8_position_temp;
    int valve9_position_temp;
    int valve10_position_temp;
    int valve11_position_temp;
} State_temp_settings;

typedef struct
{
    int valve_number[MAX_VALVES];
    int position_change[MAX_VALVES];
    int direction[MAX_VALVES];
    bool check;
    bool write_new_position;

} Valve_control_data;

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int day_of_week;

} RTC_date_time;

extern Network_settings networksettings;
extern RTC_settings rtcsettings;
extern Influxdb_settings influxdbsettings;
extern I2C_settings i2csettings;
extern MQTT_settings mqttsettings;
extern Fan_settings fansettings;
extern Statemachine_settings statemachinesettings;

extern Valve_control_data valvecontroldata;

extern RTC_date_time rtcdatetime;

extern Sensor1_settings sensor1settings[SENSOR_COUNT];
extern Sensor2_settings sensor2settings[SENSOR_COUNT];

extern State_day_settings statedaysettings;
extern State_night_settings statenightsettings;
extern State_highco2day_settings statehighco2daysettings;
extern State_highco2night_settings statehighco2nightsettings;
extern State_highrhday_settings statehighrhdaysettings;
extern State_highrhnight_settings statehighrhnightsettings;
extern State_cooking_settings statecookingsettings;
extern State_cyclingday_settings statecyclingdaysettings;
extern State_cyclingnight_settings statecyclingnightsettings;

extern State_temp_settings statetempsettings;

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
// extern JsonDocument settings_fan_data;

extern String wire_sensor_config_string;
extern String wire1_sensor_config_string;

// Date time data from RTC
/*extern String yearStr;
extern String monthStr;
extern String dayStr;
extern String hourStr;
extern String minuteStr;
extern String secondStr;
extern String dayOfWeek;*/

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
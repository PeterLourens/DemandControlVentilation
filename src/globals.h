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

#define CLOCKPIN1 11
#define LATCHPIN1 12
#define DATAPIN1 13

#define CLOCKPIN2 15
#define LATCHPIN2 16
#define DATAPIN2 14

#elif defined(DEVICE_ESP32)

#define I2C_SCL1 17 // Wire I/O settings (SDA1/SCL1) and
#define I2C_SDA1 16
#define I2C_SCL2 18 // Wire1 I/O settings (SDA2/SCL2)
#define I2C_SDA2 19

#define CLOCKPIN1 13
#define LATCHPIN1 12
#define DATAPIN1 14

#define CLOCKPIN2 26
#define LATCHPIN2 25
#define DATAPIN2 27

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
#define SETTINGS_INFLUXDB_PATH "/json/settings_influxdb.json"
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
#define XXLARGE_CONFIG_ITEM 650

#define SENSOR_I2C_BUSSES 2  // Number of I2C busses
#define SENSOR_COUNT 8       // Number of sensors per I2C bus
#define SENSOR_DATA_FIELDS 3 // 0=Temp, 1=RH, 2=CO2

#define MAX_SENSORS 16 // Maximum number of sensors supported by the hardware
#define MAX_VALVES 12  // Maximum number of valves

#define MSG_SIZE 150 // Message size

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
    int valve_position_day[MAX_VALVES];
} State_day_settings;

typedef struct
{
    char enable_state_night[SMALL_CONFIG_ITEM];
    char state_night_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_night[LARGE_CONFIG_ITEM];
    int state_night_highco2;
    int state_night_highrh;
    int valve_position_night[MAX_VALVES];
} State_night_settings;

typedef struct
{
    char enable_state_highco2day[SMALL_CONFIG_ITEM];
    char state_highco2day_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highco2day[LARGE_CONFIG_ITEM];
    int co2_high_state_highco2day;
    int co2_low_state_highco2day;
    int valve_position_highco2day[MAX_VALVES];
} State_highco2day_settings;

typedef struct
{
    char enable_state_highco2night[SMALL_CONFIG_ITEM];
    char state_highco2night_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highco2night[LARGE_CONFIG_ITEM];
    int co2_high_state_highco2night;
    int co2_low_state_highco2night;
    int valve_position_highco2night[MAX_VALVES];
} State_highco2night_settings;

typedef struct
{
    char enable_state_highrhday[SMALL_CONFIG_ITEM];
    char state_highrhday_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highrhday[LARGE_CONFIG_ITEM];
    int rh_low_state_highrhday;
    int maximum_state_time_highrhday;
    int valve_position_highrhday[MAX_VALVES];
} State_highrhday_settings;

typedef struct
{
    char enable_state_highrhnight[SMALL_CONFIG_ITEM];
    char state_highrhnight_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_highrhnight[LARGE_CONFIG_ITEM];
    int rh_low_state_highrhnight;
    int maximum_state_time_highrhnight;
    int valve_position_highrhnight[MAX_VALVES];
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
    int valve_position_cooking[MAX_VALVES];
} State_cooking_settings;

typedef struct
{
    char enable_state_cyclingday[SMALL_CONFIG_ITEM];
    char state_cyclingday_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_cyclingday[LARGE_CONFIG_ITEM];
    int valve_position_cyclingday[MAX_VALVES];
} State_cyclingday_settings;

typedef struct
{
    char enable_state_cyclingnight[SMALL_CONFIG_ITEM];
    char state_cyclingnight_fanspeed[SMALL_CONFIG_ITEM];
    char name_state_cyclingnight[LARGE_CONFIG_ITEM];
    int valve_position_cyclingnight[MAX_VALVES];
} State_cyclingnight_settings;

// To store temporary valve positions settings defined in statemachine
typedef struct
{
    int valve_position_temp[MAX_VALVES];
} State_temp_settings;

// To store valve control data from web page and from statemachine and actual valvel data
typedef struct
{
    int valve_number[MAX_VALVES];
    int position_change[MAX_VALVES];
    int direction[MAX_VALVES];
    int actual_valve_position[MAX_VALVES];
    int check_position;
    int write_new_position;

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

extern char state[LARGE_CONFIG_ITEM];
extern char fanspeed[MEDIUM_CONFIG_ITEM];

extern bool lock_valve_move;
extern bool pb_toggle;
extern bool ap_active;
extern bool debug_mode;

// Only for websocket task and function as chars are too large for stack
extern char temp_settings_char[5000];
extern char sensor_config1[1500];
extern char sensor_config2[1500];
extern char settings_statemachine[XXLARGE_CONFIG_ITEM];
extern char settings_state_day[XXLARGE_CONFIG_ITEM];
extern char settings_state_night[XXLARGE_CONFIG_ITEM];
extern char settings_state_highco2day[XXLARGE_CONFIG_ITEM];
extern char settings_state_highco2night[XXLARGE_CONFIG_ITEM];
extern char settings_state_highrhday[XXLARGE_CONFIG_ITEM];
extern char settings_state_highrhnight[XXLARGE_CONFIG_ITEM];
extern char settings_state_cooking[XXLARGE_CONFIG_ITEM];
extern char settings_state_cyclingday[XXLARGE_CONFIG_ITEM];
extern char settings_state_cyclingnight[XXLARGE_CONFIG_ITEM];
extern char settings_rtc[XXLARGE_CONFIG_ITEM];
extern char settings_influxdb[XXLARGE_CONFIG_ITEM];
extern char settings_i2c[XXLARGE_CONFIG_ITEM];
extern char settings_mqtt[XXLARGE_CONFIG_ITEM];
extern char settings_fan[XXLARGE_CONFIG_ITEM];
extern char settings_network[XXLARGE_CONFIG_ITEM];

// Data pins for 74HC595
// extern int clockPin1;
// extern int latchPin1;
// extern int dataPin1;

// Data pins for 74HC595
// extern int clockPin2;
// extern int latchPin2;
// extern int dataPin2;

// Function definitions
#endif
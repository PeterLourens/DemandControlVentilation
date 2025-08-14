#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <RTClib.h>

#define I2C_SCL1 38                                                 // Wire I/O settings (SDA1/SCL1) and 
#define I2C_SDA1 39
#define I2C_SCL2 40                                                 // Wire1 I/O settings (SDA2/SCL2)
#define I2C_SDA2 41

#define LCD_ROWS 4                                                  // LCD display rows
#define LCD_COLUMNS 20                                              // LCD display columns
#define LCDADDR 0x27                                                // LCD I2C address

//#define TCAADDR 0x70                                                //TCA9548 address

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
extern SemaphoreHandle_t valve_control_data_mutex;
extern SemaphoreHandle_t date_time_mutex;
extern SemaphoreHandle_t settings_network_mutex;
extern SemaphoreHandle_t settings_mqtt_mutex;
extern SemaphoreHandle_t settings_i2c_mutex;
extern SemaphoreHandle_t settings_fan_mutex;
extern SemaphoreHandle_t settings_statemachine_mutex;
extern SemaphoreHandle_t settings_influxdb_mutex;
extern SemaphoreHandle_t settings_rtc_mutex; 
extern SemaphoreHandle_t statemachine_state_mutex;
extern SemaphoreHandle_t fanspeed_mutex;
extern SemaphoreHandle_t lock_valve_move_mutex;
extern SemaphoreHandle_t ap_active_mutex;

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

extern QueueHandle_t sensor_queue;
extern QueueHandle_t sensor_avg_queue;
extern QueueHandle_t webserial_queue;

extern JsonDocument valve_control_data;
extern JsonDocument wire_sensor_data;
extern JsonDocument wire1_sensor_data;
extern JsonDocument settings_fan_data;
extern JsonDocument settings_statemachine_data;

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

//MQTT settings
extern String enable_mqtt;
extern String mqtt_server;
extern int mqtt_port;
extern String mqtt_base_topic;

//InfluxDB settings
extern String enable_influxdb;
extern String influxdb_url;
extern String influxdb_org;
extern String influxdb_bucket;
extern String influxdb_token;

//Settings i2C
extern int bus0_multiplexer_addr;
extern int bus1_multiplexer_addr;
extern String enable_lcd;
extern int display_i2c_addr;

//Settings RTC
extern String ntp_server;
extern String timezone;

//Fan speed control settings
extern String fan_control_mode;
extern String fan_control_mqtt_server;
extern String fan_control_mqtt_port;
extern String fan_control_mqtt_topic;
extern String fan_control_url_high_speed;
extern String fan_control_url_medium_speed;
extern String fan_control_url_low_speed;

//Date time data from RTC
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

//Data pins for 74HC595
extern int clockPin1; 
extern int latchPin1; 
extern int dataPin1; 

//Data pins for 74HC595
extern int clockPin2; 
extern int latchPin2; 
extern int dataPin2;  

//Function definitions

#endif
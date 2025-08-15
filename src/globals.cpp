#include "globals.h"

const char *settings_state_day_path = "/json/settings_state_day.json";
const char *settings_state_night_path = "/json/settings_state_night.json";
const char *settings_state_highco2day_path = "/json/settings_state_highco2day.json";
const char *settings_state_highco2night_path = "/json/settings_state_highco2night.json";
const char *settings_state_highrhday_path = "/json/settings_state_highrhday.json";
const char *settings_state_highrhnight_path = "/json/settings_state_highrhnight.json";
const char *settings_state_cooking_path = "/json/settings_state_cooking.json";
const char *settings_state_cyclingday_path = "/json/settings_state_cyclingday.json";
const char *settings_state_cyclingnight_path = "/json/settings_state_cyclingnight.json";
const char *sensor_config1_path = "/json/sensor_config1.json";
const char *sensor_config2_path = "/json/sensor_config2.json";
const char *valve_positions_path = "/json/valvepositions.json";

TaskHandle_t task_influxdb;
TaskHandle_t task_i2c;
TaskHandle_t task_mqtt;
TaskHandle_t task_np;
TaskHandle_t task_statemach;
TaskHandle_t task_sys;
TaskHandle_t task_valvectrl;
TaskHandle_t h_Task_web;
TaskHandle_t task_wifi;
TaskHandle_t task_espnow;
TaskHandle_t task_websocket;

SemaphoreHandle_t sensor_config_file_mutex = NULL;              // Sensor configuration files
SemaphoreHandle_t valve_position_file_mutex = NULL;             // Valve position file
SemaphoreHandle_t valve_control_data_mutex = NULL;              // Valve control data variable
SemaphoreHandle_t date_time_mutex = NULL;                       // Time variables
SemaphoreHandle_t settings_network_mutex = NULL;                // Network settings
SemaphoreHandle_t settings_mqtt_mutex = NULL;                   // MQTT settings
SemaphoreHandle_t settings_i2c_mutex = NULL;                    // I2C settings
SemaphoreHandle_t settings_fan_mutex = NULL;                    // Fan control settings
SemaphoreHandle_t settings_statemachine_mutex = NULL;           // Statemachine settings
SemaphoreHandle_t settings_influxdb_mutex = NULL;               // InfluxDB settings
SemaphoreHandle_t settings_rtc_mutex = NULL;                    // RTC settings
SemaphoreHandle_t statemachine_state_mutex = NULL;              // for state of statemechine
SemaphoreHandle_t fanspeed_mutex = NULL;                        // for state of fan
SemaphoreHandle_t lock_valve_move_mutex = NULL;                 // for valve lock
SemaphoreHandle_t ap_active_mutex = NULL;                       // for accesspoint active

SemaphoreHandle_t settings_state_day_mutex = NULL;              // Day state settings
SemaphoreHandle_t settings_state_night_mutex = NULL;            // Night state settings
SemaphoreHandle_t settings_state_highco2day_mutex = NULL;       // High CO2 day state settings
SemaphoreHandle_t settings_state_highco2night_mutex = NULL;     // High CO2 Night settings
SemaphoreHandle_t settings_state_highrhday_mutex = NULL;        // High RH day state settings
SemaphoreHandle_t settings_state_highrhnight_mutex = NULL;      // High RH night state settings
SemaphoreHandle_t settings_state_cooking_mutex = NULL;          // Cooking state settings 
SemaphoreHandle_t settings_state_cyclingday_mutex = NULL;       // Valve cycle day state settings
SemaphoreHandle_t settings_state_cyclingnight_mutex = NULL;     // Valve cycle night state settings
SemaphoreHandle_t settings_state_temp_mutex = NULL;             // Temp state settings

QueueHandle_t sensor_queue;                                     // Handle for sensor queue data
QueueHandle_t sensor_avg_queue;                                 // Handle for sensor queue data
QueueHandle_t webserial_queue;                                  // Handle for webserial queue data

JsonDocument valve_control_data;                                // Global for valve control data
JsonDocument wire_sensor_data;                                  // Global for bus0 sensor configuration
JsonDocument wire1_sensor_data;                                 // Global for bus1 sensor configuration
JsonDocument settings_fan_data;                                 // Define global fanspeed settings
JsonDocument settings_statemachine_data;                        // Define global statemachine settings

JsonDocument settings_state_day;                                // Settings for state day
JsonDocument settings_state_night;                              // Settings for state night
JsonDocument settings_state_highco2day;                         // Settings for state highco2day
JsonDocument settings_state_highco2night;                       // Settings for state highco2night
JsonDocument settings_state_highrhday;                          // Settings for state highrhday
JsonDocument settings_state_highrhnight;                        // Settings for state highrhnight
JsonDocument settings_state_cooking;                            // Settings for state cooking
JsonDocument settings_state_cyclingday;                         // Settings for state valve cycling day
JsonDocument settings_state_cyclingnight;                       // Settings for state valve cycling night
JsonDocument settings_state_temp;                             // Settings for dynamic valve control            

String wire_sensor_config_string = {};
String wire1_sensor_config_string = {};

//Settings for MQTT
String enable_mqtt;
String mqtt_server;
int mqtt_port = 0;
String mqtt_base_topic;

//InfluxDB settings
String enable_influxdb;
String influxdb_url;
String influxdb_org;
String influxdb_bucket;
String influxdb_token;

//Settings i2C
int bus0_multiplexer_addr;
int bus1_multiplexer_addr;
String enable_lcd;
int display_i2c_addr;

//Settings RTC
String ntp_server;
String timezone;

//Fan speed control settings
String fan_control_mode;
String fan_control_mqtt_server;
String fan_control_mqtt_port;
String fan_control_mqtt_topic;
String fan_control_url_high_speed;
String fan_control_url_medium_speed;
String fan_control_url_low_speed;

//Date time data from RTC
String yearStr = "";
String monthStr = "";
String dayStr = "";
String hourStr = "";
String minuteStr = "";
String secondStr = "";
String dayOfWeek = "";

//Statemachine globals
String state = "";
String fanspeed = "";

bool lock_valve_move = 0;                       // Variable for skipping valve move when already moving
bool pb_toggle = false;
bool ap_active = 0;
bool debug_mode = true;

//Data pins for 74HC595
int clockPin1 = 11; // IO11 on ESP32-S3 and D13 on ESP32, connected to SH_CP (11) of 74HC595
int latchPin1 = 12; // IO12 on ESP32-S3 and D12 on ESP32, connected to ST_CP (12) of 74HC595
int dataPin1 = 13;  // IO13 on ESP32-S3 and D14 on ESP32, connected to DS (14) of 74HC595

//Data pins for 74HC595
int clockPin2 = 15; // IO14 on ESP32-S3 and D26 on ESP32, connected to SH_CP (11) of 74HC595
int latchPin2 = 16; // IO15 on ESP32-S3 and D25 on ESP32, connected to ST_CP (12) of 74HC595
int dataPin2 = 14;  // IO16 on ESP32-S3 and D27 on ESP32, connected to DS (14) of 74HC595

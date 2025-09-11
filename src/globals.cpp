#include "globals.h"

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

// Files mutexes
SemaphoreHandle_t sensor_config_file_mutex = NULL;  // Sensor configuration files
SemaphoreHandle_t valve_position_file_mutex = NULL; // Valve position file
SemaphoreHandle_t settings_files_mutex = NULL;      // settings files mutex

// Settings mutexes
SemaphoreHandle_t settings_sensor1_mutex = NULL;      // Sensor 1 settings
SemaphoreHandle_t settings_sensor2_mutex = NULL;      // Sensor 2 settings
SemaphoreHandle_t valve_control_data_mutex = NULL;    // Valve control data variable
SemaphoreHandle_t settings_network_mutex = NULL;      // Network settings
SemaphoreHandle_t settings_mqtt_mutex = NULL;         // MQTT settings
SemaphoreHandle_t settings_i2c_mutex = NULL;          // I2C settings
SemaphoreHandle_t settings_fan_mutex = NULL;          // Fan control settings
SemaphoreHandle_t settings_statemachine_mutex = NULL; // Statemachine settings
SemaphoreHandle_t settings_influxdb_mutex = NULL;     // InfluxDB settings
SemaphoreHandle_t settings_rtc_mutex = NULL;          // RTC settings

SemaphoreHandle_t settings_state_day_mutex = NULL;          // Day state settings
SemaphoreHandle_t settings_state_night_mutex = NULL;        // Night state settings
SemaphoreHandle_t settings_state_highco2day_mutex = NULL;   // High CO2 day state settings
SemaphoreHandle_t settings_state_highco2night_mutex = NULL; // High CO2 Night settings
SemaphoreHandle_t settings_state_highrhday_mutex = NULL;    // High RH day state settings
SemaphoreHandle_t settings_state_highrhnight_mutex = NULL;  // High RH night state settings
SemaphoreHandle_t settings_state_cooking_mutex = NULL;      // Cooking state settings
SemaphoreHandle_t settings_state_cyclingday_mutex = NULL;   // Valve cycle day state settings
SemaphoreHandle_t settings_state_cyclingnight_mutex = NULL; // Valve cycle night state settings
SemaphoreHandle_t settings_state_temp_mutex = NULL;         // Temp state settings

SemaphoreHandle_t date_time_mutex = NULL;          // Time variables
SemaphoreHandle_t statemachine_state_mutex = NULL; // for state of statemechine
SemaphoreHandle_t fanspeed_mutex = NULL;           // for state of fan
SemaphoreHandle_t lock_valve_move_mutex = NULL;    // for valve lock
SemaphoreHandle_t ap_active_mutex = NULL;          // for accesspoint active

QueueHandle_t sensor_queue;     // Handle for sensor queue data
QueueHandle_t sensor_avg_queue; // Handle for sensor queue data
QueueHandle_t webserial_queue;  // Handle for webserial queue data

// Structs for settings
Network_settings networksettings;
RTC_settings rtcsettings;
Influxdb_settings influxdbsettings;
I2C_settings i2csettings;
MQTT_settings mqttsettings;
Fan_settings fansettings;

// Structs for statemachine settings
Statemachine_settings statemachinesettings;
State_day_settings statedaysettings;
State_night_settings statenightsettings;
State_highco2day_settings statehighco2daysettings;
State_highco2night_settings statehighco2nightsettings;
State_highrhday_settings statehighrhdaysettings;
State_highrhnight_settings statehighrhnightsettings;
State_cooking_settings statecookingsettings;
State_cyclingday_settings statecyclingdaysettings;
State_cyclingnight_settings statecyclingnightsettings;

// Used to store settings when sensors reach high CO2 or high RH and a single valve is required to move
State_temp_settings statetempsettings;

// Store data for valve control from web page
Valve_control_data valvecontroldata;

// Date and time information from RTC
RTC_date_time rtcdatetime;

// Sensor settings
Sensor1_settings sensor1settings[SENSOR_COUNT];
Sensor2_settings sensor2settings[SENSOR_COUNT];

// Statemachine globals
char state[LARGE_CONFIG_ITEM] = {};
char fanspeed[MEDIUM_CONFIG_ITEM] = {};

bool lock_valve_move = 0; // Variable for skipping valve move when already moving
bool pb_toggle = false;
bool ap_active = 0;
bool debug_mode = true;

// Char for settings statemachine. In globals because too big for stack
char temp_settings_char[5000] = {};
char sensor_config1[1500] = {};
char sensor_config2[1500] = {};
char settings_statemachine[XXLARGE_CONFIG_ITEM] = {};
char settings_state_day[XXLARGE_CONFIG_ITEM] = {};
char settings_state_night[XXLARGE_CONFIG_ITEM] = {};
char settings_state_highco2day[XXLARGE_CONFIG_ITEM] = {};
char settings_state_highco2night[XXLARGE_CONFIG_ITEM] = {};
char settings_state_highrhday[XXLARGE_CONFIG_ITEM] = {};
char settings_state_highrhnight[XXLARGE_CONFIG_ITEM] = {};
char settings_state_cooking[XXLARGE_CONFIG_ITEM] = {};
char settings_state_cyclingday[XXLARGE_CONFIG_ITEM] = {};
char settings_state_cyclingnight[XXLARGE_CONFIG_ITEM] = {};
char settings_rtc[XXLARGE_CONFIG_ITEM] = {};
char settings_influxdb[XXLARGE_CONFIG_ITEM] = {};
char settings_i2c[XLARGE_CONFIG_ITEM] = {};
char settings_mqtt[XXLARGE_CONFIG_ITEM] = {};
char settings_fan[XXLARGE_CONFIG_ITEM] = {};
char settings_network[XXLARGE_CONFIG_ITEM] = {};

// Temporary chars for concatJsonChar function. Must be in globals because too big for stack
// char concatJsonCharstemp1[5000]; // Adjust size as needed
// char concatJsonCharstemp2[600];

// char json[5000] = {};

// Data pins for 74HC595
// int clockPin1 = 11; // IO11 on ESP32-S3 and D13 on ESP32, connected to SH_CP (11) of 74HC595
// int latchPin1 = 12; // IO12 on ESP32-S3 and D12 on ESP32, connected to ST_CP (12) of 74HC595
// int dataPin1 = 13;  // IO13 on ESP32-S3 and D14 on ESP32, connected to DS (14) of 74HC595

// Data pins for 74HC595
// int clockPin2 = 15; // IO14 on ESP32-S3 and D26 on ESP32, connected to SH_CP (11) of 74HC595
// int latchPin2 = 16; // IO15 on ESP32-S3 and D25 on ESP32, connected to ST_CP (12) of 74HC595
// int dataPin2 = 14;  // IO16 on ESP32-S3 and D27 on ESP32, connected to DS (14) of 74HC595

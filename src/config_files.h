#ifndef CONFIG_FILES_H
#define CONFIG_FILES_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "globals.h"
#include "general.h"

// New function definitions
bool read_settings(const char *path, char *buffer, size_t bufferSize, SemaphoreHandle_t mutex);
bool write_settings(const char *, char *, SemaphoreHandle_t mutex);
bool parse_network_settings(void);
bool parse_rtc_settings(void);
bool parse_influxdb_settings(void);
bool parse_i2c_settings(void);
bool parse_mqtt_settings(void);
bool parse_fan_settings(void);
bool parse_statemachine_settings(void);
bool parse_sensor1_settings(void);
bool parse_sensor2_settings(void);

bool parse_state_day_settings(void);
bool parse_state_night_settings(void);
bool parse_state_highco2day_settings(void);
bool parse_state_highco2night_settings(void);

// bool parse_valve_settings(void);

// Old
String read_network_config(void);

String read_rtc_config(void);
void process_rtc_config(void);

String read_influxdb_config(void);
void process_influxdb_config(void);

String read_i2c_config(void);
void process_i2c_config(void);

String read_mqtt_config(void);
void process_mqtt_config(void);

String read_fan_config(void);
void process_fan_config(void);

String read_statemachine_config(void);
void process_statemachine_config(void);

void valve_status_file_create(void);
void delete_file(const char *);
bool check_file_exists(const char *);
String read_config_file(const char *);
bool write_config_file(const char *, String);
//void sensor_config_data_read(void);
void valve_settings_config_read(void);

#endif
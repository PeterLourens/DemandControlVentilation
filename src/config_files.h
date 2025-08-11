#ifndef CONFIG_FILES_H
#define CONFIG_FILES_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "globals.h"
#include "general.h"

// Function definitions
void read_time_settings(void);
void read_influxdb_config(void);
void read_i2c_config(void);
void read_mqtt_config(void);
void read_fan_config(void);
void valve_status_file_create(void);
void delete_file(const char *);
bool check_file_exists(const char *);
String read_config_file(const char *);
// bool verify_valve_position_file_contents(void);
bool write_config_file(const char *, String);
void sensor_config_data_read(void);
void valve_settings_config_read(void);

#endif
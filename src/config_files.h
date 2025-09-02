#ifndef CONFIG_FILES_H
#define CONFIG_FILES_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "globals.h"
#include "general.h"

// New function definitions
bool read_settings(const char *, char *, size_t, SemaphoreHandle_t);
bool write_settings(const char *, char *, SemaphoreHandle_t);
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
bool parse_state_highrhday_settings(void);
bool parse_state_highrhnight_settings(void);
bool parse_state_cooking_settings(void);
bool parse_state_cyclingday_settings(void);
bool parse_state_cyclingnight_settings(void);

bool parse_actual_valve_positions(void);

void valve_status_file_create(void);
void delete_file(const char *);
bool check_file_exists(const char *);

#endif
#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "globals.h"
#include "config_files.h"
#include "PubSubClient.h"
#include "uptime_formatter.h"


//Function definitions
void read_mqtt_config(void);
void publish_sensor_data(void);
void publish_avg_sensor_data(void);
void publish_valve_positions(void);
void publish_uptime(void);
void publish_fanspeed(void);
void publish_state(void);

#endif
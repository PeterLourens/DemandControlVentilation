#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "globals.h"
#include "config_files.h"
#include "PubSubClient.h"

//Function definitions
//void callback(char *, byte *, unsigned int);
void publish_sensor_data(void);
void publish_avg_sensor_data(void);
void publish_valve_positions(void);
void publish_uptime(void);
void publish_fanspeed(void);
void publish_state(void);
//void setup_mqtt_remotes(void);
//void loop_client(void);

#endif
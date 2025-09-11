#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "config_files.h"
#include "globals.h"
#include "general.h"

// function declarations
String create_index_json(void);
String create_settings_json(void);
void create_sensors_json(void);
// void create_statemachine_json(char *, size_t);
void create_statemachine_json(void);
String create_valvecontrol_json(void);

#endif

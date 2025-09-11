#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "config_files.h"
#include "globals.h"
#include "general.h"

// function declarations
void create_index_json(void);
void create_settings_json(void);
void create_sensors_json(void);
void create_statemachine_json(void);
void create_valvecontrol_json(void);

#endif

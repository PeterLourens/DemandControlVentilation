#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <LittleFS.h>

#include "globals.h"
#include "general.h"
#include "timefunctions.h"
#include "fancontrol.h"
#include "valvecontrol.h"
#include "config_files.h"

// function declarations
void init_statemachine(void);
void run_statemachine(void);

void init_transitions(void);
void stopped_transitions(void);
void day_transitions(void);
void night_transitions(void);
void high_co2_day_transitions(void);
void high_co2_night_transitions(void);
void high_rh_day_transitions(void);
void high_rh_night_transitions(void);
void cooking_transitions(void);
void valve_cycle_day_transitions(void);
void valve_cycle_night_transitions(void);
void manual_high_speed_transitions(void);
void select_sensors(void);

#endif

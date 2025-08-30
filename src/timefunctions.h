#ifndef TIMEFUNCTIONS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define TIMEFUNCTIONS_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "globals.h"

// function declarations
void sync_rtc_ntp(void);
bool cooking_times(void);
bool valve_cycle_times_day(void);
bool valve_cycle_times_night(void);
bool is_weekend(void);
bool is_day(void);

#endif
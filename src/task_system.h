#ifndef TASK_SYSTEM_H // To make sure you don't declare the function more than once by including the header multiple times.
#define TASK_SYSTEM_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "system.h"
#include "general.h"

// function declarations
void start_task_system(void);
void task_system_code(void *pvParameters);

#endif
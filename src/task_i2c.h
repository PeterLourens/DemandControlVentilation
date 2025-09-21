#ifndef TASK_I2C_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define TASK_I2C_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "i2c.h"
#include "globals.h"
#include "LiquidCrystal_I2C.h" 
#include "general.h"

// function declarations
void start_task_i2c(void);
void task_i2c_code(void *pvParameters);

#endif
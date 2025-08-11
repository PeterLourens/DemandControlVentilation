#ifndef GENERAL_H
#define GENERAL_H

#include <Arduino.h>

#include "globals.h"

// Function definitions
void print_message(String);
String formatted_datetime(void);
String concatJson(String, String);

#endif
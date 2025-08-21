#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "globals.h"
#include "config_files.h"
#include "i2c.h"

// Function definitions
// Sets up Wi-Fi using DHCP
bool setupDHCP(const char *ssid, const char *wifi_password);

// Sets up Wi-Fi using static IP configuration
bool setupStaticIP(const char *ssid, const char *wifi_password,
                   const char *ip_address, const char *subnet_mask,
                   const char *gateway, const char *primary_dns,
                   const char *secondary_dns);

// Main Wi-Fi configuration function
void config_wifi(void);

// Converts an array of IP address strings to a 2D array of integers
int **splitStringsToInts(const char *input[]);

#endif
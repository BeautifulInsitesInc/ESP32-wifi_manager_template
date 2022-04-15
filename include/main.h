#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// DEBUGGING
#define DEBUG // Comment out to remove all serial prints

#ifdef DEBUG
  #define out(x) Serial.print(x)
  #define outln(x) Serial.println(x)
  #define tout(x) Telnet.print(x)
  #define toutln(x) Telnet.println(x)
#else
  #define out(x)
  #define outln(x)
  #define tout(x)
  #define toutln(x)
#endif

// ADDITIONAL FILES
#include "lcd_functions.h"
#include "wifi_manager.h"
#include "battery.h"
#include "web_stuff.h"

#endif
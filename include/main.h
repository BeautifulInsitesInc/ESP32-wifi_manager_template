#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <WebSerial.h>

//#define DEBUG
#ifdef DEBUG
  //#define out(x) Serial.print(x)
  //#define outln(x) Serial.println(x)
  #define out(x) WebSerial.print(x)
  #define outln(x) WebSerial.println(x)
#else
  #define out(x)
  #define outln(x)
#endif




#endif
// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor
// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
//
// ------------------------------------------------------------------------------------
// GLOBAL DEFINES
// ------------------------------------------------------------------------------------

#include <Arduino.h>

#ifndef mydefines_h
#define mydefines_h



#define particleSensorPin           8                 //particle sensor pin; reciving the sensor values with it
#define lightningSensorPin           3                 //lightning Sensor Pin; reciving the sensor values with it


#define LCDADDRESS      0x27              // some LCD displays maybe at 0x3F, use I2Cscanner to find the correct address
#define OLEDADDRESS     0x3C              // address of OLED, do not change

#define mySQMSensor     5                 // TSL237 OUT to digital pin 5 (cannot change)
#define rs_AO           A1                // the rain sensor analog port pin, voltage
#define rs_DO           2                 // the rain sensor digital port pin, raining=yes/no
#define BME280_I2CADDR  0x76
#define LCDADDRESS      0x27
#define OLEDADDRESS     0x3C              // address of OLED
#define GPSRXPin        4
#define GPSTXPin        3                 // the gps transmits to Arduino to pin D4, Arduino transmits to GPS using Pin3 to RXD

#define LCDUPDATETIME     4260              // Display update interval
#define MLXUPDATETIME     3721              // MLX sensor update interval
#define BMEUPDATETIME     4635              // BME sensor update interval
#define RAINUPDATETIME    6133              // rain sensor update interval
#define LUXUPDATETIME     2764 
#define SQMUPDATETIME     3233

#define buf_size        20                // size of temporary buffers for string conversions
#define timebuf_size    12
#define datebuf_size    12
#define SerialPortSpeed 9600              // Note that to talk to APT as a Sky Meter needs to be 115200
#define GPSPortSpeed    9600              // The speed at which the controller talks to a GPS unit
#define LDRCutoff1      275               // boundary between black and dark, used to determine Gate time
#define LDRCutoff2      500               // boundary between dark and light
#define blackperiod     2000              // 2s Gate time, at very dark sites
#define darkperiod      1000              // 1s Gate time, at dark sites
#define lightperiod     100               // 100ms Gate time for day light
#define MAXCOMMAND      8                 // size of receive buffer, :xxxx#

#define SKYCLEAR        1
#define SKYPCLOUDY      2
#define SKYCLOUDY       3
#define SKYUNKNOWN      0

// TSL2591 Integration and gain settings
#define INTEGRATIONTIME_LOW       0x00      // 100 millis
#define INTEGRATIONTIME_200MS     0x01      // 200 millis
#define INTEGRATIONTIME_MED       0x02      // 300 millis
#define INTEGRATIONTIME_400MS     0x03      // 400 millis
#define INTEGRATIONTIME_HIGH      0x04      // 500 millis
#define INTEGRATIONTIME_MAX       0x05      // 600 millis
#define GAIN_LOW                  0x00      // low gain (1x)
#define GAIN_MED                  0x10      // medium gain (25x)
#define GAIN_HIGH                 0x20      // medium gain (428x)
#define GAIN_MAX                  0x30      // max gain (9876x)

#define copywritestr    "4BHEL"
#define EOFSTR          '#'

// ----------------------------------------------------------------------------------------------
// 2. TRACING -- DO NOT CHANGE
// ----------------------------------------------------------------------------------------------
// ArduinoTrace - github.com/bblanchon/ArduinoTrace
// Copyright Benoit Blanchon 2018-2019
// Provide a trace fucntion, printing file, line number, function and parameters
// DEBUG needs to be defined to get output to Serial Port
// If DEBUG is not defined nothing happens
#define TRACE() \
DebugPrint(__FILE__); \
DebugPrint(':'); \
DebugPrint(__LINE__); \
DebugPrint(": "); \
DebugPrintln(__PRETTY_FUNCTION__);

// ----------------------------------------------------------------------------------------------
// 3. DEBUGGING -- DO NOT CHANGE
// ----------------------------------------------------------------------------------------------
//#define DEBUG     1
//#define LOOPTIMETEST 1

#ifdef  DEBUG                                         //Macros are usually in all capital letters.
#define DebugPrint(...) Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
#define DebugPrintln(...) Serial.println(__VA_ARGS__) //DPRINTLN is a macro, debug print with new line
#else
#define DebugPrint(...)                               //now defines a blank line
#define DebugPrintln(...)                             //now defines a blank line
#endif



#endif

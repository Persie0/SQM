// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor
// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
//
// ------------------------------------------------------------------------------------
// HARDWARE MAPPINGS
// ------------------------------------------------------------------------------------

#include <Arduino.h>

#ifndef hardwaremappings_h
#define hardwaremappings_h

#define LCDADDRESS      0x27              // some LCD displays maybe at 0x3F, use I2Cscanner to find the correct address
#define OLEDADDRESS     0x3C              // address of OLED, do not change


#define particleSensorPin           6                 // 


#define mySQMSensor     5                 // TSL237 OUT to digital pin 5 (cannot change)
#define rs_AO           A1                // the rain sensor analog port pin, voltage
#define rs_DO           2                 // the rain sensor digital port pin, raining=yes/no
#define BME280_I2CADDR  0x76
#define LCDADDRESS      0x27
#define OLEDADDRESS     0x3C              // address of OLED
#define GPSRXPin        4
#define GPSTXPin        3                 // the gps transmits to Arduino to pin D4, Arduino transmits to GPS using Pin3 to RXD


// ------------------------------------------------------------------------------------
// HARDWARE MAPPINGS
// ------------------------------------------------------------------------------------
// Connection for TSL237 Sensor
// VDD (pin2) to +5V (Note: Use a 0.1uf ceramic capacitor mounted close to the sensor across VDD and GND)
// GND (pin1) to GND
// VOUT (pin3) to Arduino Pin D5

// Connection for NEO-6M-GPS
// GPS-TXD to Arduino Pin D4
// GPS-RXD to Arduino Pin D3
// GND to GND
// VCC to +5V

// Connection for LCD2004 or LCD1604 OR LCD1602 I2C display
// VCC to +5V
// GND to GND
// SDA to A4
// SCL to A5

// Connection for MLX90614 IR Sensor - be careful what I2C connector you connect this to
// Connecting to 5V will destroy this sensor
// VCC  3.3V
// GND  GND
// SDA  A4
// SCL  A5

// Connection for TSL2561 Sensor - be careful what I2C connector you connect this to
// Connecting to 5V will destroy this sensor
// VCC  3.3V
// GND  GND
// SDA  A4
// SCL  A5
// If not fitted, then GL5528 LDR will be used for lux

// Connection for GL5528 LDR
// 5V - GL5528 - Arduino pin A0 - 10K Pulldown resistor - GND

// Connection for Rain Sensor
// AO   to Arduino pin A1
// DO   to Arduino pin D2
// GND  GND
// VCC  5V

// Connection for BME280 Barometric Sensor, uses I2C interface - make sure you purchase the 5V version
// VCC  5V
// GND  GND
// SDA  A4
// SCL  A5


#endif

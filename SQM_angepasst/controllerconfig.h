// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor
// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
//
// ------------------------------------------------------------------------------------
// CONTROLLER CONFIGURATION
// ------------------------------------------------------------------------------------
#include <Arduino.h>
#ifndef controllerconfig_h
#define controllerconfig_h

// ------------------------------------------------------------------------------------
// PREFACE
// ------------------------------------------------------------------------------------
// Note 1:
// If you try to enable everything your controller will randomly reboot or become unresponsive
// When you compile this firmware the end lines show Sketch uses XXXX bytes (xx%)
// That figure is very important, and for good performace that means the target space
// to aim for is around 85% or less. Above 90% the Arduino will have issues
// So choose your options carefully and do not try to enable everything.
// Note 2:
// If you must have everything, please consider using the replacement project 
// mySQM+ which has Wifi and Web capability - 
// see https://sourceforge.net/projects/mysqmproesp32/

// ------------------------------------------------------------------------------------
// CONFIG SECTION
// ------------------------------------------------------------------------------------
// MAKE YOUR CHANGES HERE
// You can use the LDR or the TSL2561 sensor to calculate LUX values
// The TSL2561 is more accurate - BUT - it does take up more space (about 11% more space)
// to use TSL2561 sensor for determining lux, uncomment the next line
#define TSL2561SENSOR 1

// The GPS takes up about 23% of space so it is quite large
// To enable GPS, uncomment the next line 
//#define useGPSNEO 1

// The rain sensor takes up about 1% of space
// To enable the rain sensor, uncomment the next line
#define RAINSENSOR 1

// The MLX sensor takes up about 2% of space
// To enable the MLX90614 ir sensor uncomment the next line
#define MLX90614SENSOR 1

// The BME sensor takes up about 8% of space 
// To enable the BME280 humdity, ambient temp, barmetric pressure sensor uncomment the next line
//#define BME280SENSOR 1

// YOU MUST CHOOSE ONE OF THESE SENSORS TO GET AN SQM READING, not both, recommended is TSL237
//#define TSL235 1
#define TSL237 2

// The LCD display takes up about 9% of space
// To enable the LCD DISPLAY uncomment the next line [note lcd takes less space then oled]
#define LCDDISPLAY 1

// to enable LCD1602 or LCD1604 or LCD2004, only uncomment one of the below choices
// IF using an LCD display you must have only ONE uncommented
// IF not using an lcd display then leave all commented out
//#define LCD1602   1
//#define LCD1604   2
#define LCD2004   3

// The OLED display takes up about 11% of space
// To enable the OLED display (SSD1306 chip) uncomment the next line
//#define OLEDDISPLAY 1

// YOU MUST CHOOSE ONE OF THE FOLLOWING METHODS TO CALCULATE THE SQM VALUE
//#define OLDSQMMETHOD 1
//#define NEWSQMMETHOD 2
#define NEWSQMMETHODCORRECTED 3
//#define IRRADIANCEMETHOD 4

// To enable SQM remote support [INDI/EKOS and APT] uncomment the next line
//#define SQMSUPPORT 1

// To average out SQM readings over a number of samples, uncomment the next line
#define RUNNINGAVERAGE 1

// The following defines the number of samples used to create an average SQM reading
// The higher the value, the long it takes to generate an SQM reading
#define SAMPLES 5

// ------------------------------------------------------------------------------------
// DO NOT CHANGE
// ------------------------------------------------------------------------------------

#ifdef LCDDISPLAY
#ifdef OLEDDISPLAY
#halt //Error - you can only have one LCD type defined - either LCDDISPLAY or OLEDDISPLAY.
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1602
#ifdef LCD1604
#halt //Error - you can only have one LCD type defined, one of LCD1602, LCD1604 or LCD2004.
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1602
#ifdef LCD2004
#halt //Error - you can only have one LCD type defined, one of LCD1602, LCD1604 or LCD2004.
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1604
#ifdef LCD2004
#halt //Error - you can only have one LCD type defined, one of LCD1602, LCD1604 or LCD2004.
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1602
#ifdef LCD2004
#ifdef LCD1604
#halt //Error - you can only have one LCD type defined, one of LCD1602, LCD1604 or LCD2004.
#endif
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifndef LCD2004
#ifndef LCD1604
#ifndef LCD1602
#halt //Error - you can must define either LCD1602 or LCD2004 or LCD1604.
#endif
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1602
#ifdef LCD2004
#halt // Error, you cannot have both LCD1602 and LCD2004 uncommented at the same time
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1602
#ifdef LCD1604
#halt // Error, you cannot have both LCD1602 and LCD1604 uncommented at the same time
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1604
#ifdef LCD2004
#halt // Error, you cannot have both LCD1604 and LCD2004 uncommented at the same time
#endif
#endif
#endif

#ifdef LCDDISPLAY
#ifndef LCD1602
#ifndef LCD2004
#ifndef LCD1604
#halt // Error, you must have either LCD1602 OR LCD1604 OR LCD2004 uncommented
#endif
#endif
#endif
#endif

#ifndef OLDSQMMETHOD
#ifndef NEWSQMMETHOD
#ifndef NEWSQMMETHODCORRECTED
#ifndef IRRADIANCEMETHOD
#halt //Error - you must define ONE sqm method above
#endif
#endif
#endif
#endif

#ifdef OLDSQMMETHOD
#ifdef NEWSQMMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef NEWSQMMETHODCORRECTED
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef IRRADIANCEMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#endif

#ifdef NEWSQMMETHOD
#ifdef OLDSQMMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef NEWSQMMETHODCORRECTED
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef IRRADIANCEMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#endif

#ifdef NEWSQMMETHODCORRECTED
#ifdef OLDSQMMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef NEWSQMMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef IRRADIANCEMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#endif

#ifdef IRRADIANCEMETHOD
#ifdef OLDSQMMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef NEWSQMMETHOD
#halt //Error - cannot have more than one SQM method defined
#endif
#ifdef NEWSQMMETHODCORRECTED
#halt //Error - cannot have more than one SQM method defined
#endif
#endif

#ifdef OLDSQMMETHOD
#define sqm_limit 21.83                 // mag limit for earth is 21.95
#endif
#ifdef NEWSQMMETHOD
#define sqm_limit 23.09                 // mag limit for earth is 21.95
#endif
#ifdef NEWSQMMETHODCORRECTED
//#define sqm_limit 21.95               // mag limit for earth is 21.95
#define sqm_limit 20                    // mostaccurate within 18-22 mpsas
#endif
#ifdef IRRADIANCEMETHOD
#define sqm_limit 21.95                 // mag limit for earth is 21.95
#endif


#endif

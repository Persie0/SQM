/***********************************
  This is the Adafruit GPS library - the ultimate GPS library
  for the ultimate GPS module!

  Tested and works great with the Adafruit Ultimate GPS module
  using MTK33x9 chipset
    ------> http://www.adafruit.com/products/746
  Pick one up today at the Adafruit electronics shop
  and help support open source hardware & software! -ada

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above must be included in any redistribution
****************************************/
// Fllybob added lines 34,35 and 40,41 to add 100mHz logging capability

#ifndef _myADAFRUIT_GPS_H
#define _myADAFRUIT_GPS_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"

// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// how long to wait when we're looking for a response
#define MAXWAITSENTENCE 5

class Adafruit_GPS {
  public:
    void begin(uint32_t baud);

    Adafruit_GPS(SoftwareSerial *ser); // Constructor when using SoftwareSerial

    char *lastNMEA(void);
    boolean newNMEAreceived();
    void common_init(void);

    void sendCommand(const char *);

    void pause(boolean b);

    boolean parseNMEA(char *response);
    uint8_t parseHex(char c);

    char read(void);
    boolean parse(char *);
    void interruptReads(boolean r);

    uint8_t hour, minute, seconds, year, month, day;
    uint16_t milliseconds;
    // Floating point latitude and longitude value in degrees.
    float latitude, longitude;
    // Fixed point latitude and longitude value with degrees stored in units of 1/100000 degrees,
    // and minutes stored in units of 1/100000 degrees.  See pull #13 for more details:
    //   https://github.com/adafruit/Adafruit-GPS-Library/pull/13
    int32_t latitude_fixed, longitude_fixed;
    float latitudeDegrees, longitudeDegrees;
    float geoidheight, altitude;
    float speed, angle, magvariation, HDOP;
    char lat, lon, mag;
    boolean fix;
    uint8_t fixquality, satellites;
    boolean waitForSentence(const char *wait, uint8_t max = MAXWAITSENTENCE);

  private:
    boolean paused;

    uint8_t parseResponse(char *response);
    SoftwareSerial *gpsSwSerial;
};

#endif






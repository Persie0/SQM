// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor
// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
//
// ------------------------------------------------------------------------------------
// SQM SUPPORT UNIHEDRON EMULATION
// ------------------------------------------------------------------------------------


#include <Arduino.h>

#ifndef sqmsupport_h
#define sqmsupport_h

// ix     Unit information request  i,00000002,00000003,00000001,00000413
// rx     Reading request           r, 06.70m,0000022921Hz,0000000020c,0000000.000s, 039.4C 
// cx     Calibration request       c,00000017.60m,0000000.000s, 039.4C,00000008.71m, 039.4C 

// Information Request
//0     i         Indicates that the unit information response is being returned.
//2-9   00000002  Protocol number (8 digits). This will always be the first 8 characters (after the “i,” response).
//                This value indicates the revision number of the data protocol to/from the SQM-LU.
//                The protocol version is independent of the feature version.
//11-18 00000003  Model number (8 digits). The model value identifies the specific hardware model that the
//                firmware is tailored for.
//20-27 00000001  Feature number (8 digits). The feature value identifies software features independent of
//                the data protocol.
//29-36 00000413  Serial number (8 digits). Each unit has its own unique serial number.
//37-38 Carriage return (0x0d), Line feed (0x0a).

#ifdef SQMSUPPORT
void sqmsendinforequest(void)
{
  Serial.print("i,00000002,00000003,");
  Serial.print("00000001,00000413");
  Serial.print('\r');
  Serial.print('\n');
}

void sqmsendreadrequest(void)
{
  // rx command information, in a 56 character string
  // r, 06.70m,0000022921Hz,0000000020c,0000000.000s, 039.4C
  // 0      r = reading returned
  // 2-8    06.70m = magnitude reading, magnitudes per square arc second
  // 10-21  0000022921Hz = Frequency of sensor in Hz
  // 23-33  0000000020c = Period of sensor in counts, counts occur at a rate of 460.8 kHz
  // 35-46  0000000.000s = Period of sensor in seconds with millisecond resolution
  // 48-54  039.4C = Temperature measured at light sensor in degrees C.
  // 55-56  Carriage return (0x0d), Line feed (0x0a)

  char tempStr[14];
  String rxString = "r, ";
  String tmp = "";

  // add magnitude
  if ( mySQMreading < 10.0 )
  {
    rxString = rxString + "0";
  }
  rxString = rxString + String(mySQMreading, 2) + "m,";
  memset(tempStr, 0, 12);

  // add frequency
  tmp = "";
  sprintf( tempStr, "%lu", (long)frequency );
  int index = strlen(tempStr);
  while ( index < 10)
  {
    tmp = tmp + "0";
    index++;
  }
  rxString = rxString + tmp + tempStr;
  rxString = rxString + "Hz,";

  // period of sensor in counts, period of sensor in seconds,
  rxString = rxString + "0000000020c,0000000.000s, ";

  // add temperature eg 039.4C
#ifndef BME280SENSOR
  rxString = rxString + "020.0C";
#else
  tmp = String(bme280temperature, 1) + 'C';
  while ( tmp.length() < 6)
  {
    tmp = '0' + tmp;
  }
  rxString = rxString + tmp;
#endif
  Serial.print(rxString);
  Serial.print('\r');
  Serial.print('\n');
}

void sqmsendcalibrationrequest(void)
{
  // c,00000017.60m,0000000.000s, 039.4C,00000008.71m, 039.4C
  // this routine has not been coded, simply return dummy values
  String rxString = "c,00000017.60m,0000000.000s, ";
  String tmp = "";

  // add temperature light calibration eg 039.4C
#ifndef BME280SENSOR
  rxString = rxString + "020.0C,00000008.71m, ";
#else
  tmp = String(bme280temperature, 1);
  while ( tmp.length() < 6)
  {
    tmp = '0' + tmp;
  }
  rxString = rxString + tmp + "C,00000008.71m, ";
#endif

  // add temperature dark calibration eg 039.4C
#ifndef BME280SENSOR
  rxString = rxString + "020.0C";
#else
  tmp = String(bme280temperature, 1) + 'C';
  while ( tmp.length() < 6)
  {
    tmp = '0' + tmp;
  }
  rxString = rxString + tmp;
#endif
  Serial.print(rxString);
  Serial.print('\r');
  Serial.print('\n');
}
#endif // ifdef SQMSUPPORT

#endif

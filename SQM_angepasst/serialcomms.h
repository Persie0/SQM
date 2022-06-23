// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor
// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
//
// ------------------------------------------------------------------------------------
// SERIAL COMMS
// ------------------------------------------------------------------------------------

#include <Arduino.h>

#ifndef serialcomms_h
#define serialcomms_h

// ------------------------------------------------------------------------------------
// EXTERN FUNCTIONS
// ------------------------------------------------------------------------------------
#ifdef SQMSUPPORT
extern void sqmsendreadrequest(void);
extern void sqmsendinforequest(void);
extern void sqmsendcalibrationrequest(void);
#endif

// ------------------------------------------------------------------------------------
// SERIAL CODE
// ------------------------------------------------------------------------------------

char *ftoa(char *a, double f, int precision)
{
  long p[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

void sendmessage(const char *str)
{
  Serial.print(str);
}

void sendreply(const char token, const char *str)
{
  char mbuffer[32];
  snprintf(mbuffer, sizeof(mbuffer), "%c%s%c", token,  str, EOFSTR);
  sendmessage(mbuffer);
}

void sendreply(const char token, const float val, int i)    // i => decimal place
{
  char buff[32];
  char temp[10];
  // Note Arduino snprintf does not support .2f
  //dtostrf(val, 7, i, temp);
  ftoa(temp, val, i);
  snprintf(buff, sizeof(buff), "%c%s%c", token, temp, EOFSTR);
  sendmessage(buff);
}

void sendreply(const char token, const int val)
{
  char mbuffer[32];
  snprintf(mbuffer, sizeof(mbuffer), "%c%i%c", token,  val, EOFSTR);
  sendmessage(mbuffer);
}

#ifndef SQMSUPPORT
void processCommand()
{
  int cmdval;
  String receiveString = "";
  String param = "";
  char mbuffer[32];
  
  receiveString = queue.pop();
  String cmdstr = receiveString.substring(0, 2);
  cmdval = cmdstr.toInt();
  param = receiveString.substring(2, receiveString.length() );
  memset(mbuffer, 0, 32);
  
#ifdef DEBUG
  Serial.print("receivestr = "); Serial.println(receiveString);
  Serial.print("param = "); Serial.println(param);
  Serial.print("cmdstr = "); Serial.println(cmdstr);
  Serial.print("cmdval = "); Serial.println(cmdval);
#endif
  switch ( cmdval )
  {
    case 1:                         // "Get SQM"
      sendreply('A', mySQMreading, 2);
      break;
    case 2:                         // "Get Frequency"
      sendreply('B', frequency);
      break;
    case 3:                         // "Get Irradiance"
      sendreply('C', irradiance, 2);
      break;
    case 4:                         // if the command is GV "Get Firmware Version"
      sendreply('D', programVersion);
      break;
    case 5:                         // "Get Firmware Filename"
      sendreply('E', programName);
      break;
    case 6:                         // "Get LDR value"
      {
        sendreply('F', analogRead(LDRpin));
      }
      break;
    case 7:                         //  Get period gate time"

      sendreply('G', period);
      break;
    case 8:                         //  "Get Date"
#ifdef useGPSNEO
      mydate.toCharArray(mbuffer, mydate.length() + 1);
      sendreply('H', mbuffer);
#else
      sendreply('H', "01/01/2001");
#endif
      //Serial.print(param);
      break;
    case 9:                         // "Get Time"
#ifdef useGPSNEO
      mytime.toCharArray(mbuffer, mytime.length() + 1);
      sendreply('I', mbuffer);
#else
      sendreply('I', "00:00:00");
#endif
      break;
    case 10:                        // "Get Longitude"
#ifdef useGPSNEO
      mylongitude.toCharArray(mbuffer, mylongitude.length() + 1);
      sendreply('J', mbuffer);
#else
      sendreply('J', "00");
#endif
      //Serial.print(param);
      break;
    case 11:                        // "Get Latitude"
#ifdef useGPSNEO
      mylatitude.toCharArray(mbuffer, mylatitude.length() + 1);
      sendreply('K', mbuffer);
#else
      sendreply('K', "00");
#endif
      break;
    case 12:                        // "Get Altitude"
#ifdef useGPSNEO
      myaltitude.toCharArray(mbuffer, myaltitude.length() + 1);
      sendreply('L', mbuffer);
#else
      sendreply('L', 0);
#endif
      break;
    case 13:                        // "Get Satelittes"
#ifdef useGPSNEO
      mysatellites.toCharArray(mbuffer, mysatellites.length() + 1);
      sendreply('M', mbuffer);
#else
      sendreply('M', 0);
#endif
      break;
    case 16:                        // gps fix?
      param = "N0#";
#ifdef useGPSNEO
      myfix.toCharArray(mbuffer, myfix.length() + 1);
      sendreply('N', mbuffer);
#else
      sendreply('N', 0);
#endif
      break;
    case 19:                        // get IR object temperature
      sendreply('O', skyobject, 2);
      break;
    case 20:                        // get IR ambient temperature
      sendreply('P', skyambient, 2);
      break;
    case 21:                        // return LUX value
      if ( lux < 1 )
      {
        sendreply('U', lux, 5);
      }
      else if ( lux < 100 )
      {
        sendreply('U', lux, 2);
      }
      else
      {
        sendreply('U', lux, 1);
      }
      break;
    case 23:                        // return raining
#ifdef RAINSENSOR
      if ( raining == true )
      {
        sendreply('R', 1);
      }
      else
      {
        sendreply('R', 0);
      }
#else
      sendreply('R', 0);
#endif
      break;
    case 24:                        // return rain voltage (as int) - cannot return Volts (float)
    // upstream code relies on this as an int type 0-5000
#ifdef RAINSENSOR
      sendreply('S', millivolts);
#else
      sendreply('S', 0);
#endif
      break;
    case 25:                        // save setpoint1
      setpoint1 = param.toInt();
      break;
    case 26:                        // save setpoint2
      setpoint2 = param.toInt();
      break;
    case 27:                        // return sky state
      sendreply('V', skystate);
      break;
    case 28:                        // return setpoint1
      sendreply('W', setpoint1);
      break;
    case 29:                        // return setpoint2
      sendreply('X', setpoint2);
      break;
    case 31:                        // "Get Nelm"
      sendreply('Z', nelm, 2);
      break;
    case 32:                          // Get Humidity BME280 sensor
      sendreply('a', humidity, 1);
      break;
    case 33:                          // Get pressure BME280 sensor
      sendreply('e', pressure);
      break;
    case 34:                          // Get temperature BME280 sensor
      sendreply('c', temperature, 2);
      break;
    case 35:                          // Get dewpoint
      sendreply('d', dewpoint, 2);
      break;
  }
}
#endif // ifndef SQMSUPPORT

void clearSerialPort()
{
  while ( Serial.available() )
  {
    Serial.read();
  }
}

// SerialEvent occurs whenever new data comes in the serial RX.
void serialEvent()
{
  // : starts the command, # ends the command, do not store these in the command buffer
  // read the command until the terminating # character
  while (Serial.available())
  {
    char inChar = Serial.read();
    switch ( inChar )
    {
#ifdef SQMSUPPORT
      case 'r':             // sqm read request
        sqmsendreadrequest();
        clearSerialPort();
        break;
      case 'i':             // sqm info request
        sqmsendinforequest();
        clearSerialPort();
        break;
      case 'c':
        sqmsendcalibrationrequest();
        clearSerialPort();
        break;
#endif
#ifndef SQMSUPPORT
      case ':':     // start
        line = "";
        break;
      case EOFSTR:
        queue.push(line);   // and discard trailing #
        break;
      default:      // anything else
        line = line + inChar;
#endif
    }
  }
}



#endif

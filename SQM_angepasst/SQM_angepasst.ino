// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor

// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
// This project is protected by International Copyright Law.
// Permission is granted for personal and Academic/Educational use only.
// Software distributed under MIT License https://opensource.org/licenses/MIT
// ------------------------------------------------------------------------------------
// OFFICIAL FIRMWARE RELEASE 042
// REQUIRES LCD1602, LCD1604, LCD2004 or OLED I2C DISPLAY

// PCB (Version 5) gerber files at project site
// https://sourceforge.net/projects/arduinomysqmskyqualitymeter/
// ------------------------------------------------------------------------------------
// CONTRIBUTIONS
// If you wish to make a contribution in thanks for this project, please use PayPal and send the amount
// to user rbb1brown@gmail.com (Robert Brown). All contributions are gratefully accepted.

// based loosely on code from http://forum.arduino.cc/index.php?PHPSESSID=knhnejk2g00u2smd8ve8tn5l82&topic=21536.45
// and http://stargazerslounge.com/topic/183600-arduino-sky-quality-meter-working/
// Acknowledgements: madepablo, Martin Nawrath (Frequency Counter Library), Corpze

// ------------------------------------------------------------------------------------
// SPECIFY CONTROLLER CONFIGURATION
// ------------------------------------------------------------------------------------
// YOUTODO
// Goto file controllerconfig.h and specify your controller settings

// ------------------------------------------------------------------------------------
// INCLUDES
// ------------------------------------------------------------------------------------
#include "hardwaremappings.h"
#include "controllerconfig.h"
#include "mydefines.h"

// ------------------------------------------------------------------------------------
// FIRMWARE BEGIN
// ------------------------------------------------------------------------------------
// include header files
#include <Arduino.h>
#include <myQueue.h>                      //  By Steven de Salas
#ifndef USELUXMETHOD
#include <FreqCounter.h>                  // required for frequency measurement of TLS sensor, (c) Martin Nawrath
#endif
#include <math.h>                         // required for log()
#ifdef LCDDISPLAY
#include <LCD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>            // needed for LCD, see https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#endif
#ifdef OLEDDISPLAY
#include <Wire.h>                         // needed for I2C, installed when installing the Arduino IDE
#include <mySSD1306AsciiUC.h>             // oled
#include <mySSD1306AsciiWireUC.h>         // oled
#endif
#ifdef useGPSNEO
#include <SoftwareSerial.h>
#endif
#ifdef MLX90614SENSOR
#include <myMLX90614AF.h>                 // IR sensor, (c) Adafruit
#endif
#ifdef BME280SENSOR
#include <mybme280.h>
#endif
#ifdef TSL2561SENSOR
#include <myTSL2561AF.h>
#endif

// ------------------------------------------------------------------------------------
// PROGRAM VARIABLES
// ------------------------------------------------------------------------------------
char    programName[]      = "SchneiderXPerzi";     // Program title and version information
byte    programVersion     = 1;
int     setpoint1;                           // setpoint values used to determine sky state
int     setpoint2;
int     skystate;
double  humidity;
double  temperature;
unsigned long int pressure;
float   dewpoint;
float   skyambient;
float   skyobject;
double  lux;
bool    raining;
int     rainVout;
int     millivolts;
float   volts;

//  StateMachine definition
#define state_lux               1
#define state_sqm               2
#define state_rainsensor        3
#define state_bmesensor         4
#define state_mlxsensor         5
#define state_displayupdate     6



unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 3s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;


#ifdef LCDDISPLAY
LiquidCrystal_I2C lcd(LCDADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif

#ifdef OLEDDISPLAY
SSD1306AsciiWire myoled;
#endif

// TSL237 Sensor
float  mySQMreading;                      // the SQM value, sky magnitude
double frequency;                         // measured TSL237 frequency which is dependent on light
double irradiance;
double nelm;

#ifdef TSL2561SENSOR
TSL2561 myTSL2561(TSL2561_ADDR_FLOAT);
#define tsl2561gainlow  0
#define tsl2561gainhigh 16
#define tsl2561intlow   0
#define tsl2561intmed   1
#define tsl2561inthigh  2
byte tsl2561gain = tsl2561gainhigh;
byte tsl2561int  = tsl2561intmed;
#endif

String  mydate;
String  mytime;
String  mylatitude;
String  mylongitude;
String  myaltitude;
String  mysatellites;
String  myfix;      // 0 if no fix, 1 if fixed

// LDR GL5528
int LDRpin = A0;                          // Light Dependant Resistor is on A0
short period;                             // gate time - specifies duration time for frequency measurement

// IR Sensor, used as a cloud sensor, values help define sky state - cloudy, partly cloudy, clear
#ifdef MLX90614SENSOR
Adafruit_MLX90614 mymlx90614 = Adafruit_MLX90614();
#endif

// Serial command interface
#ifndef SQMSUPPORT
Queue<String> queue(5);                  // receive serial queue of commands
String line;
#endif

#ifdef BME280SENSOR
bme280 mybme280;
#endif

// ------------------------------------------------------------------------------------
// GLOBAL INCLUDES
// ------------------------------------------------------------------------------------
#include "mydisplays.h"
#include "serialcomms.h"
#ifdef SQMSUPPORT
#include "sqmsupport.h"
#endif

// ------------------------------------------------------------------------------------
// METHODS START
// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

byte timecheck(unsigned long x, unsigned long Delay)
{
  unsigned long y = x + Delay;
  unsigned long z = millis();                           // pick current time

  if ((x > y) && (x < z))
    return 0;                                           // overflow y
  if ((x < y) && ( x > z))
    return 1;                                           // overflow z

  return (y < z);                                       // no or (z and y) overflow
}

// calculates dew point
// input:   humidity [%RH], temperature in C
// output:  dew point in C
void calc_dewpoint(float t, float h)
{
  float logEx;
  logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
  dewpoint = (logEx - 0.66077) * 237.3 / (0.66077 + 7.5 - logEx);
}

#if defined(MLX90614SENSOR)
void getskystate()
{
  float TempDiff = skyambient - skyobject;
  // object temp is IR temp of sky which at night time will be a lot less than ambient temp
  // so TempDiff is basically ambient + abs(object)
  // setpoint 1 is set for clear skies
  // setpoint 2 is set for cloudy skies
  // setpoint2 should be lower than setpoint1
  // For clear, Object will be very low, so TempDiff is largest
  // For cloudy, Object is closer to ambient, so TempDiff will be lowest

  // Readings are only valid at night when dark and sensor is pointed to sky
  // During the day readings are meaningless
  if ( TempDiff > setpoint1 )
  {
    skystate = SKYCLEAR;          // clear
  }
  else if ( (TempDiff > setpoint2) && (TempDiff < setpoint1) )
  {
    skystate = SKYPCLOUDY;        // partly cloudy
  }
  else if (TempDiff < setpoint2)
  {
    skystate = SKYCLOUDY;         // cloudy
  }
  else
  {
    skystate = SKYUNKNOWN;        // unknown
  }
}
#endif // #ifdef MLX90614SENSOR

#if defined(RAINSENSOR)
void getraining()
{
  if ( !digitalRead( rs_DO ) )
  {
    raining = true;
  }
  else
  {
    raining = false;
  }
  rainVout = analogRead( rs_AO );
  millivolts = map(rainVout, 0, 1023, 0, 5000 );
  volts = (float) millivolts / 1000.0;
}
#endif  // #ifdef RAINSENSOR

#if defined(TSL2561SENSOR)
void configuresensor( byte gain, byte inttime )
{
  myTSL2561.setGain((tsl2561Gain_t) gain);
  myTSL2561.setTiming((tsl2561IntegrationTime_t) inttime);
}
#endif

// calculate LUX reading from sensor or LDR
void getlux()
{
#if defined(TSL2561SENSOR)
  static uint32_t lum;
  static uint16_t ir, full;

  lum = myTSL2561.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  //if (full < 100)
  // code suggestion by Jozef R to improve accuracy
  if (full < 500)
  {
    //Increase GAIN (and INTEGRATIONTIME) if light level too low
    switch (tsl2561gain)
    {
      case tsl2561gainlow :
        tsl2561gain = tsl2561gainhigh;
        configuresensor(tsl2561gain, tsl2561int);
        break;
      case tsl2561gainhigh :
        if (full < 100)
        // code suggestion by Jozef R to improve accuracy
        if (full < 15)
        {
          switch (tsl2561int)
          {
            case tsl2561intlow:
              tsl2561int = tsl2561intmed;
              configuresensor(tsl2561gain, tsl2561int);
              break;
            case tsl2561intmed:
              tsl2561int = tsl2561inthigh;
              configuresensor(tsl2561gain, tsl2561int);
              break;
            case tsl2561inthigh:
              tsl2561gain = tsl2561gainhigh;
              tsl2561int = tsl2561inthigh;
              configuresensor(tsl2561gain, tsl2561int);
              break;
            default:
              tsl2561gain = tsl2561gainhigh;
              tsl2561int = tsl2561inthigh;
              configuresensor(tsl2561gain, tsl2561int);
              break;
          } // end switch (tsl2561int)
        } // end if (full < 15)
        break;
      default:
        tsl2561gain = tsl2561gainhigh;
        tsl2561int = tsl2561intmed;
        configuresensor(tsl2561gain, tsl2561int);
        break;
    } // end switch (tsl2561gain)
  } // end if (full < 100)
  if (full > 30000) // Decrease GAIN (and INTEGRATIONTIME) if light level too high
  {
    switch (tsl2561gain)
    {
      case tsl2561gainlow :
        tsl2561gain = tsl2561gainlow;
        tsl2561int = tsl2561intlow;
        configuresensor(tsl2561gain, tsl2561int);
        break;
      case tsl2561gainhigh :
        tsl2561gain = tsl2561gainlow;
        tsl2561int = tsl2561intmed;
        configuresensor(tsl2561gain, tsl2561int);
        break;
      default:
        tsl2561gain = tsl2561gainlow;
        tsl2561int = tsl2561intmed;
        configuresensor(tsl2561gain, tsl2561int);
        break;
    } // end switch (tsl2561gain)
  } // end if (full > 30000)

  lum = myTSL2561.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  lux = myTSL2561.calculateLux(full, ir);

  // Unihedron FAQ - a reading of greater than 22.0 is unlikely to be recorded and the darkest we've personally experienced is 21.80
  // tsl2561 has min reading of .1 give max sqm of 15.83
  if ( lux == 0 )           // avoid 0
  {
    lux = 0.1;
    // code suggestion by Jozef R to improve accuracy
    myTSL2561.begin();
    tsl2561gain = tsl2561gainhigh;
    tsl2561int = tsl2561intmed;
    myTSL2561.setGain((tsl2561Gain_t)tsl2561gain);                // set 16x gain (for dim situations)
    myTSL2561.setTiming((tsl2561IntegrationTime_t)tsl2561int);
  }
#endif // #if defined(TSL2561SENSOR) 

#if !defined(TSL2561SENSOR)
  static int ldrRawData;
  static float resistorVoltage, ldrVoltage, ldrResistance;
  // portions of code from https://www.digikey.com/en/maker/projects/design-a-luxmeter-with-an-ldr-and-an-arduino/623aeee0f93e427bb57e02c4592567d1
  ldrRawData = analogRead(LDRpin);                  // Read the analogue pin, returns value 0-1023
  resistorVoltage = (float)ldrRawData / 1023 * 5;
  ldrVoltage = 5.0 - resistorVoltage;
  ldrResistance = ldrVoltage / resistorVoltage * 10000; // Resistor is 10 kohm
  lux = (double)12518931 * pow(ldrResistance, -1.405);
#endif // #if !defined(TSL2561SENSOR)
}

void setup()
{
  Serial.begin(SerialPortSpeed);
  clearSerialPort();

  pinMode( rs_DO, INPUT );                    // rain sensor

  
pinMode(particleSensorPin,INPUT);
starttime = millis();//get the current time;


  mySQMreading = 0.0;                         // the SQM value, sky magnitude
  frequency = irradiance = nelm = 1.0;
  setpoint1 = 22;                             // setpoint values used to determine sky state
  setpoint2 = 2;
  dewpoint = 10.0;
  temperature = 20.0;
  humidity = 50.0;
  pressure = 1100;
  skyambient = 20.0;
  skyobject = 20.0;
  lux = 1000;
  skystate = SKYCLEAR;
  raining = false;
  rainVout = 0;

#ifndef SQMSUPPORT
  line = "";
#endif

#ifdef LCDDISPLAY
  initlcd();
#endif

#ifdef OLEDDISPLAY
  initoled();
#endif

#ifdef BME280SENSOR
  mybme280.begin(BME280_I2CADDR);  //begin for I2C
  mybme280.init();
#endif

#ifdef MLS90614SENSOR
  mymlx90614.begin();
#endif

#ifdef TSL2561SENSOR
  myTSL2561.begin();
  tsl2561gain = tsl2561gainhigh;
  tsl2561int = tsl2561intmed;
  myTSL2561.setGain((tsl2561Gain_t)tsl2561gain);                // set 16x gain (for dim situations)
  myTSL2561.setTiming((tsl2561IntegrationTime_t)tsl2561int);    // shortest integration time (bright light)
#endif  // #ifdef TSL2561SENSOR

  mydate = "00/00/0000";
  mytime = "00:00:00";
  mylatitude = "0";
  mylongitude = "0";
  myaltitude = "0";
  mysatellites = "0";
  myfix = "0";

}

void loop()
{



    duration = pulseIn(particleSensorPin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;

    if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
                Serial.print(lowpulseoccupancy);
        Serial.print(",");
        Serial.print(ratio);
        Serial.print(",");
        Serial.println(concentration);
        lowpulseoccupancy = 0;
        starttime = millis();
    }


    

  
  static byte machinestate = state_lux;
  static unsigned long luxtimestamp = millis();
  static unsigned long sqmtimestamp = millis();

#if defined(MLX90614SENSOR)
  static unsigned long mlxtimestamp = millis();
#endif
#if defined(LCDDISPLAY) || defined(OLEDDISPLAY)
  static unsigned long displaytimestamp = millis();
#endif
#if defined(BME280SENSOR)
  static unsigned long bmetimestamp = millis();
#endif
#if defined(RAINSENSOR)
  static unsigned long raintimestamp = millis();
#endif

#ifdef USELUXMETHOD
  static double totalluxreading = 0.0;
  static int    luxcount = 1;
#else
  static int    LDRval = 0;
  static double totalfrequency = 0.0;
  static double totalirradiance = 0.0;
  static int    count = 1;
#endif

#ifdef LOOPTIMETEST
  DebugPrint(F(LOOPSTARTSTR));
  DebugPrintln(millis());
#endif

#ifndef SQMSUPPORT
  if ( queue.count() >= 1 )             // check for serial command
  {
    processCommand();
  }
#endif  // #ifndef SQMSUPPORT


  switch ( machinestate )
  {


    
    case state_lux:
      if (timecheck(luxtimestamp, LUXUPDATETIME))   // check the bme sensor
      {
        luxtimestamp = millis();
        getlux();
      }
      machinestate = state_sqm;
      break;

    case state_sqm:
      if (timecheck(sqmtimestamp, SQMUPDATETIME))   // check the sqm sensor
      {
        sqmtimestamp = millis();

        // refresh sqm readings
        LDRval = analogRead( LDRpin );            // read LDR to determine background light level
        if ( LDRval < LDRCutoff1 )                // and use this to set the Gate Time (duration) for this samples frequency measurement
        {
          period = blackperiod;                   // its very dark
        }
        else if ( LDRval < LDRCutoff2 )
        {
          period = darkperiod;                    // its dark
        }
        else
        {
          period = lightperiod;                   // its light
        }
        light();                                  // read frequency from sensor - this taks some time to process
#ifdef TSL235
        frequency = frequency * 7.2;
#endif
        irradiance = frequency / 2.3e3;           // calculate irradiance as uW/(cm^2)

#ifndef RUNNINGAVERAGE
        // use instant reading
#ifdef OLDSQMMETHOD
        mySQMreading = ((sqm_limit - (2.5 * log10( (frequency * 1.584) / 2.0 )))) + 1.0; // frequency to magnitudes/arcSecond^2
#endif
#ifdef NEWSQMMETHOD
        mySQMreading = sqm_limit - 2.5 * log10 (frequency);
#endif
#ifdef NEWSQMMETHODCORRECTED
        // 0.973 was derived by comparing TLS237 sensor readings against Unihedron and plotting on graph then deriving coefficient
        mySQMreading = (sqm_limit - (2.5 * log10( frequency )) * 0.973); // frequency to magnitudes/arcSecond2
#endif
#ifdef IRRADIANCEMETHOD
        mySQMreading = log10((irradiance / 6.83) / 108000) / -0.4;       // mag/arcsec^2
#endif
        if ( isnan(mySQMreading))
        {
          mySQMreading = 0.0;    // avoid nan and negative values
        }
        if ( mySQMreading == -0.0)
        {
          mySQMreading = 0.0;
        }
#else
        // use average reading
        totalfrequency += frequency;
        totalirradiance += irradiance;
        count++;
        if ( count > SAMPLES )
        {
          frequency = totalfrequency / SAMPLES;
          irradiance = totalirradiance / SAMPLES;
          totalfrequency = 0.0;
          totalirradiance = 0.0;
          count = 1;
#ifdef OLDSQMMETHOD
          mySQMreading = ((sqm_limit - (2.5 * log10( (frequency * 1.584) / 2.0 )))) + 1.0; // frequency to magnitudes/arcSecond^2
#endif
#ifdef NEWSQMMETHOD
          mySQMreading = sqm_limit - 2.5 * log10 (frequency);
#endif
#ifdef NEWSQMMETHODCORRECTED
          // 0.973 was derived by comparing TLS237 sensor readings against Unihedron and plotting on graph then deriving coefficient
          mySQMreading = (sqm_limit - (2.5 * log10( frequency )) * 0.973); // frequency to magnitudes/arcSecond2
#endif
#ifdef IRRADIANCEMETHOD
          mySQMreading = log10((irradiance / 6.83) / 108000) / -0.4;       // mag/arcsec^2
#endif
        }
#endif // #ifndef RUNNINGAVERAGE
        if ( isnan(mySQMreading))
        {
          mySQMreading = 0.0;    // avoid nan and negative values
        }
        if ( mySQMreading == -0.0)
        {
          mySQMreading = 0.0;
        }
        nelm = 7.93 - 5.0 * log10((pow(10, (4.316 - (mySQMreading / 5.0))) + 1));
      }
      machinestate = state_rainsensor;
      break;

    case state_rainsensor:
#if defined(RAINSENSOR)
      if (timecheck(raintimestamp, RAINUPDATETIME))   // check the rain sensor
      {
        raintimestamp = millis();
        getraining();
      }
#endif
      machinestate = state_bmesensor;
      break;

    case state_bmesensor:
#if defined(BME280SENSOR)
      if (timecheck(bmetimestamp, BMEUPDATETIME))   // check the bme sensor
      {
        bmetimestamp = millis();
        mybme280.readData();
        mybme280.getValues(&temperature, &humidity, &pressure);
        calc_dewpoint((float) temperature, (float) humidity);
      }
#endif
      machinestate = state_mlxsensor;
      break;

    case state_mlxsensor:
#if defined(MLX90614SENSOR)
      if (timecheck(mlxtimestamp, MLXUPDATETIME))   // check the mlx sensor
      {
        mlxtimestamp = millis();
        skyambient = mymlx90614.readAmbientTempC();
        skyobject = mymlx90614.readObjectTempC();
        getskystate();    // update skystate
      }
#endif
      machinestate = state_displayupdate;
      break;

    case state_displayupdate:
#if defined(LCDDISPLAY) || defined(OLEDDISPLAY) // see if the display needs updating
      if (timecheck(displaytimestamp, LCDUPDATETIME))   // see if the display needs updating
      {
        displaytimestamp = millis(); // update the timestamp
#ifdef LCDDISPLAY
        lcd.clear();
#ifdef LCD1602
        showpage1602();
#endif // LCD1602
#ifdef LCD1604
        showpage1604();
#endif // LCD1604
#ifdef LCD2004
        showpage2004();
#endif  // #ifdef LCDDISPLAY
#endif // LCDDISPLAY
#ifdef OLEDDISPLAY
        showpageoled();
#endif // OLEDDISPLAY
      }
#endif // if defined(LCDDISPLAY) || defined(OLEDDISPLAY)
      machinestate = state_lux;
      break;

    default:
      machinestate = state_lux;
      break;
  }

#ifdef LOOPTIMETEST
  DebugPrint(F(LOOPENDSTR));
  DebugPrintln(millis());
#endif
}

#ifndef USELUXMETHOD
void light()
{
  long  pulses = 1L;

  FreqCounter::f_comp = 0;      // Calc Value - calibrate with professional Freq Counter
  FreqCounter::start(period);   // Gate Time
  while (FreqCounter::f_ready == 0)
  {
    pulses = FreqCounter::f_freq;
  }
  delay(20);
  frequency = ((double)pulses * (double)1000.0) / (double) period;
  // if cannot measure the number of pulses set to freq 1hz which is mag 21.95
  if ( frequency < 1.0 )
  {
    frequency = 1.0;
  }
}
#endif // #ifndef USELUXMETHOD

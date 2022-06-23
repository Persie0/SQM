// ------------------------------------------------------------------------------------
// mySQMPRO a DIY Arduino Nano Project
// Sky Quality Meter, Cloud Sensor, Rain Sensor, Barometric Sensor, GPS, Lux Sensor
// (c) Copyright Robert Brown 2014-2021. All Rights Reserved.
//
// ------------------------------------------------------------------------------------
// DISPLAY CODE
// ------------------------------------------------------------------------------------

#include <Arduino.h>

#ifndef mydisplays_h
#define mydisplays_h

#ifdef LCDDISPLAY
int lcdpage;                              // which page to display
#endif

#ifdef OLEDDISPLAY
int lcdpage;                              // which page to display
#endif

#ifdef LCDDISPLAY
void initlcd(void)
{
#ifdef LCD1602
  lcd.begin(16, 2);
#endif
#ifdef LCD1604
  lcd.begin(16, 4);
#endif
#ifdef LCD2004
  lcd.begin(20, 4);
#endif
  lcdpage = 1;
  lcd.setBacklight(HIGH);
  lcd.print(programName);
  lcd.setCursor(0, 1);
  lcd.print("VER: ");
  lcd.print(programVersion);
  lcd.setCursor(0, 2);
  lcd.print(copywritestr);
}
#endif

#ifdef OLEDDISPLAY
void initoled(void)
{
  lcdpage = 1;
  Wire.begin();
  myoled.begin(&Adafruit128x64, OLEDADDRESS);
  myoled.set400kHz();
  myoled.setFont(Adafruit5x7);
  myoled.clear();                              // clrscr OLED
  myoled.Display_Normal();                     // black on white
  myoled.Display_On();                         // display ON
  myoled.Display_Rotate(0);                    // portrait, not rotated
  myoled.Display_Bright();
  // The screen size is 128 x 64, so using characters at 6x8 this gives 21chars across and 8 lines down
  myoled.println(programName);
  myoled.println(programVersion);
  myoled.InverseCharOn();
  myoled.println(copywritestr);
  myoled.InverseCharOff();
}
#endif

#ifdef OLEDDISPLAY
// change LC to UC as suggested by Jozef R
void showpageoled()
{
  char sqmstr[] = "SQM";
  // can only be called if displayenabled is true
  myoled.clear();
  switch ( lcdpage )
  {
    case 1:
      myoled.set2X();
      myoled.println(sqmstr);
      myoled.println(mySQMreading, 2);
      // sky state, code suggestion by Jozef R
      myoled.println(' ');
      switch ( skystate )             
      {
        case SKYCLEAR:
          myoled.println("CLEAR");
          break;
        case SKYPCLOUDY:
          myoled.println("PART CLOUDY");
          break;
        case SKYCLOUDY:
          myoled.println("CLOUDY");
          break;
        default:
          myoled.println("UNKNOWN");
          break;
      }
      lcdpage = 2;
      myoled.set1X(); 
      break;

    case 2:

      
      lcdpage = 3;
      break;

    case 3:
      myoled.print("NELM = ");
      myoled.println(nelm);

      // ir sensor
      myoled.print("IRAMB= ");
      myoled.println(skyambient, 2);
      myoled.print("IROBJ= ");
      myoled.println(skyobject, 2);

      // LUX
      myoled.print("LUX  = ");
      myoled.println(lux);

      // raining
      myoled.print("RAIN?= ");
      if ( raining == false )
      {
        myoled.println("NO");
      }
      else
      {
        myoled.println("YES");
      }

      // raining Vout, resolution accuracy is 4.9 mV
      myoled.print("RVOUT: ");
      myoled.print(volts, 2);

      // setpoint1 and 2
      myoled.print("SP1  = ");
      myoled.println(setpoint1);
      myoled.print("SP2  = ");
      myoled.println(setpoint2);
      lcdpage = 4;
      break;
    case 4:
      // sky state
      switch ( skystate )
      {
        case SKYCLEAR:
          myoled.println("CLEAR");
          break;
        case SKYPCLOUDY:
          myoled.println("PARTLY CLOUDY");
          break;
        case SKYCLOUDY:
          myoled.println("CLOUDY");
          break;
        default:
          myoled.println("UNKNOWN");
          break;
      }
      // bme280 data, ambient, humidity, dewpoint, barometric pressure
      myoled.print("HUMIDITY: ");
      myoled.println(humidity);
      myoled.print("AMBIENT : ");
      myoled.println(temperature, 2);
      myoled.print("DEWPOINT: ");
      myoled.println(dewpoint, 2);
      myoled.print("PRESSURE: ");
      myoled.println(pressure);
      lcdpage = 1;
      break;
  }
}
#endif

#ifdef LCDDISPLAY
#ifdef LCD2004
void showpage2004()
{
  switch ( lcdpage )
  {
    case 1:
      lcd.print("SQM : ");
      lcd.print(mySQMreading);
      lcd.setCursor(0, 1);
      lcd.print("NELM: ");
      lcd.print(nelm);
#ifdef useGPSNEO
      lcd.setCursor(0, 2);
      lcd.print("DATE: ");
      lcd.print(mydate);
      lcd.setCursor(0, 3);
      lcd.print("TIME: ");
      lcd.print(mytime);
#endif //change
/*      lcdpage = 2;
      break;
    case 2:
#ifdef useGPSNEO
      // latitude and longitude
      lcd.print("LAT: ");
      lcd.print(mylatitude);
      lcd.setCursor(0, 1);
      lcd.print("LON: ");
      lcd.print(mylongitude);

      lcd.setCursor(0, 2);
      // satellites
      lcd.print("SAT: ");
      lcd.print(mysatellites);
      lcd.setCursor(0, 3);
      lcd.print("ALT: ");
      lcd.print((myaltitude);
#else
      lcd.print("NO GPS");
#endif
cahnge */      lcdpage = 3;
      break;
    case 3:
      lcd.print("IRAMB: ");
      lcd.print(skyambient, 2);
      lcd.setCursor(0, 1);
      lcd.print("IROBJ: ");
      lcd.print(skyobject, 2);
      lcd.setCursor(0, 2);
      // raining
      lcd.print("RAIN?: ");
      if ( raining == false )
      {
        lcd.print("NO");
      }
      else
      {
        lcd.print("YES");
      }
      lcd.setCursor(0, 3);
      lcd.print("RVOUT: ");
      lcd.print(volts, 2);
      lcdpage = 4;
      break;
    case 4:
      // LUX
      lcd.print("LUX: " );
      lcd.print(lux, 1);
      lcd.setCursor(0, 1);
      lcd.print("SKY: ");
      // Sky state
      switch ( skystate )
      {
        case SKYCLEAR:
          lcd.print("CLEAR");
          break;
        case SKYPCLOUDY:
          lcd.print("PART CLOUDY");
          break;
        case SKYCLOUDY:
          lcd.print("CLOUDY");
          break;
        default:
          lcd.print("UNKNOWN");
          break;
      }
      // setpoint1 and 2
      lcd.setCursor(0, 2);
      lcd.print("SP1: ");
      lcd.print(setpoint1);
      lcd.setCursor(0, 3);
      lcd.print("SP2: ");
      lcd.print(setpoint2);
      lcdpage = 5;
      break;
    case 5:
      // staub & blitz sensor

      
      lcd.print("STAUB: ");
      lcd.setCursor(0, 1);
      lcd.print("ratio: ");
      lcd.print(ratio, 2);
      lcd.setCursor(0, 2);
      lcd.print("concentr: ");
      lcd.print(concentration, 2);
      lcd.setCursor(0, 3);

      lcdpage = 1;
      break;
  }
}
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1604
void showpage1604()
{
  switch ( lcdpage )
  {
    case 1:
      lcd.print("SQM : ");
      lcd.print(mySQMreading);
      lcd.setCursor(0, 1);
      lcd.print("NELM: ");
      lcd.print(nelm);
#ifdef useGPSNEO
      lcd.setCursor(0, 2);
      lcd.print("DATE: ");
      lcd.print(mydate);
      lcd.setCursor(0, 3);
      lcd.print("TIME: ");
      lcd.print(mytime);
#endif
      lcdpage = 2;
      break;
    case 2:
#ifdef useGPSNEO
      // latitude and longitude
      lcd.print("LAT: ");
      lcd.print(mylatitude);
      lcd.setCursor(0, 1);
      lcd.print("LON: ");
      lcd.print(mylongitude);
      lcd.setCursor(0, 2);

      // satellites
      lcd.print("SAT: ");
      lcd.print(mysatellites);
      lcd.setCursor(0, 3);
      lcd.print("ALT: ");
      lcd.print(myaltitude);
#else
      lcd.print("NO GPS");
#endif
      lcdpage = 3;
      break;
    case 3:
      lcd.print("IRAMB: ");
      lcd.print(skyambient, 2);
      lcd.setCursor(0, 1);
      lcd.print("IROBJ: ");
      lcd.print(skyobject, 2);
      lcd.setCursor(0, 2);
      // raining
      lcd.print("RAIN?: ");
      if ( raining == false )
      {
        lcd.print("NO");
      }
      else
      {
        lcd.print("YES");
      }
      lcd.setCursor(0, 3);
      lcd.print("RVOUT: ");
      lcd.print(volts, 2);
      lcdpage = 4;
      break;
    case 4:
      // LUX
      lcd.print("LUX: " );
      lcd.print(lux, 1);
      lcd.setCursor(0, 1);
      lcd.print("SKY: ");
      // Sky state
      switch ( skystate )
      {
        case SKYCLEAR:
          lcd.print("CLEAR");
          break;
        case SKYPCLOUDY:
          lcd.print("PCLOUDY");
          break;
        case SKYCLOUDY:
          lcd.print("CLOUDY");
          break;
        default:
          lcd.print("UNKNOWN");
          break;
      }
      // setpoint1 and 2
      lcd.setCursor(0, 2);
      lcd.print("SP1: ");
      lcd.print(setpoint1);
      lcd.setCursor(0, 3);
      lcd.print("SP2: ");
      lcd.print(setpoint2);
      lcdpage = 5;
      break;
    case 5:
      // bme280 data, ambient, humidity, dewpoint, barometric pressure
      lcd.print("HUM: ");
      lcd.print(humidity, 2);
      lcd.setCursor(0, 1);
      lcd.print("AMB: ");
      lcd.print(temperature, 2);
      lcd.setCursor(0, 2);
      lcd.print("DEW: ");
      lcd.print(dewpoint);
      lcd.setCursor(0, 3);
      lcd.print("PRE: ");
      lcd.print(pressure);
      lcdpage = 1;
      break;
  }
}
#endif
#endif

#ifdef LCDDISPLAY
#ifdef LCD1602
void showpage1602()
{
  switch ( lcdpage )
  {
    case 1:
      lcd.print("SQM : ");
      lcd.print(mySQMreading, 2);
      lcd.setCursor(0, 1);
      lcd.print("NELM: ");
      lcd.print(nelm);
      lcdpage = 2;
      break;
    case 2:
#ifdef useGPSNEO
      lcd.print("Date: ");
      lcd.print(mydate);
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(mytime);
#endif
      lcdpage = 3;
      break;
    case 3:
#ifdef useGPSNEO
      // latitude and longitude
      lcd.print("LAT: ");
      lcd.print(mylatitude);
      lcd.setCursor(0, 1);
      lcd.print("LON: ");
      lcd.print(mylongitude);
      lcdpage = 4;
      break;
    case 4:
      // satellites
      lcd.print("SAT: ");
      lcd.print( mysatellites );
      lcd.setCursor(0, 1);
      lcd.print("ALT: ");
      lcd.print(myaltitude);
 
      lcdpage = 5;
      break;
#else
      lcd.print("NO GPS");
#endif
      lcdpage = 5;
      break;
    case 5:
      lcd.print("IRAMB: ");
      lcd.print(skyambient, 2);;
      lcd.setCursor(0, 1);
      lcd.print("IROBJ: ");
      lcd.print(skyobject, 2);
      lcdpage = 6;
      break;
    case 6:
      // raining
      lcd.print("RAIN?: ");
      if ( raining == false )
      {
        lcd.print("NO");
      }
      else
      {
        lcd.print("YES");
      }
      lcd.setCursor(0, 1);
      lcd.print("RVOUT: ");
      lcd.print(volts, 2);
      lcdpage = 7;
      break;
    case 7:
      lcd.print(lux, 1);
      lcd.setCursor(0, 1);
      lcd.print("SKY: ");
      switch ( skystate )
      {
        case SKYCLEAR:
          lcd.print("CLEAR");
          break;
        case SKYPCLOUDY:
          lcd.print("PART CLOUDY");
          break;
        case SKYCLOUDY:
          lcd.print("CLOUDY");
          break;
        default:
          lcd.print("UNKNOWN");
          break;
      }
      lcdpage = 8;
      break;
    case 8:
      // setpoint1 and 2
      lcd.print("SP1: ");
      lcd.print(setpoint1);
      lcd.setCursor(0, 1);
      lcd.print("SP2: ");
      lcd.print(setpoint2);
      lcdpage = 9;
      break;
    case 9:
      // bme280 ambient, humidity
      lcd.print("HUM: ");
      lcd.print(humidity, 2);
      lcd.setCursor(0, 1);
      lcd.print("AMB: ");
      lcd.print(temperature, 2);
      lcdpage = 10;
      break;
    case 10:
      // bme280 dewpoint, barometric pressure
      lcd.print("DEW: ");
      lcd.print(dewpoint, 2);
      lcd.setCursor(0, 1);
      lcd.print("PRE: ");
      lcd.print(pressure);
      lcdpage = 1;
      break;
  }
}
#endif
#endif

#endif

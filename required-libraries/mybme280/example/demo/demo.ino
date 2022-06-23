#include <Arduino.h>
#include "mybme280.h"

#define BME280_I2CADDR 0x76

bme280 mybme280;
double bme280humidity;
double bme280temperature;
unsigned long int bme280pressure;

void setup()
{
  mybme280.begin(BME280_I2CADDR);
  mybme280.init();
  Serial.begin(9600);
}

void loop()
{
  mybme280.readData();
  mybme280.getValues(&bme280temperature, &bme280humidity, &bme280pressure);

  Serial.print("TEMP : ");
  Serial.print(bme280temperature);
  Serial.print(" DegC  PRESS : ");
  Serial.print(bme280pressure);
  Serial.print(" hPa  HUM : ");
  Serial.print(bme280humidity);
  Serial.println(" %");

  delay(3000);
}



#ifndef mybme280_h
#define mybme280_h

#include <Arduino.h>
#include <Wire.h>

class bme280
{
  public:
    char _i2c_address;
    signed long int t_fine;
    double temp_act = 0.0, press_act = 0.0, hum_act = 0.0;
    signed long int temp_cal;
    unsigned long int press_cal, hum_cal;
    unsigned long int hum_raw, temp_raw, pres_raw;
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    int8_t  dig_H1;
    int16_t dig_H2;
    int8_t  dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t  dig_H6;

    void begin(char I2C_address);
    void init();
    signed long int calibration_T(signed long int);
    unsigned long int calibration_P(signed long int);
    unsigned long int calibration_H(signed long int);
    void readtrim();
    void writeReg(uint8_t, uint8_t);
    void readData();
    void getValues(double *, double *, unsigned long int *);

};

#endif

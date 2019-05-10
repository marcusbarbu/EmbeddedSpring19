#include "Arduino.h"
#include <Wire.h>
#include "constants.h"
#include "helpers.h"

int16_t lx, ly, lz;
int16_t ax, ay, az;

float lx_f, ly_f, lz_f;
float ax_f, ay_f, az_f;

void prettyPrint(){
  ppLIS3DH();
  ppADXL343();
  Serial.println("------------------------------------");
}

void ppLIS3DH(){
  Serial.print("L x:");
  Serial.print(lx, HEX);
  Serial.print(" y: ");
  Serial.print(ly, HEX);
  Serial.print(" z: ");
  Serial.print(lz, HEX);
  Serial.print(" xf: ");
  Serial.print(lx_f);
  Serial.print(" yf: ");
  Serial.print(ly_f);
  Serial.print(" zf: ");
  Serial.println(lz_f);
}

void ppADXL343(){
  Serial.print("A x:");
  Serial.print(ax, HEX);
  Serial.print(" y: ");
  Serial.print(ay, HEX);
  Serial.print(" z: ");
  Serial.print(az, HEX);
  Serial.print(" xf: ");
  Serial.print(ax_f);
  Serial.print(" yf: ");
  Serial.print(ay_f);
  Serial.print(" zf: ");
  Serial.println(az_f);
}

void readLIS3DH(){
  //request the entire XYZ data region
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(LIS3DH_DATA_REG_START|0x80);
  Wire.endTransmission();
  Wire.requestFrom(LIS3DH_ADDR, 6);
  lx = Wire.read();
  lx |= ((uint16_t) Wire.read()) << 8;
  ly = Wire.read();
  ly |= ((uint16_t) Wire.read()) << 8;
  lz = Wire.read();
  lz |= ((uint16_t) Wire.read()) << 8;
}

void readADXL343(){
  Wire.beginTransmission(ADXL343_ADDR);
  Wire.write(ADXL343_DATA_REG_START);
  Wire.endTransmission();
  
  Wire.requestFrom(ADXL343_ADDR, 6);
  ax = Wire.read();
  ax |= ((uint16_t) Wire.read()) << 8;
  ay = Wire.read();
  ay |= ((uint16_t) Wire.read()) << 8;
  az = Wire.read();
  az |= ((uint16_t) Wire.read()) << 8;
}

void testLIS3DH(){
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(0xF);
  Wire.endTransmission();
  Wire.requestFrom(LIS3DH_ADDR, 1);
  Serial.println(Wire.read(), HEX);
  
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(0x20);
  Wire.endTransmission();
  Wire.requestFrom(LIS3DH_ADDR, 1);
  Serial.print("CONFIG");
  Serial.println(Wire.read(), HEX);
}

void setupLIS3DH(){
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(LIS3DH_CTL_REG_START);
  Wire.write(LIS3DH_CONFIG_BYTE_0);
  Wire.endTransmission();

  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(LIS3DH_CTL_REG_START + 3);
  Wire.write(LIS3DH_CONFIG_BYTE_3);
  Wire.endTransmission();
}

void setupADXL343(){
  Wire.beginTransmission(ADXL343_ADDR);
  Wire.write(ADXL343_POWER_CTL_REG);
  Wire.write(ADXL343_POWER_CTL_BYTE);
  Wire.endTransmission();

  Wire.beginTransmission(ADXL343_ADDR);
  Wire.write(ADXL343_DATA_FMT_REG);
  Wire.write(ADXL343_DATA_FMT_BYTE);
  Wire.endTransmission();
}

float LIS3DHConversion(int16_t raw){
  return ((float) raw) / 8190 * 9.81;
}

void convertLIS3DH(){
  lx_f = LIS3DHConversion(lx);
  ly_f = LIS3DHConversion(ly);
  lz_f = LIS3DHConversion(lz);
}

float ADXL343Conversion(int16_t raw){
  return ((float) raw) * 0.004 * 9.81;
}

void convertADXL343(){
  ax_f = ADXL343Conversion(ax);
  ay_f = ADXL343Conversion(ay);
  az_f = ADXL343Conversion(az);
}
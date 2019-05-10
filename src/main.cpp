#include "Arduino.h"
#include <Wire.h>
#include "helpers.h"
#include "constants.h"

/*
Using two different types of accelerometers was a mistake.
*/

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  setupLIS3DH();
  setupADXL343();
}

void loop()
{
  readLIS3DH();
  readADXL343();
  convertLIS3DH();
  convertADXL343();
  prettyPrint();
  delay(1000);
}
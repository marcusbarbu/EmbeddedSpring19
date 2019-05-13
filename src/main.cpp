#include "Arduino.h"
#include <Wire.h>
#include "helpers.h"
#include "constants.h"

/*
Using two different types of accelerometers was a mistake.
*/

typedef volatile float * accel; 

volatile float aReadings[3];
volatile float bReadings[3];

accel aHistory[HISTSIZE];
accel bHistory[HISTSIZE];

volatile float jerkHolder[3];

volatile int loop_iter = 0;


accel computeJerkRel(accel a, accel b){
  for (int i=0; i<3; i++){ 
    jerkHolder[i] = ((float)b[i] - (float)a[i])/((float)TDELTA);
    Serial.print((float)b[i]);
    Serial.print(" - ");
    Serial.print((float)a[i]); 
    Serial.print("    ");
    Serial.print(jerkHolder[i]);
    Serial.print("    ");
  }
  return jerkHolder;
}

float computeJerkAbs(accel a, accel b){
  //Serial.println("computing rel abs");
  accel c = computeJerkRel(a, b);
  float res = sqrt((c[0]*c[0]) + (c[1]*c[1]) + (c[2]*c[2]));
  return res;
}

void testHistory(accel* hist){
  //Serial.println("testing");
  float j = computeJerkAbs(hist[HISTSIZE], hist[HISTSIZE-1]);
  Serial.println(j); 
  if (j > FALL_JERK){
    Serial.println("Falling?");
  }
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  setupLIS3DH();
  setupADXL343();
  delay(200);
}

void loop()
{
  readLIS3DH();
  readADXL343();
 
  convertLIS3DH();
  convertADXL343();
  prettyPrint();
  
  aReadings[0] = ax_f;
  aReadings[1] = ay_f;
  aReadings[2] = az_f;
  
  bReadings[0] = lx_f;
  bReadings[1] = ly_f;
  bReadings[2] = lz_f;

  testHistory[]

  testHistory(aHistory);
  delay(TDELTA);
  
  loop_iter++;
}


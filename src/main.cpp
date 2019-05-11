#include "Arduino.h"
#include <Wire.h>
#include "helpers.h"
#include "constants.h"

/*
Using two different types of accelerometers was a mistake.
*/

typedef float* accel; 

float aReadings[3];
float bReadings[3];

accel aHistory[HISTSIZE];
accel bHistory[HISTSIZE];


accel computeJerkRel(accel a, accel b){
  accel final = (float*) malloc(sizeof(float)*3);
  for (int i=0; i<3; i++){ 
    final[i] = (b[i] - a[i])/(2*TDELTA); //this can be refined once we know how long things really take
  }
  return final;
}

float computeJerkAbs(accel a, accel b){
  accel c = computeJerkRel(a, b);
  float res;
  res = sqrt((c[0]*c[0]) + (c[1]*c[1]) + (c[2]*c[2]));
  free(c);
  return res;
}

void testHistory(accel* hist){
  Serial.println("testing");
  float j = computeJerkAbs(hist[HISTSIZE], hist[0]);
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
}

void loop()
{
  readLIS3DH();
  readADXL343();
 
  convertLIS3DH();
  convertADXL343();
  //prettyPrint();
  
  aReadings[0] = ax_f;
  aReadings[1] = ay_f;
  aReadings[2] = az_f;
  
  bReadings[0] = lx_f;
  bReadings[1] = ly_f;
  bReadings[2] = lz_f;

  //testHistory(aHistory);
  delay(TDELTA);
}


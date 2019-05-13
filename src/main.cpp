#include "Arduino.h"
#include <Wire.h>
#include "helpers.h"
#include "constants.h"

/*
Using two different types of accelerometers was a mistake.
*/
struct accel {
  float x;
  float y;
  float z;
};

volatile float aReadings[3];
volatile float bReadings[3];

accel aHistory[HISTSIZE];
accel bHistory[HISTSIZE];

volatile int loop_iter = 0;

void printAccel(accel a){
  Serial.print("x: ");
  Serial.print(a.x);
  Serial.print(" y: ");
  Serial.print(a.y);
  Serial.print(" z: ");
  Serial.println(a.z);
}
void printCSV(accel a){
  Serial.print(a.x);
  Serial.print(",");
  Serial.print(a.y);
  Serial.print(",");
  Serial.print(a.z);
  Serial.println(",");
}

accel computeJerkRel(accel a,  accel b){
  accel jerkHolder = {0.0, 0.0, 0.0};
  jerkHolder.x = (a.x - b.x) / ((float)TDELTA * 0.001);
  jerkHolder.y = (a.y - b.y) / ((float)TDELTA * 0.001);
  jerkHolder.z = (a.z - b.z) / ((float)TDELTA * 0.001);

  return jerkHolder;
}

float computeJerkAbs(accel a, accel b){
  accel c = computeJerkRel(a, b);
  //printAccel(c);
  float res = sqrt((c.x*c.x) + (c.y*c.y) + (c.z*c.z)) / ((float) TDELTA);
  return res;
}

void testHistory(accel* hist){
  //float j = computeJerkAbs(hist[iteration % HISTSIZE], hist[iteration-1 % HISTSIZE]);
  float jerk_cumulative = 0.0;
  float j; 
  for (int i=1; i < HISTSIZE; i++){
    j = computeJerkAbs(hist[i], hist[i-1]);
    jerk_cumulative += j;
  } 
  jerk_cumulative /= (HISTSIZE-1);
  //Serial.println(jerk_cumulative);
  digitalWrite(LED_BUILTIN, LOW);
  if (jerk_cumulative > FALL_JERK){
    Serial.println("Falling?");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  setupLIS3DH();
  setupADXL343();
  delay(200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  /*if (loop_iter % HISTSIZE == 0){
    Serial.println("HISTORY INBOUND ======================");
    for (int i=0; i < HISTSIZE; i++){
      printCSV(aHistory[i]);
    }
    Serial.println("THAT WAS HISTORY =====================");
  } */
  readLIS3DH();
  readADXL343();
 
  convertLIS3DH();
  convertADXL343();

  aHistory[loop_iter % HISTSIZE].x = ax_f;
  aHistory[loop_iter % HISTSIZE].y = ay_f;
  aHistory[loop_iter % HISTSIZE].z = az_f;
  
  bHistory[loop_iter % HISTSIZE].x = lx_f;
  bHistory[loop_iter % HISTSIZE].y = ly_f;
  bHistory[loop_iter % HISTSIZE].z = lz_f;

  if (loop_iter > 0 && loop_iter % HISTSIZE == 0) testHistory(aHistory);
  delay(TDELTA);
  loop_iter++;
  //Serial.println(loop_iter);
}


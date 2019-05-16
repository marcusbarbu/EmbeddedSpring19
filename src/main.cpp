#include "Arduino.h"
#include <Wire.h>
#include "helpers.h"
#include "constants.h"

/*
Using two different types of accelerometers was a mistake.
*/

volatile float aReadings[3];
volatile float bReadings[3];

accel aHistory[HISTSIZE];
accel bHistory[HISTSIZE];

volatile int loop_iter = 0;
volatile float jerk_history_a = 0.0;
volatile float jerk_history_b = 0.0;

void printAccel(accel a);
void printCSV(accel a);
accel computeJerkRel(accel a,  accel b);
float computeJerkAbs(accel a, accel b);
float testHistory(accel* hist);

void setup()
{
  Wire.begin();
  Serial.begin(115200); //for debugging
  setupLIS3DH(); //initialize sensor modes
  setupADXL343();
  pinMode(LED_BUILTIN, OUTPUT); // for signaling
  delay(200);
}

void loop()
{
  //read the raw, unscaled sensor data
  readLIS3DH();
  readADXL343();
  
  //convert everything to m/s^2
  convertLIS3DH();
  convertADXL343();

  //pack everything appropriately
  aHistory[loop_iter % HISTSIZE].x = ax_f;
  aHistory[loop_iter % HISTSIZE].y = ay_f;
  aHistory[loop_iter % HISTSIZE].z = az_f;
  
  bHistory[loop_iter % HISTSIZE].x = lx_f;
  bHistory[loop_iter % HISTSIZE].y = ly_f;
  bHistory[loop_iter % HISTSIZE].z = lz_f;

  //use a HISTSIZE long sliding window for calculations
  if (loop_iter > 0 && loop_iter % HISTSIZE == 0){
    digitalWrite(LED_BUILTIN, LOW); //turn the LED off if we need to
    jerk_history_a += testHistory(aHistory);
    jerk_history_b += testHistory(bHistory);
    if (loop_iter % CUM_SIZE == 0){ //do a test every CUMSIZE loops
      //take the average jerk for each sensor
      jerk_history_a /= CUM_SIZE; 
      jerk_history_b /= CUM_SIZE; 
      Serial.print("Jerk A: ");
      Serial.print(jerk_history_a);
      Serial.print("  Jerk B: ");
      Serial.println(jerk_history_b);
      //if both sensors experience high jerk we're probably falling
      if (jerk_history_a > CUMULATIVE_JERK_THRESHOLD && jerk_history_b > CUMULATIVE_JERK_THRESHOLD)
      {
        digitalWrite(LED_BUILTIN, HIGH);
      }

      jerk_history_a = 0.0;
      jerk_history_b = 0.0;
    }
  }
  delay(TDELTA);
  loop_iter++;
}

//used for debugging
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

//compute each component of jerk
accel computeJerkRel(accel a,  accel b){
  accel jerkHolder = {0.0, 0.0, 0.0};
  jerkHolder.x = (a.x - b.x) / ((float)TDELTA * 0.001);
  jerkHolder.y = (a.y - b.y) / ((float)TDELTA * 0.001);
  jerkHolder.z = (a.z - b.z) / ((float)TDELTA * 0.001);

  return jerkHolder;
}

//compute the pythagorean sum of jerk
float computeJerkAbs(accel a, accel b){
  accel c = computeJerkRel(a, b);
  float res = sqrt((c.x*c.x) + (c.y*c.y) + (c.z*c.z)) / ((float) TDELTA);
  return res;
}

float testHistory(accel* hist){
  float jerk_cumulative = 0.0;
  float j; 
  for (int i=1; i < HISTSIZE; i++){
    j = computeJerkAbs(hist[i], hist[i-1]); //calculate the instantaneous jerk betweeen two readings
    jerk_cumulative += j;
  } 
  jerk_cumulative /= (HISTSIZE-1); //average the instantaneous jerk to help with noise
  return jerk_cumulative;
}
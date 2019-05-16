#ifndef HELPERS
#define HELPERS 1

extern int16_t lx, ly, lz;
extern int16_t ax, ay, az;

extern float lx_f, ly_f, lz_f;
extern float ax_f, ay_f, az_f;

struct accel {
  float x;
  float y;
  float z;
};

//LIS3DH

void readLIS3DH();
void setupLIS3DH();
void testLIS3DH(); 
void convertLIS3DH();
void ppLIS3DH();

void readADXL343();
void setupADXL343();
void convertADXL343();
void ppADXL343();

void prettyPrint();

#endif
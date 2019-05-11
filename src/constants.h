#ifndef CONSTANTS
#define CONSTANTS 1

#define TDELTA 150
#define HISTSIZE 10
#define FALL_JERK 0.2

#define LIS3DH_ADDR 0x18
#define LIS3DH_DATA_REG_START 0x28
#define LIS3DH_CTL_REG_START 0x20
#define LIS3DH_CONFIG_BYTE_0 0x77
//0x77 7x -> 400Hz x7 ->No low power mode, XYZ enabled
#define LIS3DH_CONFIG_BYTE_3 0x98
//0x98 9x -> Block update, 4G scale x8 -> High resolution, no self testing

#define ADXL343_ADDR 0x53
#define ADXL343_DATA_REG_START 0x32
#define ADXL343_POWER_CTL_REG 0x2D
#define ADXL343_POWER_CTL_BYTE 0x08

#define ADXL343_DATA_FMT_REG 0x31
#define ADXL343_DATA_FMT_BYTE 0x09
//0x09 x9 -> Full res, 4g scale
#endif
#ifndef _BMP180_H_
#define _BMP180_H_

#include <stdint.h>

#define BMP180_I2CADDR 0x77

#define BMP180_ULTRALOWPOWER 0
#define BMP180_STANDARD      1
#define BMP180_HIGHRES       2
#define BMP180_ULTRAHIGHRES  3
#define BMP180_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMP180_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMP180_CAL_AC3           0xAE  // R   Calibration data (16 bits)
#define BMP180_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMP180_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMP180_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMP180_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMP180_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMP180_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMP180_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMP180_CAL_MD            0xBE  // R   Calibration data (16 bits)

#define BMP180_CONTROL           0xF4
#define BMP180_TEMPDATA          0xF6
#define BMP180_PRESSUREDATA      0xF6
#define BMP180_READTEMPCMD       0x2E
#define BMP180_READPRESSURECMD   0x34

int oversampling;

short int ac1, ac2, ac3, b1, b2, mb, mc, md;
unsigned short int ac4, ac5, ac6;

int begin(int fd);

int computeB5(unsigned int UT);

unsigned int readRawTemperature(int fd);

uint32_t readRawPressure(int fd);

int getPressure(int fd, double* pressure);

int32_t readSealevelPressure(int fd, float altitude_meters);

int getTemperature(int fd, double* temperature);

float readAltitude(int fd, float sealevelPressure);

#endif

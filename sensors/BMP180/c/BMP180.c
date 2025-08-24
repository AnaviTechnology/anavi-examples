#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <math.h>
#include <unistd.h>

#include "BMP180.h"

int oversampling;

short int ac1, ac2, ac3, b1, b2, mb, mc, md;
unsigned short int ac4, ac5, ac6;

// Read two words from the BMP085 and supply it as a 16 bit integer
int i2cReadInt(int fd, int address);

int begin(int fd)
{
	oversampling = BMP180_ULTRAHIGHRES;

	// Check BMP180 device ID
	int id = i2c_smbus_read_byte_data(fd, 0xD0);
	if (id < 0)
	{
		perror("I2C read failed");
		return -1;
	}
	if (id != 0x55)
	{
		// Device not found
		return -1;
	}

	// Read calibration data
	ac1 = i2cReadInt(fd, BMP180_CAL_AC1);
	ac2 = i2cReadInt(fd, BMP180_CAL_AC2);
	ac3 = i2cReadInt(fd, BMP180_CAL_AC3);
	ac4 = i2cReadInt(fd, BMP180_CAL_AC4);
	ac5 = i2cReadInt(fd, BMP180_CAL_AC5);
	ac6 = i2cReadInt(fd, BMP180_CAL_AC6);

	b1 = i2cReadInt(fd, BMP180_CAL_B1);
	b2 = i2cReadInt(fd, BMP180_CAL_B2);

	mb = i2cReadInt(fd, BMP180_CAL_MB);
	mc = i2cReadInt(fd, BMP180_CAL_MC);
	md = i2cReadInt(fd, BMP180_CAL_MD);

	return 0;
}

int computeB5(unsigned int UT)
{
	int x1 = (((int)UT - (int)ac6)*(int)ac5) >> 15;
	int x2 = ((int)mc << 11)/(x1 + md);
	return x1 + x2;
}

unsigned int readRawTemperature(int fd)
{
	// Write the read temperature command to the control register
	if (i2c_smbus_write_byte_data(fd, BMP180_CONTROL, BMP180_READTEMPCMD) < 0)
	{
		perror("I2C write failed");
		return 0xFFFF;
	}
	// Wait at least 4.5ms
	usleep(4500);
	return (unsigned int)i2cReadInt(fd, BMP180_TEMPDATA);
}

uint32_t readRawPressure(int fd)
{
	// Write the read pressure command with oversampling
	if (i2c_smbus_write_byte_data(fd, BMP180_CONTROL, BMP180_READPRESSURECMD + (oversampling << 6)) < 0)
	{
		perror("I2C write failed");
		return 0xFFFFFFFF;
	}

	// Wait for conversion based on oversampling
	switch (oversampling)
	{
		case BMP180_ULTRALOWPOWER:
				usleep(5000);	 // 5 ms
				break;
		case BMP180_STANDARD:
				usleep(8000);	 // 8 ms
				break;
		case BMP180_HIGHRES:
				usleep(14000);	// 14 ms
				break;
		default: // BMP180_ULTRAHIGHRES
				usleep(26000);	// 26 ms
				break;
	}

	// Read the raw 16-bit pressure value
	int raw = i2cReadInt(fd, BMP180_PRESSUREDATA);

	// Read the third byte (LSB) of pressure
	int lsb = i2c_smbus_read_byte_data(fd, BMP180_PRESSUREDATA + 2);
	if (lsb < 0)
	{
		perror("I2C read failed");
		return 0xFFFFFFFF; // indicate error
	}

	// Combine MSB, LSB, and XLSB according to BMP180 datasheet
	raw <<= 8;
	raw |= lsb;
	raw >>= (8 - oversampling);

	return (uint32_t)raw;
}

int32_t readPressure(int fd)
{
	int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
	uint32_t B4, B7;

	UT = readRawTemperature(fd);
	UP = readRawPressure(fd);

	B5 = computeB5(UT);

	// do pressure calcs
	B6 = B5 - 4000;
	X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
	X2 = ((int32_t)ac2 * B6) >> 11;
	X3 = X1 + X2;
	B3 = ((((int32_t)ac1*4 + X3) << oversampling) + 2) / 4;

	X1 = ((int32_t)ac3 * B6) >> 13;
	X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
	B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );

	if (B7 < 0x80000000)
	{
		p = (B7 * 2) / B4;
	}
	else
	{
		p = (B7 / B4) * 2;
	}
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;

	p = p + ((X1 + X2 + (int32_t)3791)>>4);
	return p;
}

int getPressure(int fd, double* pressure)
{
	*pressure = (double) readPressure(fd) / 100;
	return 0;
}

int32_t readSealevelPressure(int fd, float altitude_meters)
{
	float pressure = readPressure(fd);
	return (int32_t)(pressure / pow(1.0-altitude_meters/44330, 5.255));
}

int getTemperature(int fd, double* temperature)
{
	unsigned int UT = readRawTemperature(fd);
	int compensate = computeB5(UT);
	int rawTemperature = ((compensate + 8)>>4);
	*temperature = ((double)rawTemperature)/10;
	return 0;
}

float readAltitude(int fd, float sealevelPressure)
{
	float pressure = readPressure(fd);
	float altitude = 44330 * (1.0 - pow(pressure /sealevelPressure,0.1903));
	return altitude;
}

// Read two words from the BMP085 and supply it as a 16 bit integer
int i2cReadInt(int fd, int reg)
{
	// Read 16-bit word from the device register
	int res = i2c_smbus_read_word_data(fd, reg);
	if (res < 0)
	{
		// return negative value on error
		return res;
	}

	// Swap bytes to match BMP085 big-endian format
	res = ((res << 8) & 0xFF00) | ((res >> 8) & 0x00FF);
	return res;
}

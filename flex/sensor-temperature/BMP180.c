#include <fcntl.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdio.h>

#include "BMP180.h"
#include "smbus.h"

const unsigned char BMP085_OVERSAMPLING_SETTING = 3;

// Open a connection to the bmp085
// Returns a file id
int begin()
{
	int fd = 0;
	char *fileName = "/dev/i2c-1";
	
	// Open port for reading and writing
	if ((fd = open(fileName, O_RDWR)) < 0)
	{
		return -1;
	}
	
	// Set the port options and set the address of the device
	if (ioctl(fd, I2C_SLAVE, BMP180_I2C_ADDRESS) < 0) 
	{					
		close(fd);
		return -1;
	}

	return fd;
}

// Read two words from the BMP085 and supply it as a 16 bit integer
__s32 i2cReadInt(int fd, __u8 address)
{
	__s32 res = i2c_smbus_read_word_data(fd, address);
	if (0 > res) 
	{
		close(fd);
		return -1;
	}

	// Convert result to 16 bits and swap bytes
	res = ((res<<8) & 0xFF00) | ((res>>8) & 0xFF);

	return res;
}

//Write a byte to the BMP085
int i2cWriteByteData(int fd, __u8 address, __u8 value)
{
	if (0 > i2c_smbus_write_byte_data(fd, address, value)) 
	{
		close(fd);
		return -1;
	}
	return 0;
}

// Read a block of data BMP085
int i2cReadBlockData(int fd, __u8 address, __u8 length, __u8 *values)
{
	if (0 > i2c_smbus_read_i2c_block_data(fd, address,length,values)) 
	{
		close(fd);
		return -1;
	}
	return 0;
}


int calibration()
{
	int fd = begin();
	if (0 > fd)
	{
		return fd;
	}
	cal.ac1 = i2cReadInt(fd,0xAA);
	if (0 > cal.ac1)
	{
		return cal.ac1;
	}
	cal.ac2 = i2cReadInt(fd,0xAC);
	cal.ac3 = i2cReadInt(fd,0xAE);
	cal.ac4 = i2cReadInt(fd,0xB0);
	cal.ac5 = i2cReadInt(fd,0xB2);
	cal.ac6 = i2cReadInt(fd,0xB4);
	cal.b1 = i2cReadInt(fd,0xB6);
	cal.b2 = i2cReadInt(fd,0xB8);
	cal.mb = i2cReadInt(fd,0xBA);
	cal.mc = i2cReadInt(fd,0xBC);
	cal.md = i2cReadInt(fd,0xBE);
	close(fd);
	return 0;
}

// Read the uncompensated temperature value
unsigned int readRawTemperature()
{
	int fd = begin();

	// Write 0x2E into Register 0xF4
	// This requests a temperature reading
	i2cWriteByteData(fd,0xF4,0x2E);
	
	// Wait at least 4.5ms
	usleep(5000);

	// Read the two byte result from address 0xF6
	unsigned int ut = i2cReadInt(fd,0xF6);

	// Close the i2c file
	close (fd);
	
	return ut;
}

// Read the uncompensated pressure value
unsigned int readRawPressure()
{
	int fd = begin();

	// Write 0x34+(BMP085_OVERSAMPLING_SETTING<<6) into register 0xF4
	// Request a pressure reading w/ oversampling setting
	i2cWriteByteData(fd,0xF4,0x34 + (BMP085_OVERSAMPLING_SETTING<<6));

	// Wait for conversion, delay time dependent on oversampling setting
	usleep((2 + (3<<BMP085_OVERSAMPLING_SETTING)) * 1000);

	// Read the three byte result from 0xF6
	// 0xF6 = MSB, 0xF7 = LSB and 0xF8 = XLSB
	__u8 values[3];
	i2cReadBlockData(fd, 0xF6, 3, values);

	unsigned int up = (((unsigned int) values[0] << 16) | ((unsigned int) values[1] << 8) | (unsigned int) values[2]) >> (8-BMP085_OVERSAMPLING_SETTING);

	// Close the i2c file
	close (fd);
	
	return up;
}

int compensateTemperature()
{
	unsigned int ut = readRawTemperature();
	int x1 = (((int)ut - (int)cal.ac6)*(int)cal.ac5) >> 15;
 	int x2 = ((int)cal.mc << 11)/(x1 + cal.md);
	return x1 + x2;
}

// Calculate pressure given uncalibrated pressure
// Value returned will be in units of Pa
int getPressure()
{
	unsigned int up = readRawPressure();

	int b6 = compensateTemperature() - 4000;
	// Calculate B3
	int x1 = (cal.b2 * (b6 * b6)>>12)>>11;
	int x2 = (cal.ac2 * b6)>>11;
	int x3 = x1 + x2;
	int b3 = (((((int) cal.ac1)*4 + x3)<<BMP085_OVERSAMPLING_SETTING) + 2)>>2;
  
	// Calculate B4
	x1 = (cal.ac3 * b6)>>13;
	x2 = (cal.b1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	unsigned int b4 = (cal.ac4 * (unsigned int)(x3 + 32768))>>15;
  
	unsigned int b7 = ((unsigned int)(up - b3) * (50000>>BMP085_OVERSAMPLING_SETTING));
	int p = (b7 < 0x80000000) ? (b7<<1)/b4 : (b7/b4)<<1;
	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	p += (x1 + x2 + 3791)>>4;
  
	return p;
}

// Calculate temperature given uncalibrated temperature
double getTemperature()
{
	// Retrieve temperature in units of 0.1 deg C
	int rawTemperature = ((compensateTemperature() + 8)>>4);  
	return ((double)rawTemperature)/10;
}

double getAltitude()
{
	double pressure = getPressure();
	// Sea level pressure: 101325.0
	return 44330.0 * (1.0 - pow(pressure / 101325.0, (1.0/5.255)));
}

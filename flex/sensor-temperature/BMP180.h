/*
Raspberry Pi Bosch BMP0180/BMP085 communication code.

This is a derivative work based on:
	John Burns (www.john.geek.nz)
	Source: https://www.john.geek.nz/2013/02/update-bosch-bmp085-source-raspberry-pi/ 
	BMP085 Extended Example Code
	by: Jim Lindblom
	SparkFun Electronics
	date: 1/18/11
	license: CC BY-SA v3.0 - http://creativecommons.org/licenses/by-sa/3.0/
	Source: http://www.sparkfun.com/tutorial/Barometric/BMP085_Example_Code.pde

Circuit detail:
	Using BMP180 Board Module

	VIN     - 	3.3V (Raspberry Pi pin 1)
	GND	-	GND  (Raspberry Pi pin 14)
	SCL 	-	SCL  (Raspberry Pi pin 5)
	SDA     - 	SDA  (Raspberry Pi pin 3)
	
	Note: Make sure BMP180/085 is connected to 3.3V NOT the 5V pin!

	Note: Change /dev/i2c-1 to /dev/i2c-0 if you are using the very first Raspberry Pi.
*/

#ifndef _BMP180_H_
#define _BMP180_H_

#define BMP180_I2C_ADDRESS 0x77

// Set default calibration values from values in the datasheet example
// After that exact values will be read from BMP180/BMP085 sensor

struct calibrate {
	short int ac1;
	short int ac2;
	short int ac3;
	unsigned short int ac4;
	unsigned short int ac5;
	unsigned short int ac6;
	short int b1;
	short int b2;
	short int mb;
	short int mc;
	short int md;
} cal;

// Calibrate BMP180/BMP085
int calibration();

// Calculate pressure in Pa
int getPressure();

// Calculate temperature in Celsius
double getTemperature();

// Calculate 
double getAltitude();

#endif

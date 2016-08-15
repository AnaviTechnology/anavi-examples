#include <unistd.h>

#include "wiringPi.h"
#include "wiringPiI2C.h"

#include "HTU21D.h"

// Get temperature
double getTemperature(int fd)
{
	unsigned char buf [4];
	wiringPiI2CWrite(fd, HTU21D_TEMP);
	delay(100);
	read(fd, buf, 3);
	unsigned int temp = (buf [0] << 8 | buf [1]) & 0xFFFC;
	// Convert sensor reading into temperature.
	// See page 14 of the datasheet
	double tSensorTemp = temp / 65536.0;
	return -46.85 + (175.72 * tSensorTemp);
}

// Get humidity
double getHumidity(int fd)
{
	unsigned char buf [4];
	wiringPiI2CWrite(fd, HTU21D_HUMID);
	delay(100);
	read(fd, buf, 3);
  	unsigned int humid = (buf [0] << 8 | buf [1]) & 0xFFFC;
	// Convert sensor reading into humidity.
	// See page 15 of the datasheet
	double tSensorHumid = humid / 65536.0;
	return -6.0 + (125.0 * tSensorHumid);
}

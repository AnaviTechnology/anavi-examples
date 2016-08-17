#include <unistd.h>

#include "wiringPi.h"
#include "wiringPiI2C.h"

#include "HTU21D.h"

// Get temperature
int getTemperature(int fd, double *temperature)
{
	unsigned char buf [4];
	if (0 < wiringPiI2CWrite(fd, HTU21D_TEMP))
	{
		return -1;
	}
	delay(100);
	if (-1 == read(fd, buf, 3))
	{
		return -1;
	}
	unsigned int temp = (buf [0] << 8 | buf [1]) & 0xFFFC;
	// Convert sensor reading into temperature.
	// See page 14 of the datasheet
	double tSensorTemp = temp / 65536.0;
	*temperature = -46.85 + (175.72 * tSensorTemp);
	return 0;
}

// Get humidity
int getHumidity(int fd, double *humidity)
{
	unsigned char buf [4];
	if (0 < wiringPiI2CWrite(fd, HTU21D_HUMID))
	{
		return -1;
	}
	delay(100);
	if (-1 == read(fd, buf, 3))
	{
		return -1;
	}
  	unsigned int humid = (buf [0] << 8 | buf [1]) & 0xFFFC;
	// Convert sensor reading into humidity.
	// See page 15 of the datasheet
	double tSensorHumid = humid / 65536.0;
	*humidity = -6.0 + (125.0 * tSensorHumid);
	return 0;
}

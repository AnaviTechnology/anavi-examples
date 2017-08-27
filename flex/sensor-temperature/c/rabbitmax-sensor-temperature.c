#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string.h>

#include "BMP180.h"

int main()
{
	int fd = wiringPiI2CSetup(BMP180_I2CADDR);
	if (0 > fd)
	{
		fprintf(stderr, "ERROR: Unable to access RabbitMax temperature sensor: %s\n", strerror (errno));
		exit(-1);
	}
	if (0 > begin(fd))
	{
		fprintf(stderr, "ERROR: RabbitMax temperature sensor not found\n");
		exit(-1);
	}

	double temperature = 0;
	getTemperature(fd, &temperature);

	double pressure = 0;
	getPressure(fd, &pressure);

	printf("RabbitMax Temperature and Barometric Pressure Sensor\n");
	printf("Temperature\t%0.1f C\n", temperature);
	printf("Pressure\t%0.2f hPa\n", pressure);
	return 0;
}

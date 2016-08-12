#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "BMP180.h"

int main()
{
	int fd = wiringPiI2CSetup(BMP180_I2CADDR);
	if (0 > fd)
	{
		fprintf(stderr, "ERROR: Unable to access RabbitMax light sensor: %s\n", strerror (errno));
		exit(-1);
	}
	begin(fd);

	double temperature = readTemperature(fd);
	double pressure = (double)readPressure(fd) /100;
	float altitude = readAltitude(fd, 101325);

	printf("RabbitMax Temperature and Barometric Pressure Sensor\n");
	printf("Temperature\t%0.1f C\n", temperature);
	printf("Pressure\t%0.2f hPa\n", pressure);
	printf("Altitude\t%0.2f m\n", altitude);
	return 0;
}

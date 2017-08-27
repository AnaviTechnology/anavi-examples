#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string.h>

#include "HTU21D.h"

int main()
{
	wiringPiSetup();
	int fd = wiringPiI2CSetup(HTU21D_I2C_ADDR);
	if ( 0 > fd )
	{
		fprintf(stderr, "ERROR: Unable to access RabbitMax humidity sensor: %s\n", strerror (errno));
		exit (-1);
	}

	// Retrieve temperature and humidity
	double temperature = 0;
	double humidity = 0;
	if ( (0 > getHumidity(fd, &humidity)) || (0 > getTemperature(fd, &temperature)) )
	{
		fprintf(stderr, "ERROR: RabbitMax humidity sensor not found\n");
		exit(-1);
	}
	
	// Print temperature and humidity on the screen
	printf("RabbitMax Humidity and Temperature Sensor\n");
	printf("%5.2fC\n", temperature);
	printf("%5.2f%%rh\n", humidity);
	
	return 0;
}

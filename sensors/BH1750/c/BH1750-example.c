#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>

#include "BH1750.h"

int main()
{
	int fd = wiringPiI2CSetup(BH1750_ADDR);
	if (0 > fd)
	{
		fprintf(stderr, "ERROR: Unable to access BH1750 sensor module: %s\n", strerror (errno));
		exit(-1);
	}

	int lux = getLux(fd);
	if (0 > lux)
	{
		fprintf(stderr, "ERROR: BH1750 sensor module not found\n");
		exit(-1);
	}

	printf("BH1750 Sensor Module\n");
	printf("Light: %d Lux\n", lux);
	return 0;
}

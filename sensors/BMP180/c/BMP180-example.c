#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "BMP180.h"

int main()
{
	const char *i2cDevice = "/dev/i2c-1";  // Change if using a different I2C bus
	int fd = open(i2cDevice, O_RDWR);
	if (fd < 0)
	{
		fprintf(stderr, "ERROR: Unable to open I2C device %s: %s\n", i2cDevice, strerror(errno));
		exit(-1);
	}

	if (ioctl(fd, I2C_SLAVE, BMP180_I2CADDR) < 0)
	{
		fprintf(stderr, "ERROR: Unable to set I2C address 0x%X: %s\n", BMP180_I2CADDR, strerror(errno));
		close(fd);
		exit(-1);
	}

	if (begin(fd) < 0)
	{
		fprintf(stderr, "ERROR: BMP180 sensor module not found\n");
		close(fd);
		exit(-1);
	}

	double temperature = 0;
	if (getTemperature(fd, &temperature) < 0)
	{
		fprintf(stderr, "ERROR: Failed to read temperature\n");
		close(fd);
		exit(-1);
	}

	double pressure = 0;
	if (getPressure(fd, &pressure) < 0)
	{
		fprintf(stderr, "ERROR: Failed to read pressure\n");
		close(fd);
		exit(-1);
	}

	printf("BMP180 Sensor Module\n");
	printf("Temperature\t%0.1f C\n", temperature);
	printf("Pressure\t%0.2f hPa\n", pressure);

	close(fd);
	return 0;
}

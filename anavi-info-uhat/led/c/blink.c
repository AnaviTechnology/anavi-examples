#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pigpio.h>

//Pin 29 on Raspberry Pi corresponds to BCM GPIO 5
#define D1 5
//Pin 31 on Raspberry Pi corresponds to BCM GPIO 6
#define D2 6

void terminate()
{
	gpioWrite(5, 0);
	gpioWrite(6, 0);

	//Stop DMA, release resources
	gpioTerminate();
	exit(0);
}

int main(int argc, char *argv[])
{
	if (0 > gpioInitialise())
	{
		fprintf(stderr, "pigpio initialisation failed\n");
		return 1;
	}

	// Set GPIOs to output modes
	gpioSetMode(5, PI_OUTPUT);
	gpioSetMode(6, PI_OUTPUT);

	signal(SIGINT, terminate);

	while(1)
	{
		gpioWrite(5, 1);
		gpioWrite(6, 0);
		time_sleep(0.5);
		gpioWrite(5, 0);
		gpioWrite(6, 1);
		time_sleep(0.5);
	}

	// Stop DMA, release resources
	gpioTerminate();

	return 0;
}

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pigpio.h>

//Pin 15 on Raspberry Pi corresponds to BCM GPIO 22
#define D1 22
//Pin 16 on Raspberry Pi corresponds to BCM GPIO 23
#define D2 23

void terminate()
{
	gpioWrite(D1, 0);
	gpioWrite(D2, 0);

	//Stop DMA, release resources
	gpioTerminate();
	exit(0);
}

int main()
{
	if (0 > gpioInitialise())
	{
		fprintf(stderr, "pigpio initialisation failed\n");
		return 1;
	}

	// Set GPIOs to output modes
	gpioSetMode(D1, PI_OUTPUT);
	gpioSetMode(D2, PI_OUTPUT);

	signal(SIGINT, terminate);

	// Blink
	while(1)
	{
		gpioWrite(D1, 1);
		gpioWrite(D2, 0);
		time_sleep(0.5);
		gpioWrite(D1, 0);
		gpioWrite(D2, 1);
		time_sleep(0.5);
	}

	// Stop DMA, release resources
	gpioTerminate();

	return 0;
}

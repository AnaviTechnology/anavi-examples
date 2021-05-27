#include <wiringPi.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

//Pin 15 on Raspberry Pi corresponds to BCM GPIO 3 and wiringPi pin 3
#define LED 3

void close()
{
	digitalWrite(LED, 0);
	exit(0);
}

int main()
{
	signal(SIGINT, close);

	if(-1 == wiringPiSetup())
	{
		printf("setup wiringPi failed!");
		return 1;
	}

	//Set GPIO pins
	pinMode(LED, OUTPUT);

	// Blink
	while(1)
	{
		digitalWrite(LED, 1);
		delay(500);
		digitalWrite(LED, 0);
		delay(500);
	}

	return 0;
}


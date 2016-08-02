#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//Pin 31 on Raspberry Pi corresponds to BCM GPIO 6 and wiringPi pin 22
#define BeepPin 22

void close()
{
	printf("\nTurning off buzzer...\n");
	digitalWrite(BeepPin, LOW);
	exit(0);
}

int main(void)
{
	signal(SIGINT, close);

	if(-1 == wiringPiSetup())
	{
			printf("setup wiringPi failed!");
			return 1;
	}

	//Set GPIO pin
	pinMode(BeepPin, OUTPUT);

	printf("Press CTRL+C to exit.\n");

	//Play a sound until the user closes the app
	while(1)
	{
		digitalWrite(BeepPin, LOW);
		delay(2);
		digitalWrite(BeepPin, HIGH);
		delay(2);
	}

	return 0;
}

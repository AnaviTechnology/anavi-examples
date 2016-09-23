#include <wiringPi.h>
#include <stdio.h>

//Pin 23 on Raspberry Pi corresponds to BCM GPIO 11 and wiringPi pin 14
#define ButtonPin 14

int main(void)
{
	if(-1 == wiringPiSetup())
	{
			printf("setup wiringPi failed!");
			return 1;
	}

	//Set GPIO pin
	pinMode(ButtonPin, INPUT);
	pullUpDnControl(ButtonPin, PUD_UP);

	printf("Press the button and release it after at least a second.\n");

	//Monitor the button and print text if button
	//has been released after at least a second
	int buf = HIGH;
	while(1)
	{
		int status = digitalRead(ButtonPin);
		if ( (HIGH == status) && (LOW == buf) )
		{
			printf("Button released!\n");
		}
		buf = status;
		delay(1000);
	}

	return 0;
}

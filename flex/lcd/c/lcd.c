#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <wiringPi.h>
#include <lcd.h>

int main()
{
	wiringPiSetup();
	int rows = 2;
	int cols = 16;
	static int lcdHandle = 0;
	// rows - 2 
	// columns - 16
	// RESET - pin 7 GPIO4
	// Enable - pin 40 GPIO21
	// D0 - pin 13 GPIO27
	// D1 - pin 15 GPIO22
	// D2 - pin 19 GPIO10
	// D3 - pin 21 GPIO9
	lcdHandle = lcdInit(2, 16, 4, 7, 29, 2, 3, 12, 13, 0, 0, 0, 0);
	lcdPosition(lcdHandle, 0, 0);
	lcdPuts(lcdHandle, "ANAVI");
	lcdPosition (lcdHandle, 0, 1);
	lcdPuts(lcdHandle, "anavi.technology");
	return 0;
}

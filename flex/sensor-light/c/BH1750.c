#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "BH1750.h"

int getLux(int fd)
{
	wiringPiI2CWrite(fd,0x10);
	delay(LUXDELAY);
	int word = wiringPiI2CReadReg16(fd, 0x00);
	if (-1 == word)
	{
		return -1;
	}
	int lux=((word & 0xff00)>>8) | ((word & 0x00ff)<<8);
	return lux;
}

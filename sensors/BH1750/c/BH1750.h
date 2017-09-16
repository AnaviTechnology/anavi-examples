#ifndef _BH1750_H_
#define _BMP1750_H_

#define BH1750_ADDR 0x23

//Delay getLux function
#define LUXDELAY 500

int getLux(int fd);

#endif

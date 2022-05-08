#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "BH1750.h"

int getLux(int fd)
{
	if (0 > ioctl(fd, I2C_SLAVE, BH1750_ADDR))
	{
		return -1;
	}

	char buf[2];
	__s32 res = i2c_smbus_read_i2c_block_data(fd, 0x10, 2, buf);
	if (0 > res)
	{
		return -1;
	}
	int lux = (buf[1] + (256 * buf[0])) / 1.2;
	return lux;
}

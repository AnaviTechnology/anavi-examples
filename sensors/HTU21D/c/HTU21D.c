#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "HTU21D.h"

int reset(int fd)
{
	if (0 > ioctl(fd, I2C_SLAVE, HTU21D_I2C_ADDR))
	{
		return -1;
	}
	i2c_smbus_write_byte(fd, HTU21D_RESET);
	return 0;
}

// Get temperature
int getTemperature(int fd, double *temperature)
{
	reset(fd);

	char buf[3];
	__s32 res = i2c_smbus_read_i2c_block_data(fd, HTU21D_TEMP, 3, buf);
	if (0 > res)
	{
		return -1;
	}
	*temperature = -46.85 + 175.72 * (buf[0] * 256 + buf[1]) / 65536;
	return 0;
}

// Get humidity
int getHumidity(int fd, double *humidity)
{
	reset(fd);
	char buf[3];
	__s32 res = i2c_smbus_read_i2c_block_data(fd, HTU21D_HUMID, 3, buf);
	if (0 > res)
	{
		return -1;
	}
	*humidity = -6 + 125 * (buf[0] * 256 + buf[1]) / 65536.0;
	return 0;
}

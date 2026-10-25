#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define ENS212_I2C_ADDR 0x45

#define ENS212_PART_ID 0x00
#define ENS212_SYS_CTRL 0x10
#define ENS212_SENS_RUN 0x21
#define ENS212_SENS_START 0x22
/* T_VAL (3 bytes) followed by H_VAL (3 bytes) */
#define ENS212_T_VAL 0x30

/* Single-shot T+H conversion, with margin */
#define ENS212_CONVERSION_US 150000

static uint32_t crc7(uint32_t val)
{
	const unsigned int width = 7;
	const uint32_t poly = 0x89;
	const uint32_t ivec = 0x7F;
	const unsigned int dwidth = 17;
	uint32_t pol = poly << (dwidth - width - 1) << width;
	uint32_t bit = (1U << (dwidth - 1)) << width;
	uint32_t mask = ((1U << dwidth) - 1) << width;

	val = (val << width) | ivec;
	while (bit & mask)
	{
		if (bit & val)
		{
			val ^= pol;
		}
		bit >>= 1;
		pol >>= 1;
	}
	return val;
}

static int ens212_decode(const uint8_t raw[3], uint16_t *data, const char **error)
{
	uint32_t word = (uint32_t)raw[0] | ((uint32_t)raw[1] << 8) | ((uint32_t)raw[2] << 16);
	uint16_t value = word & 0xFFFF;
	unsigned int valid = (word >> 16) & 1;
	unsigned int crc = (word >> 17) & 0x7F;

	if (!valid)
	{
		if (error)
		{
			*error = "measurement not valid";
		}
		return -1;
	}
	if (crc7(word & 0x1FFFF) != crc)
	{
		if (error)
		{
			*error = "CRC mismatch";
		}
		return -1;
	}

	*data = value;
	return 0;
}

static int ens212_write(int fd, uint8_t reg, uint8_t value)
{
	if (i2c_smbus_write_byte_data(fd, reg, value) < 0)
	{
		return -1;
	}
	return 0;
}

static int ens212_read_reg(int fd, uint8_t reg, uint8_t *buf, size_t len)
{
	__s32 res = i2c_smbus_read_i2c_block_data(fd, reg, (uint8_t)len, buf);

	if (res < 0 || (size_t)res != len)
	{
		return -1;
	}
	return 0;
}

static int ens212_init(int fd, int addr)
{
	uint8_t raw[2];
	uint16_t part_id;

	if (ioctl(fd, I2C_SLAVE, addr) < 0)
	{
		return -1;
	}

	if (ens212_write(fd, ENS212_SYS_CTRL, 0x80) < 0) /* reset */
	{
		return -1;
	}
	usleep(10000);

	if (ens212_write(fd, ENS212_SYS_CTRL, 0x00) < 0) /* disable low-power */
	{
		return -1;
	}
	usleep(10000);

	if (ens212_read_reg(fd, ENS212_PART_ID, raw, sizeof(raw)) < 0)
	{
		return -1;
	}

	part_id = (uint16_t)(raw[0] | (raw[1] << 8));
	if ((part_id & 0xFF00) != 0x0200)
	{
		fprintf(stderr, "Unexpected part ID 0x%04X\n", part_id);
		return -1;
	}

	printf("Found ENS21x part ID 0x%04X\n", part_id);
	return 0;
}

static int ens212_read(int fd, double *temperature, double *humidity, const char **error)
{
	uint8_t raw[6];
	uint16_t t_raw;
	uint16_t h_raw;

	if (ens212_write(fd, ENS212_SENS_RUN, 0x00) < 0 || /* single-shot T and H */
		ens212_write(fd, ENS212_SENS_START, 0x03) < 0) /* start T and H */
	{
		if (error)
		{
			*error = strerror(errno);
		}
		return -1;
	}

	usleep(ENS212_CONVERSION_US);

	if (ens212_read_reg(fd, ENS212_T_VAL, raw, sizeof(raw)) < 0)
	{
		if (error)
		{
			*error = strerror(errno);
		}
		return -1;
	}

	if (ens212_decode(raw, &t_raw, error) < 0)
	{
		return -1;
	}
	if (ens212_decode(raw + 3, &h_raw, error) < 0)
	{
		return -1;
	}

	*temperature = t_raw / 64.0 - 273.15;
	*humidity = h_raw / 512.0;
	return 0;
}

static void usage(const char *program)
{
	fprintf(stderr,
			"Usage: %s [-b bus] [-a address] [-i interval]\n"
			"  -b bus       I2C bus number (default 1)\n"
			"  -a address   I2C address (default 0x45)\n"
			"  -i interval  seconds between reads; 0 = read once (default 2.0)\n",
			program);
}

int main(int argc, char **argv)
{
	int bus = 1;
	int addr = ENS212_I2C_ADDR;
	double interval = 2.0;
	char filename[32];
	int opt;
	int fd;

	while ((opt = getopt(argc, argv, "b:a:i:h")) != -1)
	{
		switch (opt)
		{
		case 'b':
			bus = atoi(optarg);
			break;
		case 'a':
			addr = (int)strtol(optarg, NULL, 0);
			break;
		case 'i':
			interval = atof(optarg);
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		default:
			usage(argv[0]);
			return 1;
		}
	}

	snprintf(filename, sizeof(filename), "/dev/i2c-%d", bus);
	fd = open(filename, O_RDWR);
	if (fd < 0)
	{
		fprintf(stderr, "ERROR: Unable to open I2C device %s: %s\n", filename, strerror(errno));
		return 1;
	}

	if (ens212_init(fd, addr) < 0)
	{
		fprintf(stderr, "ERROR: ENS212 sensor module not found\n");
		close(fd);
		return 1;
	}

	for (;;)
	{
		double temperature;
		double humidity;
		const char *error = NULL;

		if (ens212_read(fd, &temperature, &humidity, &error) < 0)
		{
			fprintf(stderr, "read error: %s\n", error ? error : "unknown error");
		}
		else
		{
			time_t now = time(NULL);
			struct tm tm;
			char timestamp[16];

			localtime_r(&now, &tm);
			strftime(timestamp, sizeof(timestamp), "%H:%M:%S", &tm);
			printf("%s  %6.2f C  %6.2f %%RH\n", timestamp, temperature, humidity);
			fflush(stdout);
		}

		if (interval <= 0.0)
		{
			break;
		}
		usleep((useconds_t)(interval * 1000000.0));
	}

	close(fd);
	return 0;
}

#ifndef _HTU21D_H_
#define _HTU21D_H_

#define   HTU21D_I2C_ADDR 0x40

#define   HTU21D_TEMP     0xE3
#define   HTU21D_HUMID    0xE5
#define   HTU21D_RESET    0xFE

// Get temperature
int getTemperature(int fd, double *temperature);

// Get humidity
int getHumidity(int fd, double *humidity);

#endif

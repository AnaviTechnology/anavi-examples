#ifndef _HTU21D_H_
#define _HTU21D_H_

#define   HTU21D_I2C_ADDR 0x40

#define   HTU21D_TEMP     0xF3
#define   HTU21D_HUMID    0xF5

// Get temperature
double getTemperature(int fd);

// Get humidity
double getHumidity(int fd);

#endif

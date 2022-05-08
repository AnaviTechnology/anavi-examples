#!/usr/bin/python3
import time
import smbus

HTU21D_ADDR = 0x40
CMD_READ_TEMP = 0xE3
CMD_READ_HUM = 0xE5
CMD_RESET = 0xFE

class HTU21D(object):

   def __init__(self):
       # Rev 2 of Raspberry Pi and all newer use bus 1
       self.bus = smbus.SMBus(1)

   def reset(self):
       self.bus.write_byte(HTU21D_ADDR, CMD_RESET)

   def temperature(self):
       self.reset()
       msb, lsb, crc = self.bus.read_i2c_block_data(HTU21D_ADDR, CMD_READ_TEMP, 3)
       return -46.85 + 175.72 * (msb * 256 + lsb) / 65536

   def humidity(self):
       self.reset()
       msb, lsb, crc = self.bus.read_i2c_block_data(HTU21D_ADDR, CMD_READ_HUM, 3)
       return -6 + 125 * (msb * 256 + lsb) / 65536.0
         
if __name__ == "__main__":
    try:
        obj = HTU21D()
        print('Temperature: ' + str(round(obj.temperature(), 2)) + 'C')
        print('Humidity: ' + str(round(obj.humidity(), 0)) + '%')
    except FileNotFoundError:
        print('ERROR: Please enable I2C.')
    except OSError:
        print('ERROR: I2C device not found. Please check HTU21D wiring.')
    except:
        print('ERROR: General unknown error')

#!/usr/bin/python3

# Based on: https://github.com/m-rtijn/bmp180

import smbus
import math
from time import sleep

BH1750_ADDR = 0x77

class BMP180:
    address = BH1750_ADDR
    bus = smbus.SMBus(1)
    mode = 1

    # BMP180 registers
    CONTROL_REG = 0xF4
    DATA_REG = 0xF6

    # Calibration data registers
    CAL_AC1_REG = 0xAA
    CAL_AC2_REG = 0xAC
    CAL_AC3_REG = 0xAE
    CAL_AC4_REG = 0xB0
    CAL_AC5_REG = 0xB2
    CAL_AC6_REG = 0xB4
    CAL_B1_REG = 0xB6
    CAL_B2_REG = 0xB8
    CAL_MB_REG = 0xBA
    CAL_MC_REG = 0xBC
    CAL_MD_REG = 0xBE

    # Calibration data variables
    calAC1 = 0
    calAC2 = 0
    calAC3 = 0
    calAC4 = 0
    calAC5 = 0
    calAC6 = 0
    calB1 = 0
    calB2 = 0
    calMB = 0
    calMC = 0
    calMD = 0


    def __init__(self):
        # Get the calibration data from the BMP180
        self.read_calibration_data()

    # I2C methods

    def read_signed_16_bit(self, register):
        """Reads a signed 16-bit value.

        register -- the register to read from.
        Returns the read value.
        """
        msb = self.bus.read_byte_data(self.address, register)
        lsb = self.bus.read_byte_data(self.address, register + 1)

        if msb > 127:
            msb -= 256

        return (msb << 8) + lsb

    def read_unsigned_16_bit(self, register):
        msb = self.bus.read_byte_data(self.address, register)
        lsb = self.bus.read_byte_data(self.address, register + 1)

        return (msb << 8) + lsb

    def read_calibration_data(self):
        #Reads and stores the raw calibration data.
        self.calAC1 = self.read_signed_16_bit(self.CAL_AC1_REG)
        self.calAC2 = self.read_signed_16_bit(self.CAL_AC2_REG)
        self.calAC3 = self.read_signed_16_bit(self.CAL_AC3_REG)
        self.calAC4 = self.read_unsigned_16_bit(self.CAL_AC4_REG)
        self.calAC5 = self.read_unsigned_16_bit(self.CAL_AC5_REG)
        self.calAC6 = self.read_unsigned_16_bit(self.CAL_AC6_REG)
        self.calB1 = self.read_signed_16_bit(self.CAL_B1_REG)
        self.calB2 = self.read_signed_16_bit(self.CAL_B2_REG)
        self.calMB = self.read_signed_16_bit(self.CAL_MB_REG)
        self.calMC = self.read_signed_16_bit(self.CAL_MC_REG)
        self.calMD = self.read_signed_16_bit(self.CAL_MD_REG)

    def get_raw_temp(self):
        #Reads and returns the raw temperature data
        # Write 0x2E to CONTROL_REG to start the measurement
        self.bus.write_byte_data(self.address, self.CONTROL_REG, 0x2E)

        # Wait 4,5 ms
        sleep(0.0045)

        # Read the raw data from the DATA_REG, 0xF6
        raw_data = self.read_unsigned_16_bit(self.DATA_REG)

        # Return the raw data
        return raw_data

    def get_raw_pressure(self):
        #Reads and returns the raw pressure data
        # Write appropriate data to sensor to start the measurement
        self.bus.write_byte_data(self.address, self.CONTROL_REG, 0x34 + (self.mode << 6))

        # Sleep for 8 ms.
        # TODO: Way to use the correct wait time for the current mode
        sleep(0.008)

        MSB = self.bus.read_byte_data(self.address, self.DATA_REG)
        LSB = self.bus.read_byte_data(self.address, self.DATA_REG + 1)
        XLSB = self.bus.read_byte_data(self.address, self.DATA_REG + 2)

        raw_data = ((MSB << 16) + (LSB << 8) + XLSB) >> (8 - self.mode)

        return raw_data

    def temperature(self):
        """Reads the raw temperature and calculates the actual temperature.

        The calculations used to get the actual temperature are from the BMP-180
        datasheet.
        Returns the actual temperature in degrees Celcius.
        """
        UT = self.get_raw_temp()

        X1 = 0
        X2 = 0
        B5 = 0
        actual_temp = 0.0

        X1 = ((UT - self.calAC6) * self.calAC5) / math.pow(2, 15)
        X2 = (self.calMC * math.pow(2, 11)) / (X1 + self.calMD)
        B5 = X1 + X2
        actual_temp = ((B5 + 8) / math.pow(2, 4)) / 10

        return actual_temp

    def pressure(self):
        #Returns the actual pressure in hectopascal (1hPa = 100 Pa).
        UP = self.get_raw_pressure()
        UT = self.get_raw_temp()
        B3 = 0
        B4 = 0
        B5 = 0
        B6 = 0
        B7 = 0
        X1 = 0
        X2 = 0
        X3 = 0
        pressure = 0

        # These calculations are from the BMP180 datasheet, page 15

        # Not sure if these calculations should be here, maybe they could be
        # removed?
        X1 = ((UT - self.calAC6) * self.calAC5) / math.pow(2, 15)
        X2 = (self.calMC * math.pow(2, 11)) / (X1 + self.calMD)
        B5 = X1 + X2

        # Todo: change math.pow cals to constants
        B6 = B5 - 4000
        X1 = (self.calB2 * (B6 * B6 / math.pow(2, 12))) / math.pow(2, 11)
        X2 = self.calAC2 * B6 / math.pow(2, 11)
        X3 = X1 + X2
        B3 = (((self.calAC1 * 4 + int(X3)) << self.mode) + 2) / 4
        X1 = self.calAC3 * B6 / math.pow(2, 13)
        X2 = (self.calB1 * (B6 * B6 / math.pow(2, 12))) / math.pow(2, 16)
        X3 = ((X1 + X2) + 2) / math.pow(2, 2)
        B4 = self.calAC4 * (X3 + 32768) / math.pow(2,15)
        B7 = (UP - B3) * (50000 >> self.mode)

        if B7 < 0x80000000:
            pressure = (B7 * 2) / B4
        else:
            pressure = (B7 / B4) * 2

        X1 = (pressure / math.pow(2, 8)) * (pressure / math.pow(2, 8))
        X1 = (X1 * 3038) / math.pow(2, 16)
        X2 = (-7357 * pressure) / math.pow(2, 16)
        # Calculate and covert to hPa
        pressure = (pressure + (X1 + X2 + 3791) / math.pow(2, 4))/100
        return pressure

    def altitude(self, sea_level_pressure = 101325):
        # Calulates an estimated altitude, not very correct
        altitude = 0.0
        pressure = float(self.pressure())

        altitude = 44330.0 * (1.0 - math.pow(pressure / sea_level_pressure, 0.00019029495))

        return altitude

if __name__ == "__main__":
    try:
        obj = BMP180()
        print('Temperature: ' + str(round(obj.temperature(), 2)) + 'C')
        print('Pressure: ' + str(round(obj.pressure())) + 'hPa')
    except FileNotFoundError:
        print('ERROR: Please enable I2C.')
    except OSError:
        print('ERROR: I2C device not found. Please check BMP180 wiring.')
    except:
        print('ERROR: General unknown error')


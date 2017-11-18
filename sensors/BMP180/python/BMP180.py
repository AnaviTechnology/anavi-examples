# coding=utf-8
from __future__ import division

import Adafruit_BMP.BMP085 as BMP085

sensor = BMP085.BMP085()
temp = sensor.read_temperature()
pressure = sensor.read_pressure() / 100

print('RabbitMax Temperature and Barometric Pressure Sensor')
print('Temperature\t{0:0.1f} C'.format(temp))
print('Pressure\t{0:0.2f} hPa'.format(pressure))

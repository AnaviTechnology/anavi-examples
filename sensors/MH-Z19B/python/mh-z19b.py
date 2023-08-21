#!/usr/bin/python3

import mh_z19
import serial
import sys

try:
  mh_z19.detection_range_10000()
  data = mh_z19.read_all()
  print('CO2: ' + str(data["co2"]) + 'ppm')
  print('Temperature: ' + str(data["temperature"]) + 'C')
except serial.serialutil.SerialException as error:
  print(error)
  print("Have you tried running it with sudo?")
except KeyError:
  print("Unable to read sensor data.")
  print("Please check the wiring and wait for the preheat time before using the sensor.")
else:
  sys.exit(0)

#!/usr/bin/python3

import mh_z19
import sys

mh_z19.detection_range_10000()
data = mh_z19.read_all()
print('CO2: ' + str(data["co2"]) + 'ppm')
print('Temperature: ' + str(data["temperature"]) + 'C')
sys.exit(0)

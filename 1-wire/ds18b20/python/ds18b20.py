# coding=utf-8
from __future__ import division

import glob
import time

base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'


def read_temp_raw():
    with open(device_file) as f:
        lines = f.readlines()
    return lines


def read_temp():
    lines = read_temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw()
    equals_pos = lines[1].find('t=')
    if -1 != equals_pos:
        temp_string = lines[1][equals_pos + 2:]
        return float(temp_string) / 1000


temperatureC = read_temp()
temperatureF = temperatureC * 9 / 5 + 32
print("Temperature: {:.2f}C ({:.2f}F)".format(temperatureC, temperatureF))

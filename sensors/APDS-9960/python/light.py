from apds9960.const import *
from apds9960 import APDS9960
import RPi.GPIO as GPIO
import smbus
from time import sleep

port = 1
bus = smbus.SMBus(port)

apds = APDS9960(bus)

try:

    print("Light Sensor Test")
    print("=================")
    apds.enableLightSensor()
    oval = -1
    while True:
        sleep(0.25)
        val = apds.readAmbientLight()
        if val != oval:
            print("AmbientLight={}".format(val))
            oval = val

finally:
    print("Bye")

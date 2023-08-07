from apds9960.const import *
from apds9960 import APDS9960
import RPi.GPIO as GPIO
import smbus
from time import sleep

port = 1
bus = smbus.SMBus(port)

apds = APDS9960(bus)

dirs = {
    APDS9960_DIR_NONE: "none",
    APDS9960_DIR_LEFT: "left",
    APDS9960_DIR_RIGHT: "right",
    APDS9960_DIR_UP: "up",
    APDS9960_DIR_DOWN: "down",
    APDS9960_DIR_NEAR: "near",
    APDS9960_DIR_FAR: "far",
}
try:
    apds.setProximityIntLowThreshold(50)

    print("APDS9960 Gesture Test")
    print("============")
    apds.enableGestureSensor()
    while True:
        sleep(0.5)
        if apds.isGestureAvailable():
            motion = apds.readGesture()
            print("Gesture={}".format(dirs.get(motion, "unknown")))


finally:
    GPIO.cleanup()
    print("Bye")

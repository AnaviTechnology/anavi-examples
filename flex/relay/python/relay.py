# coding=utf-8
import time

import RPi.GPIO as GPIO

# Relay - pin 29 on Raspberry Pi corresponds to BCM GPIO 5
RelayPin = 29

# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

# set up GPIO output channel
GPIO.setup(RelayPin, GPIO.OUT)

GPIO.output(RelayPin, GPIO.HIGH)
time.sleep(3)
GPIO.output(RelayPin, GPIO.LOW)
GPIO.cleanup()

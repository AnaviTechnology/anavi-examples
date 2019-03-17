# coding=utf-8
import signal
import sys
import time

import RPi.GPIO as GPIO

# Piezo buzzer - pin 15 on Raspberry Pi corresponds to BCM GPIO 22
BeepPin = 15


def close(signal, frame):
    GPIO.output(BeepPin, GPIO.LOW)
    sys.exit(0)


# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

signal.signal(signal.SIGINT, close)

# set up GPIO output channel
GPIO.setup(BeepPin, GPIO.OUT)

print('Press CTRL+C to exit.')
try:
    while True:
        GPIO.output(BeepPin, GPIO.HIGH)
        time.sleep(0.000002)
        GPIO.output(BeepPin, GPIO.LOW)
        time.sleep(0.000002)
finally:
    GPIO.cleanup()

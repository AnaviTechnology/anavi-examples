# coding=utf-8
import signal
import sys
import time

import RPi.GPIO as GPIO

# Blue - pin 33 on Raspberry Pi corresponds to BCM GPIO 13
Pin0 = 33
# Green - pin 35 on Raspberry Pi corresponds to BCM GPIO 19
Pin1 = 35
# Red - pin 37 on Raspberry Pi corresponds to BCM GPIO 26
Pin2 = 37


def led(red, green, blue):
    GPIO.output(Pin0, blue)
    GPIO.output(Pin1, green)
    GPIO.output(Pin2, red)


def close(signal, frame):
    print("\nTurning off lights...\n")
    led(GPIO.LOW, GPIO.LOW, GPIO.LOW)
    sys.exit(0)


signal.signal(signal.SIGINT, close)

# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

# set up GPIO output channel
GPIO.setup(Pin0, GPIO.OUT)
GPIO.setup(Pin1, GPIO.OUT)
GPIO.setup(Pin2, GPIO.OUT)

# reset
led(GPIO.LOW, GPIO.LOW, GPIO.LOW)

try:
    while True:
        led(GPIO.HIGH, GPIO.LOW, GPIO.LOW)
        time.sleep(3)
        led(GPIO.LOW, GPIO.HIGH, GPIO.LOW)
        time.sleep(3)
        led(GPIO.LOW, GPIO.LOW, GPIO.HIGH)
        time.sleep(3)
finally:
    GPIO.cleanup()

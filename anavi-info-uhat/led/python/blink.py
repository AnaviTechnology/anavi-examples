# coding=utf-8
import signal
import sys
import time

import RPi.GPIO as GPIO

# Pin 29 on Raspberry Pi corresponds to GPIO 5
LED1 = 29
# Pin 16 on Raspberry Pi corresponds to GPIO 6
LED2 = 31


def close(signal, frame):
    GPIO.output(LED1, 0)
    GPIO.output(LED2, 0)
    sys.exit(0)


signal.signal(signal.SIGINT, close)

# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

# set up GPIO output channel
GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)

# reset

try:
    while True:
        GPIO.output(LED1, 1)
        GPIO.output(LED2, 0)
        time.sleep(0.5)
        GPIO.output(LED1, 0)
        GPIO.output(LED2, 1)
        time.sleep(0.5)
finally:
    GPIO.cleanup()

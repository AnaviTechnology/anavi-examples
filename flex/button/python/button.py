# coding=utf-8
import signal
import sys
import time

import RPi.GPIO as GPIO

# Button - pin 23 on Raspberry Pi corresponds to BCM GPIO 11
ButtonPin = 23


def close(signal, frame):
    GPIO.cleanup()
    sys.exit(0)


# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)

# set up GPIO input channel
GPIO.setup(ButtonPin, GPIO.IN, GPIO.PUD_UP)

# handle Ctll+C
signal.signal(signal.SIGINT, close)

print('Press the button and release it after at least a second.')

buf = True
while True:
    status = GPIO.input(ButtonPin)
    if status and not buf:
        print('Button released!')
    buf = status
    time.sleep(1)

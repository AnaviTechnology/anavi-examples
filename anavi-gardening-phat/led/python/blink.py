import RPi.GPIO as GPIO
import time
import signal
import sys

#Pin 15 on Raspberry Pi corresponds to GPIO 3
LED = 15

def close(signal, frame):
	GPIO.output(LED, 0)
        sys.exit(0)

signal.signal(signal.SIGINT, close)

# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

# set up GPIO output channel
GPIO.setup(LED, GPIO.OUT)

#reset

while True:
	GPIO.output(LED, 1)
	time.sleep(0.5)
	GPIO.output(LED, 0)
	time.sleep(0.5)
GPIO.cleanup() 

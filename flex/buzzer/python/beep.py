import RPi.GPIO as GPIO
import time
import signal
import sys

#Piezo buzzer - pin 31 on Raspberry Pi corresponds to BCM GPIO 22
BeepPin = 31

def close(signal, frame):
	GPIO.output(BeepPin, GPIO.LOW)
        sys.exit(0)

# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

signal.signal(signal.SIGINT, close)

# set up GPIO output channel
GPIO.setup(BeepPin, GPIO.OUT)

while True:
	GPIO.output(BeepPin, GPIO.HIGH)
	time.sleep(0.000002)
	GPIO.output(BeepPin, GPIO.LOW)
	time.sleep(0.000002)
GPIO.cleanup() 

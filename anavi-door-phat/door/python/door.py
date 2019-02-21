#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# Initial state
isOpen = GPIO.input(13);
datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
if isOpen == True:
	print('%s Initially the door is open.' % (datetime))
else:
	print('%s Initially the door is closed.' % (datetime))

while True:
	state = GPIO.input(13)
	if state == True and isOpen == False:
		isOpen = state
		datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
		print('%s The door has been opened.' % (datetime))
	elif state == False and isOpen == True:
		isOpen = state
		datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
		print('%s The door has been closed.' % (datetime))
	time.sleep(0.2)

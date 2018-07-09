import RPi.GPIO as GPIO
import time
from evdev import UInput, ecodes as e

# Set GPIO for each button
btnLeft = 4
btnUp = 22
btnRight = 17
btnDown = 27
btnB = 26
btnA = 19
btnSelect = 6
btnStart = 5

GPIO.setmode(GPIO.BCM)

# Enable buttons
GPIO.setup(btnLeft, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnUp, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnRight, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnDown, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnB, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnA, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnSelect, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(btnStart, GPIO.IN, pull_up_down = GPIO.PUD_UP)

# Use evdev to send key events
ui = UInput()

def hold(key):
	ui.write(e.EV_KEY, key, 1)
	ui.syn()
	return 0

def release(key):
	ui.write(e.EV_KEY, key, 0)
	ui.syn()
	return 0

bD = False
bF = False
bG = False
bH = False
bLeft = False
bRight = False
bUp = False
bDown = False

# Handle buttons
while True:
	time.sleep(0.02)
	if (not bLeft) and (not GPIO.input(btnLeft)):
                bLeft = True
		hold(e.KEY_LEFT)
	if bLeft and GPIO.input(btnLeft):
                release(e.KEY_LEFT)
		bLeft = False
	if (not bRight) and (not GPIO.input(btnRight)):
                bRight = True
		hold(e.KEY_RIGHT)
	if bRight and GPIO.input(btnRight):
                release(e.KEY_RIGHT)
		bRight = False
	if (not bUp) and (not GPIO.input(btnUp)):
                bUp = True
		hold(e.KEY_UP)
	if bUp and GPIO.input(btnUp):
                release(e.KEY_UP)
		bUp = False
	if (not bDown) and (not GPIO.input(btnDown)):
                bDown = True
		hold(e.KEY_DOWN)
        if bDown and GPIO.input(btnDown):
                release(e.KEY_DOWN)
		bDown = False
	if (not bD) and (not GPIO.input(btnA)):
                hold(e.KEY_A)
                bD = True
	if bD and GPIO.input(btnA):
                release(e.KEY_A)
                bD = False
	if (not bF) and (not GPIO.input(btnB)):
                hold(e.KEY_B)
                bF = True
	if bF and GPIO.input(btnB):
                release(e.KEY_B)
                bF = False
	if (not bG) and (not GPIO.input(btnSelect)):
		hold(e.KEY_S)
		bG = True
	if bG and GPIO.input(btnSelect):
                release(e.KEY_S)
		bG = False
	if (not bH) and (not GPIO.input(btnStart)):
		hold(e.KEY_SPACE)
		bH = True
	if bH and GPIO.input(btnStart):
                release(e.KEY_SPACE)
		bH = False


GPIO.cleanup()

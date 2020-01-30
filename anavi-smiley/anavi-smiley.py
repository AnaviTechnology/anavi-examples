#!/usr/bin/env python3

from gpiozero import Button
from gpiozero import LEDBoard
import time

#btn = Button( 26, pull_up = True, bounce_time= None) 
btn = Button(26, pull_up = True, bounce_time=0.1)
leds = LEDBoard(13, 19)
leds.value = (1, 1);

print("ANAVI Smiley")
print("---")
print("Please press the button")
print("---")

counter = 1

while True:
    time.sleep(0.2)
    if btn.is_pressed:
        print("Button pressed, sequence: ", counter)
        if 0 == counter:
            leds.value = (1, 1);
        if 1 == counter:
            leds.value = (1, 0);
        if 2 == counter:
            leds.value = (0, 1);
        if 3 == counter:
            leds.value = (0, 0);
        counter += 1
        if 4 <= counter:
            counter = 0

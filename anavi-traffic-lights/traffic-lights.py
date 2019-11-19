#!/usr/bin/env python3

from gpiozero import LEDBoard
import time

leds = LEDBoard(13, 19, 26)

leds.off();

print("ANAVI Traffic Lights")
print("---")

while True:
    leds.on();
    time.sleep(1)
    leds.value = (1, 0, 0);
    time.sleep(1)
    leds.value = (0, 1, 0);
    time.sleep(1)
    leds.value = (0, 0, 1);
    time.sleep(1)

#!/usr/bin/env python3

from gpiozero import Button
import time
import signal
import sys

def close(signal, frame):
    sys.exit(0)

signal.signal(signal.SIGINT, close)

btnB1 = Button(22)
btnB2 = Button(27)
btnB3 = Button(17)

print("ANAVI Info uHAT")
print("---")
print("Please press a button")
print("---")

while True:
    time.sleep(0.1)
    if btnB1.is_pressed:
        print("Button SW1")
    if btnB2.is_pressed:
        print("Button SW2")
    if btnB3.is_pressed:
        print("Button SW3")

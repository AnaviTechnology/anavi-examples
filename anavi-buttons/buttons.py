#!/usr/bin/env python3

from gpiozero import Button
import time

btnB1 = Button(13)
btnB2 = Button(19)
btnB3 = Button(26)

print("ANAVI Buttons")
print("---")
print("Please press a button")
print("---")

while True:
    time.sleep(0.1)
    if btnB1.is_pressed:
        print("B1")
    if btnB2.is_pressed:
        print("B2")
    if btnB3.is_pressed:
        print("B3")
    

    

#!/usr/bin/env python3

from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106
from luma.core.error import DeviceNotFoundError
import os
import time
import signal
import sys
import socket

from PIL import ImageFont, ImageDraw

def getIP():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip = s.getsockname()[0]
    s.close()
    return ip

def signal_handler(sig, frame):
        print("\nApplication terminated with Ctrl+C.")
        sys.exit(0)

drawfont = "pixelmix.ttf"

try:
    signal.signal(signal.SIGINT, signal_handler)
    serial = i2c(port=1, address=0x3C)
    device = ssd1306(serial, rotate=0)

    while True:
        with canvas(device) as draw:
            draw.rectangle(device.bounding_box, outline="white", fill="black")
            font = ImageFont.truetype(drawfont, 10)
            ip = getIP()
            draw.text((5, 5), "IP: " + ip, fill="white", font=font)
            font = ImageFont.truetype(drawfont, 16)
            draw.text((5, 20), "Hello,", fill="white", font=font)
            draw.text((5, 42), "World!", fill="white", font=font)
    sleep(1)
except DeviceNotFoundError:
    print("I2C mini OLED display not found.")
    sys.exit(1)
except SystemExit:
    print("Exiting...")
    sys.exit(0)
except:
    print("Unexpected error:", sys.exc_info()[0])
    sys.exit(2)

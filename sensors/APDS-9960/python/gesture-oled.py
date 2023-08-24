import os
import sys
import signal
import smbus
import socket
from time import sleep
from apds9960.const import *
from apds9960 import APDS9960
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106
from luma.core.error import DeviceNotFoundError
from PIL import ImageFont, ImageDraw

def signal_handler(sig, frame):
  # Clear the OLED display
  serial = i2c(port=1, address=0x3C)
  device = ssd1306(serial, rotate=0)
  print("\nApplication termiated with Ctrl+C.")
  os._exit(0)

def draw_text(display, line1, line2, line3):
  with canvas(display) as draw:
    draw.rectangle(device.bounding_box, outline="white", fill="black")
    font = ImageFont.truetype(drawfont, 10)
    draw.text((5, 5), line1, fill="white", font=font)
    font = ImageFont.truetype(drawfont, 16)
    draw.text((5, 20), line2, fill="white", font=font)
    draw.text((5, 42), line3, fill="white", font=font)

dirs = {
  APDS9960_DIR_NONE: "none",
  APDS9960_DIR_LEFT: "left",
  APDS9960_DIR_RIGHT: "right",
  APDS9960_DIR_UP: "up",
  APDS9960_DIR_DOWN: "down",
  APDS9960_DIR_NEAR: "near",
  APDS9960_DIR_FAR: "far",
}

try:

  signal.signal(signal.SIGINT, signal_handler)

  drawfont = "pixelmix.ttf"
  serial = i2c(port=1, address=0x3C)
  device = ssd1306(serial, rotate=0)

  port = 1
  bus = smbus.SMBus(port)
  apds = APDS9960(bus)

  apds.setProximityIntLowThreshold(50)

  print("APDS9960 Gesture Test")
  print("============")
  apds.enableGestureSensor()

  draw_text(device, "APDS-9960", "Show me a", "gesture!")

  while True:
    sleep(0.5)

    if apds.isGestureAvailable():
      motion = apds.readGesture()
      print("Gesture={}".format(dirs.get(motion, "unknown")))

      draw_text(device, "APDS-9960", "Gesture", "{}".format(dirs.get(motion, "unknown")))

except OSError as error:
  if 121 == error.errno:
    print('No sensor found')
  elif None == error.errno:
    print('Error. Is the ttf font file available?')
  else:
    print('Error:', sys.exc_info()[0])
  os._exit(1)

except:
  print("Unexpected error:", sys.exc_info()[0])
  os._exit(2)

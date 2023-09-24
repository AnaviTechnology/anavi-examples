import os
import sys
import signal
import smbus
import socket
from time import sleep
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106
from luma.core.error import DeviceNotFoundError
from PIL import ImageFont, ImageDraw

BH1750_ADDR = 0x23
CMD_READ = 0x10

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

class BH1750(object):

    def __init__(self):
        # Rev 2 of Raspberry Pi and all newer use bus 1
        self.bus = smbus.SMBus(1)

    def light(self):
        data = self.bus.read_i2c_block_data(BH1750_ADDR, CMD_READ)
        result = (data[1] + (256 * data[0])) / 1.2
        return format(result,'.0f')

try:

  signal.signal(signal.SIGINT, signal_handler)

  drawfont = "pixelmix.ttf"
  serial = i2c(port=1, address=0x3C)
  device = ssd1306(serial, rotate=0)

  sensor = BH1750()

  print("BH1750 Ambient Light Sensor")
  print("============")

  draw_text(device, "BH1750", "", "")

  while True:

    print("Light: " + sensor.light() + " Lux")
    draw_text(device, "BH1750", "Light:", sensor.light() + " lux")
    sleep(0.5)

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

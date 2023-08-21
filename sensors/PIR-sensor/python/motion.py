import os
import sys
import signal
from signal import pause
from gpiozero import MotionSensor

def motion_function():
  global motion_count
  motion_count += 1
  print(f'Motion Detected: {motion_count}')

def no_motion_function():
  print(f'Motion stopped: {motion_count}')

def signal_handler(sig, frame):
  print("\nApplication termiated with Ctrl+C.")
  os._exit(0)

try:
  signal.signal(signal.SIGINT, signal_handler)

  pir = MotionSensor(4)
  motion_count = 0

  pir.when_motion = motion_function
  pir.when_no_motion = no_motion_function

  pause()

except:
  print("Unexpected error:", sys.exc_info()[0])
  os._exit(2)

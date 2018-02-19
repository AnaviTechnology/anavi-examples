from gpiozero import MotionSensor
import time
import signal
import sys

def close(signal, frame):
    sys.exit(0)

signal.signal(signal.SIGINT, close)

pir = MotionSensor(4)
count = 0;

while True:
    if pir.motion_detected:
        count += 1
        print "Motion detected: ", count
        time.sleep(1)

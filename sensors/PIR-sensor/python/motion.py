from gpiozero import MotionSensor
from signal import pause
pir = MotionSensor(4)
motion_count = 0

def motion_function():
    global motion_count
    motion_count += 1
    print(f'Motion Detected: {motion_count}')

def no_motion_function():
    print(f'Motion stopped: {motion_count}')

pir.when_motion = motion_function
pir.when_no_motion = no_motion_function

pause()


import signal
import sys
import time
import spidev
import RPi.GPIO as GPIO

# Pin 15 on Raspberry Pi corresponds to GPIO 22
LED1 = 15
# Pin 16 on Raspberry Pi corresponds to GPIO 23
LED2 = 16

spi_ch = 0

# Enable SPI
spi = spidev.SpiDev(0, spi_ch)
spi.max_speed_hz = 1200000

# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

# set up GPIO output channel
GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)

def close(signal, frame):
    GPIO.output(LED1, 0)
    GPIO.output(LED2, 0)
    sys.exit(0)

signal.signal(signal.SIGINT, close)

def get_adc(channel):

    # Make sure ADC channel is 0 or 1
    if channel != 0:
        channel = 1

    # Construct SPI message
    #  First bit (Start): Logic high (1)
    #  Second bit (SGL/DIFF): 1 to select single mode
    #  Third bit (ODD/SIGN): Select channel (0 or 1)
    #  Fourth bit (MSFB): 0 for LSB first
    #  Next 12 bits: 0 (don't care)
    msg = 0b11
    msg = ((msg << 1) + channel) << 5
    msg = [msg, 0b00000000]
    reply = spi.xfer2(msg)

    # Construct single integer out of the reply (2 bytes)
    adc = 0
    for n in reply:
        adc = (adc << 8) + n

    # Last bit (0) is not part of ADC value, shift to remove it
    adc = adc >> 1

    # Calculate voltage form ADC value
    # considering the soil moisture sensor is working at 5V
    voltage = (5 * adc) / 1024

    return voltage

if __name__ == '__main__':
    # Report the channel 0 and channel 1 voltages to the terminal
    try:
        while True:
            adc_0 = get_adc(0)
            adc_1 = get_adc(1)
            print("Soil Moisture Sensor 1:", round(adc_0, 2), "V Soil Moisture Sensor 2:", round(adc_1, 2), "V")
            GPIO.output(LED1, 1)
            GPIO.output(LED2, 0)
            time.sleep(0.2)
            GPIO.output(LED1, 0)
            GPIO.output(LED2, 1)
            time.sleep(0.2)

    finally:
        GPIO.cleanup()

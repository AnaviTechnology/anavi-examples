# anavi-examples

Examples for getting started and testing ANAVI hardware

## ANAVI Infrared pHAT

ANAVI Infrared pHAT is an open source hardware Raspberry Pi add-on board with IR receiver, transmitter, UART and 3 I2C slots for sensors. The project allows you to convert your Raspberry Pi into a smart remote control using the open source software LIRC.

ANAVI Infrared pHAT Raspberry Pi HAT includes:

* IR LED
* IR photo sensor
* Slots for up to 3 plug and play I2C sensors

User's manual for ANAVI Infrared pHAT is available at: http://anavi.technology/files/anavi-infrared-phat.pdf

### Sensors

It is mandatory to enable **I2C** to use any of the sensors supported by ANAVI Infrared pHAT Raspberry Pi HAT.

#### Temperature Sensor (BMP180)

Follow the steps below to use the BMP180 I2C temperature and barometric pressure sensor with ANAVI Infrared pHAT:

* Connect BMP180 to any of the I2C slots on ANAVI Infrared pHAT using male to female Duport jumper wire.

* Type in the following command and verify that the address of the sensor is listed:

```
sudo i2cdetect -y 1
```

* Type in the following commands to build and run the sample application that display temperature and barometric pressure:

```
cd ~/anavi-examples/sensors/BMP180/c/
make
./BMP180
```

* Verify that the output is similar to (the exact values depend on the atmospheric condition):

```
pi@raspberrypi:~/anavi-examples/sensors/BMP180/c $ ./BMP180
BMP180 Sensor Module
Temperature	28.6 C
Pressure	991.57 hPa
```

#### Humidity Sensor (HTU21D)

Follow the steps below to use the HTU21D I2C temperature and humidity sensor with ANAVI Infrared pHAT:

* Connect HTU21D to any of the I2C slots on ANAVI Infrared pHAT using male to female Duport jumper wire.

* Type in the following command and verify that the address of the sensor is listed:

```
sudo i2cdetect -y 1
```

* Type in the following commands to build and run the sample application that display temperature and humidity:

```
cd ~/anavi-examples/sensors/HTU21D/c/
make
./HTU21D
```

* Verify that the output is similar to (the exact values depend on the atmospheric condition):

```
pi@raspberrypi:~/anavi-examples/sensors/HTU21D/c $ ./HTU21D
HTU21D Sensor Module
25.64C
118.99%rh
```

#### BH1750 Light Sensor Module

Follow the steps below to use the BH1750 I2C light sensor with ANAVI Infrared pHAT:

* Connect BH1750 to any of the I2C slots on ANAVI Infrared pHAT using male to female Dupont jumper wires.

* Type in the following command and verify that the address of the sensor is listed:

```
sudo i2cdetect -y 1
```

* Type in the following commands to build and run the sample application that display luminous emittance:

```
cd ~/anavi-examples/sensors/BH1750/c/
make
./BH1750
```

* Verify that the output is similar to (the exact values depend on the atmospheric condition):

```
pi@raspberrypi:~/anavi-examples/sensors/BH1750/c $ ./BH1750
BH1750 Sensor Module
Light: 418 Lux
```

## ANAVI Flex

ANAVI Flex is an open source hardware Raspberry Pi HAT (add-on board) for rapid prototyping of Internet of Things (IoT) powered by free and open source software. It features:

* IR transmitter (IR LED)
* IR receiver (IR photo sensor)
* Relay
* Piezoelectric speaker (buzzer)
* Button
* RGB LED
* Slot for modular 16x2 LCD character display
* Slots for up to 5 plug and play I2C sensors for temperature, barometric pressure, humidity and light
* UART (Universal asynchronous receiver/transmitter) pins for debugging
* contrib/ac-toshiba.py - Reads Toshiba AC Remote packets
* contrib/ir_packet_read.py - Read IR packets and display as binary & hex

Support the project by backing [ANAVI Flex IndieGoGo campaign](https://igg.me/at/Rzjuluvxe6Y/10833949).

# C

Run make to build each application written in the C programming language.

# Python

Execute the following command to install the required packages for Python development:

```
sudo apt-get install python-dev python-rpi.gpio
```

Execute the following commands to download and install Adafruit Python BMP library:

```
git clone https://github.com/adafruit/Adafruit_Python_BMP.git
cd Adafruit_Python_BMP
sudo python setup.py install
```

## ESPHome

[ESPHome](https://esphome.io/) is a system to control your ESP8266/ESP32 by simple yet powerful configuration files and control them remotely through Home Automation systems.

An example ESPHome configuration is available for ANAVI Thermometer. Follow the steps below to install it through the command-line:

* Install ESPHome

```
pip3 install esphome
```

* Download the example

```
git clone https://github.com/AnaviTechnology/anavi-examples.git
```

* Set your WiFi credentials at `anavi-thermometer/esphome-anavi-thermometer/anavi-thermometer.yaml`

* Compile and flash the firmware. Follow the on-screen instructions. Press the RESET button on ANAVI Thermometer before powering ANAVI Thermometer and hold it until the firmware upload complete. Power cycle ANAVI Thermometer after flashing ESPHome:

```
cd anavi-thermometer/esphome-anavi-thermometer
esphome anavi-thermometer.yaml run
```

## ANAVI Smiley

### Device Tree Overlay

Follow the steps below to compile and use anavi-smiley.dts as a sample device tree overlay configuration that shows how to configure button as volume mute, LED1 to indicated CPU activity and LED2 the heartbeat

* On Raspberry Pi OS, install device tree compiler:

```
sudo apt update
sudo apt install -y device-tree-compiler
```

* Download the examples and compile device tree overlay:

```
dtc -I dts -O dtb -o anavi-smiley.dtbo anavi-smiley.dts
```

* Copy the dtbo file to `/boot/overlays/`:

```
sudo cp anavi-smiley.dtbo /boot/overlays/
```

* Manually enable the device tree overlay in /boot/config.txt:

```
dtoverlay=anavi-smiley
gpio=26=pu
```

* Reboot 

Linux key codes are as defined in `/usr/include/linux/input-event-codes.h`.

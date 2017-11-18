# rabbitmax-examples
Examples for getting started and testing ANAVI hardware

# ANAVI Flex

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

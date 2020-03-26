from machine import Pin, I2C
import ssd1306

i2c = I2C(scl=Pin(5), sda=Pin(4), freq=100000)

lcd = ssd1306.SSD1306_I2C(128, 64, i2c)
lcd.text("MicroPython", 0, 0)
lcd.text("-----------", 0, 16)
lcd.text("Hello World!", 0, 32)
lcd.show()

print("Hello World!")

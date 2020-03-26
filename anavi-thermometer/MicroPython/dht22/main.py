import dht
import machine
import time

while True:
    time.sleep(2)
    sensor = dht.DHT22(machine.Pin(2))
    sensor.measure()
    temperature = sensor.temperature()
    humidity = sensor.humidity()
    print ("Temperature: " + str(temperature) + "C")
    print ("Humidity: " + str(humidity) + "%")

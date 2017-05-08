import glob
import time

base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'

def read_temp_raw():
	f = open(device_file, 'r')
	lines = f.readlines()
	f.close()
	return lines

def read_temp():
	lines = read_temp_raw()
	while lines[0].strip()[-3:] != 'YES':
		time.sleep(0.2)
		lines = read_temp_raw()
	equals_pos = lines[1].find('t=')
	if -1 != equals_pos:
		temp_string = lines[1][equals_pos+2:]
		return float(temp_string) / 1000.0
	
temperatureC = read_temp()
temperatureF = temperatureC * 9.0 / 5.0 + 32.0
print("Temperature: %.2fC (%.2fF)" % (temperatureC, temperatureF))

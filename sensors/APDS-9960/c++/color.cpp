#include <iostream>
#include "APDS9960.h"

using namespace std;

int main() 
{
	APDS9960 apds = APDS9960();
	uint16_t ambient_light = 0;
	uint16_t red_light = 0;
	uint16_t green_light = 0;
	uint16_t blue_light = 0;

	cout << endl;
	cout << "------------------------------------" << endl;
	cout << "APDS-9960 - ColorSensor  " << endl;
	cout << "------------------------------------" << endl;

	// Initialize APDS-9960 (configure I2C and initial values)
	if ( (false == apds.init()) || (false == apds.enableLightSensor(false)) ) 
	{
		cout << "APDS-9960 init failed!" << endl;
	}


	// Wait for initialization and calibration to finish
	delay(500);

	while(1) {
		// Read the light levels (ambient, red, green, blue)
		if (  !apds.readAmbientLight(ambient_light) ||
				!apds.readRedLight(red_light) ||
				!apds.readGreenLight(green_light) ||
				!apds.readBlueLight(blue_light) ) {
			cout << "Error reading light values" << endl;
		}
		else {
			cout << "Ambient: ";
			cout << int(ambient_light);
			cout << " Red: ";
			cout << int(red_light);
			cout << " Green: ";
			cout << int(green_light);
			cout << " Blue: ";
			cout << int(blue_light) << endl;
		}

		// Wait 1 second before next reading
		delay(1000);
	}
	return 0;
}

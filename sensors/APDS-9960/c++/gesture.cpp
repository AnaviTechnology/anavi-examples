#include <iostream>
#include "APDS9960.h"

using namespace std;

void handleGesture(APDS9960 apds);

int main() 
{
	APDS9960 apds = APDS9960();

	// init wiringPi
	wiringPiSetup();

	// Initialize Serial port
	cout << endl;
	cout << "------------------------------------" << endl;
	cout << " APDS-9960 - GestureTest  " << endl;
	cout << "------------------------------------" << endl;

	// Initialize APDS-9960 (configure I2C and initial values)
	if ( (false == apds.init()) || (false == apds.enableGestureSensor(false)) )
	{
		cout << "APDS-9960 init failed!" << endl;
	}

	while(1) 
	{
		handleGesture(apds);
	}
	return 0;
}

void handleGesture(APDS9960 apds) 
{
	if ( false == apds.isGestureAvailable() )
	{
		return;
	}

	switch ( apds.readGesture() ) 
	{
		case DIR_UP:
			cout << "UP" << endl;
			break;
		case DIR_DOWN:
			cout << "DOWN" << endl;
			break;
		case DIR_LEFT:
			cout << "LEFT" << endl;
			break;
		case DIR_RIGHT:
			cout << "RIGHT" << endl;
			break;
		case DIR_NEAR:
			cout << "NEAR" << endl;
			break;
		case DIR_FAR:
			cout << "FAR" << endl;
			break;
		default:
			//Print empty line if gesture in not detected
			cout << endl;
	}
}

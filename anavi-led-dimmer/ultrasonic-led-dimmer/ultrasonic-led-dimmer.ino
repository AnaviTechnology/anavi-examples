/**
 * Simple example for using HC-SR4 ultrasonic sensor with ATtiny85
 * and a potentiometer for a single channel non-addressable 12/24V
 * LED strip.
 *
 * Place an object within 5cm or less to turn on the LED strip.
 * Control the brightness manually using the potentiometer.
 */

// Iclude Arduino Pin Library
#include <NewPing.h>

// Pin for the button
#define BUT 1
// Pin to drive the LED strip
#define PWM 0
// Analog 1 corresponds to pin PB2
#define POT 1

#define echoPin 3
#define trigPin 4

NewPing sonar(trigPin, echoPin); 

void setup()
{
	pinMode(PWM, OUTPUT);
}

void loop()
{
	uint8_t distance = sonar.ping_cm();

	if (6 < distance)
	{
		// Turn off the LED strip
		analogWrite(PWM, 0);
	}
	else
	{
		// Read potentiometer value in range 0 - 1024 
		// and map it to the LED
		int potValue = map(analogRead(1), 0, 1023, 0, 255);
		analogWrite(PWM, potValue);
	}
}

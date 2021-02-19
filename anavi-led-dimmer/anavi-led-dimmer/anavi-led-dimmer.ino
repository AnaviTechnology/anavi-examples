// Pin for the button
#define BUT 1
// Pin to drive the LED strip
#define PWM 0
// Analog 1 corresponds to pin PB2
#define POT 1

int potValue = 0;

// the current state of LED
int ledState = HIGH;
// the previous state of button
int lastButtonState = HIGH;
// the current state of button
int currentButtonState = HIGH;

void setup()
{
	pinMode(BUT, INPUT_PULLUP);
	currentButtonState = digitalRead(BUT); 
	pinMode(PWM, OUTPUT);
}

void loop()
{
	lastButtonState = currentButtonState;
	// Check the status of the button
	currentButtonState = digitalRead(BUT);
	if ( (HIGH == lastButtonState) && (LOW == currentButtonState) ) 
	{
		// toggle state of LED
		ledState = !ledState;
	}
	
	if (LOW == ledState)
	{
		// Turn off the LED strip
		analogWrite(PWM, 0);
	}
	else
	{
		// Read potentiometer value in range 0 - 1024 
		// and map it to the LED
		potValue = map(analogRead(1), 0, 1023, 0, 255);
		analogWrite(PWM, potValue);
	}
}

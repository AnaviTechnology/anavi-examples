// Pin for the button
#define BUT 1
// Pin to drive the LED strip
#define PWM 0
// Analog 1 corresponds to pin PB2
#define POT 1
#define PIR 4
// Time period to keep the lights on
#define DELAY 10000

int potValue = 0;

// the current state of LED
int ledState = HIGH;
// the previous state of button
int lastButtonState = HIGH;
// the current state of button
int currentButtonState = HIGH;

int pirState = LOW;
unsigned long motionTime = 0;

void setup()
{
	pinMode(PWM, OUTPUT);
	pinMode(BUT, INPUT_PULLUP);
	currentButtonState = digitalRead(BUT);
	pinMode(PIR, INPUT_PULLUP);
  digitalWrite(PIR, LOW);

  //give the sensor some time to calibrate
  for(int iter = 0; iter < 30; iter++)
  {
    analogWrite(PWM, (iter*4));
    delay(1000);
  }
  // Turn off the lights at startup
  analogWrite(PWM, 0);
  delay(50);
}

void loop()
{
	// Allow to turn on or off the lights with the button
	// no matter what is the state of the PIR motion sensor
	lastButtonState = currentButtonState;
	// Check the status of the button
	currentButtonState = digitalRead(BUT);
	if ( (HIGH == lastButtonState) && (LOW == currentButtonState) )
	{
		// toggle state of LED
		ledState = !ledState;
		if (LOW != ledState)
		{
			motionTime = millis();
		}
	}

	if (HIGH == digitalRead(PIR))
	{
		if (LOW == pirState)
		{
			motionTime = millis();
			ledState = HIGH;
			pirState = HIGH;
		}
	}
	else
	{
		pirState = LOW;
	}

	if ((millis() - DELAY) > motionTime)
	{
		ledState = LOW;
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

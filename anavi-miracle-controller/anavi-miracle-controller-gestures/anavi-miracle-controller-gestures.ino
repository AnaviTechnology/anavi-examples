#include <FS.h> 
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include "Adafruit_APDS9960.h"

// For FastLED
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define LED_PIN1    12
#define LED_PIN2    14
#define NUM_LEDS    10
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120

Adafruit_APDS9960 apds;

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

int currentPosition = 1;

void updateColor(int numToFill)
{
  // Turn off all LEDs
  fill_solid(leds1, NUM_LEDS, CRGB::Black);
  // Turn on LEDs depending the gesture
  fill_rainbow(leds1, numToFill, 0, 7);

  // Turn off all LEDs
  fill_solid(leds2, NUM_LEDS, CRGB::Black);
  // Turn on LEDs depending the gesture
  fill_rainbow(leds2, numToFill, 0, 7);

  
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.println("ANAVI Miracle Controller");
  Serial.println("------------------------------");
  Serial.println("Control LED strip with gesture");
  Serial.println("------------------------------");

  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  updateColor(currentPosition);
  delay(100);
  Serial.println("LED strips initialized.");

  if(!apds.begin())
  {
    Serial.println("ERROR: Failed to initialize gesture sensor! Please check your wiring.");
  }
  else Serial.println("Gesture sensor initialized.");

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  apds.enableGesture(true);
}

void loop()
{
  //read a gesture from the device
  uint8_t gesture = apds.readGesture();
  if(APDS9960_LEFT == gesture)
  {
    if (1 < currentPosition)
    {
      Serial.println("Go left!");
      currentPosition -= 1;
      updateColor(currentPosition);
    }
    else
    {
      Serial.println("You have already reached the maximum on the left.");
    }
  }
  else if(APDS9960_RIGHT == gesture)
  {
    if (NUM_LEDS > currentPosition)
    {
      Serial.println("Go right!");
      currentPosition += 1;
      updateColor(currentPosition);
    }
    else
    {
      Serial.println("You have already reached the maximum on the right.");
    }
  }
}

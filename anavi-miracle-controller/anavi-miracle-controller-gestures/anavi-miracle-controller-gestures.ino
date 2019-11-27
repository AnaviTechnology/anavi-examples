#include <FS.h> 
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// For gesture sensor
#include "Adafruit_APDS9960.h"

// For OLED display
#include <U8g2lib.h>
// For I2C
#include <Wire.h>

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

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

Adafruit_APDS9960 apds;

const int pinButton = 0;
const int i2cDisplayAddress = 0x3c;

int currentPosition = 1;

void drawDisplay(const char *line1, const char *line2 = "", const char *line3 = "")
{
  // Write on OLED display
  // Clear the internal memory
  u8g2.clearBuffer();
  // Set appropriate font
  u8g2.setFont(u8g2_font_ncenR14_tr);
  u8g2.drawStr(0,14, line1);
  u8g2.setFont(u8g2_font_ncenR10_tr);
  u8g2.drawStr(0,39, line2);
  u8g2.setFont(u8g2_font_ncenR14_tr);
  u8g2.drawStr(0,64, line3);
  // Transfer internal memory to the display
  u8g2.sendBuffer();
}

bool isSensorAvailable(int sensorAddress)
{
  // Check if I2C sensor is present
  Wire.beginTransmission(sensorAddress);
  return 0 == Wire.endTransmission();
}

void checkDisplay()
{
  Serial.print("Mini I2C OLED Display at address ");
  Serial.print(i2cDisplayAddress, HEX);
  if (isSensorAvailable(i2cDisplayAddress))
  {
      Serial.println(": OK");
  }
  else
  {
      Serial.println(": N/A");
  }
}

void showPosition()
{
  String postext = "Position: " + String(currentPosition);
  drawDisplay("ANAVI", "Miracle Controller", postext.c_str());
}

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

  showPosition();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.println("ANAVI Miracle Controller");
  Serial.println("------------------------------");
  Serial.println("Control LED strip with gesture");
  Serial.println("------------------------------");

  Wire.begin();
  checkDisplay();
  u8g2.begin();
  delay(10);

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

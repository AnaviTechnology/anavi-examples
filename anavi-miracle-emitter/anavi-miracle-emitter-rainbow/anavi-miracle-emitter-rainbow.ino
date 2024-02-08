// For OLED display
#include <U8g2lib.h>
// For I2C
#include <Wire.h>
// For button release
#include <ezButton.h>

// For FastLED
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define LED_PIN    10
#define NUM_LEDS    10
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120

CRGB leds1[NUM_LEDS];

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

ezButton button(8);
const int i2cDisplayAddress = 0x3c;

int currentPosition = 1;

bool powerStatus = true;
uint8_t gHue = 0;

void drawDisplay(const char *line1, const char *line2 = "", const char *line3 = "")
{
  // Write on OLED display
  // Clear the internal memory
  u8g2.clearBuffer();
  // Set appropriate font
  u8g2.setFont(u8g2_font_ncenR12_tr);
  u8g2.drawStr(0,14, line1);
  u8g2.setFont(u8g2_font_ncenR14_tr);
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

void rainbow(CRGB *leds, uint8_t gHue, uint8_t gVal, int numToFill)
{
  CHSV hsv;
  hsv.hue = gHue;
  hsv.val = gVal;
  hsv.sat = 240;
  for( int i = 0; i < numToFill; i++) {
    leds[i] = hsv;
    hsv.hue += 7;
  }
}

void setup() {
  
  button.setDebounceTime(50);

  Serial.begin(115200);
  Serial.println();

  Serial.println("ANAVI Miracle Emitter");
  Serial.println("------------------------------");
  Serial.println("Rainbow Demo");
  Serial.println("------------------------------");

  Wire.begin();
  checkDisplay();
  u8g2.begin();
  delay(10);

  drawDisplay("Miracle Emitter", "Rainbow", "Demo");

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // Turn on lights
  rainbow(leds1, 0, 255, NUM_LEDS);

  FastLED.show();
  delay(100);
  Serial.println("LED strips have been initialized!");
}

void loop() {

  button.loop();

  if (true == button.isReleased())
  {
    Serial.println("Button pressed");
    powerStatus = !powerStatus;
  }

  if (true == powerStatus)
  {
    // Turn on lights
    rainbow(leds1, gHue, 255, NUM_LEDS);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);

        // do some periodic updates
    EVERY_N_MILLISECONDS(100)
    {
      // Slowly cycle the "base color" through the rainbow.
      // As gHue is uint8_t, this will automatically wrap around to
      // 0 after 255, so they will cover the whole color spectrum.
      gHue++;
    }
  }
  else
  {
    FastLED.clear();
    FastLED.show();
  }
}

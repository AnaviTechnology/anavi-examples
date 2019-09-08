/**
 * "The Breathanalyzer"
 *
 * This Arduino sketch is for a fun hobby project for adults:
 * do it yourselft breathalyzer with Lego parts, MQ-3 gas sensor,
 * mini OLED I2C SSD1306 display, ANAVI Gas Detector and male to
 * female jumper wires.
 *
 * MQ-3 is not very accurate and needs calibration. This is a
 * fun project, it is NOT a professional and reliable breathalyzer.
 * Drink responsibly. Never drink and drive.
 */

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// For OLED display
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Configure pins
const int pinAlarm = 16;
const int pinButton = 0;
const int pinCustom = 2;
const int pinGreen = 13;
const int pinYellow = 12;
const int pinRed = 14;
const int pinAdc = 0;

unsigned long gasPreviousMillis = 0;
// Read data from the analog MQ-3 sensor on every second
const long gasInterval = 1000;

void drawDisplay(const char *line1, const char *line2 = "", const char *line3 = "")
{
    // Write on OLED display
    // Clear the internal memory
    u8g2.clearBuffer();
    // Set appropriate font
    u8g2.setFont(u8g2_font_ncenR14_tr);
    u8g2.drawStr(0,14, line1);
    u8g2.drawStr(0,39, line2);
    u8g2.drawStr(0,64, line3);
    // Transfer internal memory to the display
    u8g2.sendBuffer();
}

void setColor(bool red, bool yellow, bool green)
{
  digitalWrite(pinRed, red);
  digitalWrite(pinYellow, yellow);
  digitalWrite(pinGreen, green);
}

void detectAlcohol()
{
  int breath = analogRead(pinAdc);
  // Calculate conductivity in pecents
  // The alcohol concetration in the air depends on the coductivity
  // If the analog MQ-3 sensor detects more alcohol gases
  // the conductivity will be higher
  int conductivity = round(((float)breath/1023)*100);

  // MQ-3 is not very accurate. The proper threshold point should be determined
  // after considering the temperature and humidity influence and testing in the
  // particular environment.
  String state = "Sober";
  if (breath <= 300)
  {
    setColor(LOW, LOW, HIGH);
  }
  else if (breath <= 450)
  {
    state="on 1-2 beers";
    setColor(LOW, HIGH, LOW);
  }
  else if (breath <= 600)
  {
    state="Drunk!";
    setColor(HIGH, LOW, LOW);
  }
  else
  {
    state="Totally wasted!";
    setColor(HIGH, LOW, LOW);
  }

  String line2 = "You are: ";
  String line3 = state ;

  // Print values in the serial output
  Serial.print("Alcohol value: ");
  Serial.println(breath);
  Serial.print("Conductivity: ");
  Serial.print(conductivity);
  Serial.println("%");
  Serial.print(line2);
   Serial.println(line3);

  drawDisplay("Breathanalyzer", line2.c_str(), line3.c_str());
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    
    u8g2.begin();
    delay(10);

    //LEDs
    pinMode(pinAlarm, OUTPUT);
    pinMode(pinRed, OUTPUT);
    pinMode(pinYellow, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    //Button
    pinMode(pinButton, INPUT);
    //Custom GPIO
    pinMode(pinCustom, OUTPUT);
}

void loop()
{
    const unsigned long currentMillis = millis();
    // Update the values from the analog MQ gas sensor
    if (gasInterval <= (currentMillis - gasPreviousMillis))
    {
        gasPreviousMillis = currentMillis;
        detectAlcohol();
    }
}

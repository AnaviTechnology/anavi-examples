#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// For OLED display
#include <U8g2lib.h>
// For I2C
#include <Wire.h>
// For gesture sensor
#include "Adafruit_APDS9960.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

Adafruit_APDS9960 apds;

const int pinButton = 0;
const int i2cDisplayAddress = 0x3c;

bool gestureSensorDetected = false;

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

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  Wire.begin();
  checkDisplay();

  u8g2.begin();

  delay(10);

  //Button
  pinMode(pinButton, INPUT);

  Serial.println("Testing APDS-9960...");
  drawDisplay("APDS-9960", "Test", "Application");

  if(!apds.begin())
  {
    Serial.println("ERROR: Failed to initialize gesture sensor! Please check your wiring.");
    drawDisplay("APDS-9960", "ERROR", "NO SENSOR");
  }
  else {
    Serial.println("Gesture sensor initialized.");
    drawDisplay("APDS-9960", "Initialized", "");
    gestureSensorDetected = true;
    //gesture mode will be entered once proximity mode senses something close
    apds.enableProximity(true);
    apds.enableGesture(true);
  }
}

void detectGesture()
{
  //read a gesture from the device
  uint8_t gesture = apds.readGesture();
  if(APDS9960_LEFT == gesture)
  {
    Serial.println("Left");
    drawDisplay("APDS-9960", "Left", "");
  }
  else if(APDS9960_RIGHT == gesture)
  {
    Serial.println("Right");
    drawDisplay("APDS-9960", "Right", "");
  }
  else if (APDS9960_DOWN == gesture)
  {
    Serial.println("Down");
    drawDisplay("APDS-9960", "Down", "");
  }
  else if (APDS9960_UP == gesture)
  {
    Serial.println("Up");
    drawDisplay("APDS-9960", "Up", "");
  }
}

void loop()
{
  if (false == digitalRead(pinButton))
  {
    Serial.println("Rebooting...");
    // Restart the board
    ESP.restart(); 
  }

  if (true == gestureSensorDetected)
  {
    detectGesture();
  }
}

/**
 * This Arduino sketch is a simple example for
 * ANAVI Gardening Controller with XIAO ESP32C3
 * and DS18B20 1-wire temperature sensor. It is
 * also compatible with ANAVI Thermometer.
 * 
 * This source code is suitable for microcontrollers
 * only. The data pin must be adjust if using with
 * another board.
 */

#if defined(ESP8266)
// This needs to be first, or it all crashes and burns...
#include <FS.h>
// From https://github.com/esp8266/Arduino
#include <ESP8266WiFi.h>
// Data wire is connected to pin 12
// on ANAVI Thermometer
#define ONE_WIRE_BUS 12
#else
// Data wire is connected to digital pin 2
// on ANAVI Gardening Controller
#define ONE_WIRE_BUS D10
#endif

// For OLED display
#include <U8g2lib.h>
// For I2C
#include <Wire.h>

// For DS18B20 temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to digital pin 2
#define ONE_WIRE_BUS D10

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int i2cDisplayAddress = 0x3c;

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

  Serial.println("Testing DS18B20 temperature sensor...");
  drawDisplay("DS18B20", "Loading...", "");

  // Initialize the DS18B20 sensor
  sensors.begin();
}

void loop()
{
  // Call sensors.requestTemperatures() to issue a global temperature request to all devices on the bus
  sensors.requestTemperatures(); 
  
  // Get the temperature in Celsius
  float tempC = sensors.getTempCByIndex(0);
  
  // Check if reading is valid
  if(tempC != DEVICE_DISCONNECTED_C) {
    // Show temperature on the mini OLED display
    String displayText = String(tempC) + "°C";
    drawDisplay("DS18B20", "Temperature: ", displayText.c_str());
    // Print temperature to serial monitor
    Serial.print("Temperature: ");
    Serial.println(displayText.c_str());
  } else {
    Serial.println("Error: Unable to read temperature from DS18B20 sensor. Is the sensor connected?");
    drawDisplay("DS18B20", "Error", "Is sensor OK?");
  }
  
  // Wait for a moment before taking the next reading
  delay(1000);
}

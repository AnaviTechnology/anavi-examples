#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// For DHT22 temperature and humidity sensor
#include <DHT.h>
// For OLED display
#include <U8g2lib.h>
#include <Wire.h>
// For DS18B20 (waterproof) temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN  2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configure pins
const int pinAlarm = 16;
const int pinButton = 0;

unsigned long sensorPreviousMillis = 0;
const long sensorInterval = 3000;

// Set temperature coefficient for calibration depending on an empirical research with
// comparison to DS18B20 and other temperature sensors. You may need to adjust it for the
// specfic DHT22 unit on your board
const float temperatureCoef = 0.9;

float dhtTemperature = 0;
float dhtHumidity = 0;
float dsTemperature = 0;

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

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();
    u8g2.begin();
    dht.begin();
    sensors.begin();

    delay(10);

    //LED
    pinMode(pinAlarm, OUTPUT);
    //Button
    pinMode(pinButton, INPUT);

    drawDisplay("ANAVI Thermometer");
}

void loop()
{
    const unsigned long currentMillis = millis();
    if (sensorInterval <= (currentMillis - sensorPreviousMillis))
    {
        sensorPreviousMillis = currentMillis;

        float temp = dht.readTemperature();
        // Adjust temperature depending on the calibration coefficient
        temp = temp*temperatureCoef;
        float humidity = dht.readHumidity();

        if (!isnan(humidity) && !isnan(temp))
        {
            dhtTemperature = temp;
            dhtHumidity = humidity;
        }
        String air="Air "+String(dhtTemperature, 1)+"C ";
        Serial.println(air);
        String hum="Humidity "+String(dhtHumidity, 0)+"%";
        Serial.println(hum);

        String water="";
        if (0 < sensors.getDeviceCount())
        {
            sensors.requestTemperatures();
            for (int ds=0; ds < sensors.getDeviceCount(); ds++)
            {
              float wtemp = sensors.getTempCByIndex(ds);
              dsTemperature = wtemp;
              water="Water "+ String(ds) + ": "+String(dsTemperature,1)+"C";
              Serial.println(water);
            }
        }
        // Show on the display only the value from the last DS18B20 sensor
        drawDisplay(air.c_str(), hum.c_str(), water.c_str());
    }
}

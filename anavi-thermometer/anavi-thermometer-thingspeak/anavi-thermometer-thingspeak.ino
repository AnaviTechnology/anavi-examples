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

// For MQTT
#include <PubSubClient.h>        //https://github.com/knolleary/pubsubclient

//WiFi
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "mqtt.thingspeak.com";

// Set MQTT user name (it could be anything):
char mqttUserName[] = "ANAVI Thermometer";
// Change this your MQTT API Key from Account > MyProfile.
char mqttPass[] = "";
// Change to your channel Write API Key:
char writeAPIKey[] = "";
// Replace it with your thingspeak.com channel ID:
long channelID = 0;

// MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN  2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configure the interval in milliseconds to read data
// from the sensors and to publish it to thingspeak.com
// 1 second is 1000milliseconds, for example:
// 15000 is 15 seconds
// Keep in mind thingspeak.com has limits for free accounts
const long publishInterval = 15000;
// Interval for reading DHT22 data
const long sensorInterval = 3000;
unsigned long sensorPreviousMillis = 0;

// Set temperature coefficient for calibration depending on an empirical research with
// comparison to DS18B20 and other temperature sensors. You may need to adjust it for the
// specfic DHT22 unit on your board
const float temperatureCoef = 1;

//Configure I2C sensor for light
const int sensorBH1750 = 0x23;

// For random generation of MQTT client ID
static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";

String txtTemperature = "";
String txtHumidity = "";
String txtWater="";
String txtLight = "";

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

void connectWiFi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  char clientID[9];

  // Loop until we're reconnected
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }
    clientID[8]='\0';

    // Connect to the MQTT broker
    if (mqttClient.connect(clientID, mqttUserName, mqttPass)) 
    {
      Serial.println("connected");
    } else 
    {
      Serial.print("failed, rc=");
      // Print to know why the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

bool isSensorAvailable(int sensorAddress)
{
  // Check if I2C sensor is present
  Wire.beginTransmission(sensorAddress);
  return 0 == Wire.endTransmission();
}

void sensorWriteData(int i2cAddress, uint8_t data)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(data);
  Wire.endTransmission();
}

String readLight()
{
  // Power on sensor
  sensorWriteData(sensorBH1750, 0x01);
  // Set mode continuously high resolution mode
  sensorWriteData(sensorBH1750, 0x10);

  uint16_t tempAmbientLight;

  Wire.requestFrom(sensorBH1750, 2);
  tempAmbientLight = Wire.read();
  tempAmbientLight <<= 8;
  tempAmbientLight |= Wire.read();
  // s. page 7 of datasheet for calculation
  tempAmbientLight = tempAmbientLight/1.2;

  char convert[80];
  sprintf(convert, "%u", tempAmbientLight);
  return String(convert);
}

void mqttpublish(String temperature, String humidity, String water, String light)
{
  // Create data string to send to ThingSpeak
  String data = String("field1=" + temperature + "&field2=" + humidity + "&field3=" + water + "&field4=" + light);
  int length = data.length();
  char msgBuffer[length];
  data.toCharArray(msgBuffer,length+1);
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString ="channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length=topicString.length();
  char topicBuffer[length];
  topicString.toCharArray(topicBuffer,length+1);
 
  mqttClient.publish(topicBuffer, msgBuffer);
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

  drawDisplay("ANAVI Thermometer");

  connectWiFi();
  mqttClient.setServer(mqtt_server, 1883);
}

void loop()
{
  // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected())
  {
    reconnect();
  }

  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  if (!isnan(humidity) && !isnan(temp))
  {
    // Adjust temperature depending on the calibration coefficient
    temp = temp*temperatureCoef;
    
    txtTemperature = String(temp, 1);
    txtHumidity = String(humidity, 0);
  }

  String air = "";
  if (0 < txtTemperature.length())
  {
    air="Air "+txtTemperature+"C ";
    Serial.println(air);
  }

  String hum = "";
  if (0 < txtHumidity.length())
  {
    hum="Humidity "+txtHumidity+"%";
    Serial.println(hum);
  }

  String water="";
  if (0 < sensors.getDeviceCount())
  {
    sensors.requestTemperatures();
    txtWater = String(sensors.getTempCByIndex(0),1);
    water="Water "+txtWater+"C";
    Serial.println(water);
  }
  drawDisplay(air.c_str(), hum.c_str(), water.c_str());

  // Publish data to thingspeak.com over MQTT
  const unsigned long currentMillis = millis();
  if (publishInterval <= (currentMillis - sensorPreviousMillis))
  {
    // Read light only if sensor is attached
    if (isSensorAvailable(sensorBH1750))
    {
        txtLight = readLight();
        Serial.println("Light "+txtLight+" Lux");
    }

    sensorPreviousMillis = currentMillis;
    mqttpublish(txtTemperature, txtHumidity, txtWater, txtLight);
  }

  // Read data from the sensor every 3 seconds
  delay(sensorInterval);
}

#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#define PIN_FAN         12
#define PIN_FAN_BUTTON  13

bool fanOn = false;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  delay(10);

  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_FAN_BUTTON, INPUT);
  
  Serial.println("ANAVI Fume Extractor");
  Serial.println("--------------------");
  Serial.println("Fan Example");
  Serial.println("--------------------");
  
  delay(1000);
  digitalWrite(PIN_FAN, LOW);
  fanOn = false;
}

void loop()
{
  if (LOW == digitalRead(PIN_FAN_BUTTON))
  {
    Serial.print("Fan: ");
    if (false == fanOn)
    {
      digitalWrite(PIN_FAN, HIGH);
      fanOn = true;
      Serial.println("ON");
    }
    else
    {
      digitalWrite(PIN_FAN, LOW);
      fanOn = false;
      Serial.println("OFF");
    }
    // Give the fan 3 seconds between swithing it again
    delay(3000);
  }
}

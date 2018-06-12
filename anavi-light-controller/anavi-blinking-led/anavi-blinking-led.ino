#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// Configure pins
const int pinAlarm = 16;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  //LED
  pinMode(pinAlarm, OUTPUT);
  
  Serial.println("Blinking LED demo on ANAVI Light Controller");
}

void loop() {
  // Blinking LED
  while(true)
  {
      digitalWrite(pinAlarm, HIGH);
      delay(100);
      digitalWrite(pinAlarm, LOW);
      delay(100);
  }
}

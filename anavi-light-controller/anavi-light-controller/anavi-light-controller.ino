#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// Configure pins
const int pinAlarm = 16;
const int pinButton = 0;
const int pinLedRed = 12;
const int pinLedGreen = 13;
const int pinLedBlue = 14;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("ANAVI Light Controller");

    //LED
    pinMode(pinAlarm, OUTPUT);
    //Button
    pinMode(pinButton, INPUT);

    //RGB LED Strip
    pinMode(pinLedRed, OUTPUT);
    pinMode(pinLedGreen, OUTPUT);
    pinMode(pinLedBlue, OUTPUT);

    digitalWrite(pinAlarm, HIGH);

    // Turn all 3 colors of the LED strip
    // This way the setup and testing will be easier
    analogWrite(pinLedRed, 255);
    analogWrite(pinLedGreen, 255);
    analogWrite(pinLedBlue, 255);
}

void loop()
{
    // put your main code here, to run repeatedly:
    int red = random(255);
    analogWrite(pinLedRed, red);
    int green = random(255);
    analogWrite(pinLedGreen, green);
    int blue = random(255);
    analogWrite(pinLedBlue, blue);
    delay(3000);
}

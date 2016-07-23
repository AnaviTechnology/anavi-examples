#include <wiringPi.h>
#include <stdio.h>

//Pin 29 on Raspberry Pi corresponds to BCM GPIO 5 and wiringPi pin 21
#define RelayPin 21

int main(void)
{
  if(-1 == wiringPiSetup())
  {
      printf("setup wiringPi failed!");
      return 1;
  }

  //Set GPIO pin
  pinMode(RelayPin, OUTPUT);

  //Turn on the relay
  digitalWrite(RelayPin, HIGH);
  //Wait for 3 seconds
  delay(3000);
  //Turn off the relay
  digitalWrite(RelayPin, LOW);

  return 0;
}

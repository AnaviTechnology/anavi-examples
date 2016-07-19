#include <wiringPi.h>
#include <stdio.h>

//Pin 31 on Raspberry Pi corresponds to BCM GPIO 6 and wiringPi pin 22
#define RelayPin 22

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

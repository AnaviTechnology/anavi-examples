#include <wiringPi.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

//Pin 33 on Raspberry Pi corresponds to BCM GPIO 13 and wiringPi pin 22
#define Pin0 23
//Pin 35 on Raspberry Pi corresponds to BCM GPIO 19 and wiringPi pin 24
#define Pin1 24
//Pin 37 on Raspberry Pi corresponds to BCM GPIO 26 and wiringPi pin 25
#define Pin2 25

void led(int red, int green, int blue)
{
  digitalWrite(Pin0, red);
  digitalWrite(Pin1, green);
  digitalWrite(Pin2, blue);
}

void close()
{
  printf("\nTurning off lights...\n");
  led(LOW, LOW, LOW);
  exit(0);
}

int main(void)
{
  signal(SIGINT, close);

  if(-1 == wiringPiSetup())
  {
      printf("setup wiringPi failed!");
      return 1;
  }

  //Set GPIO pins
  pinMode(Pin0, OUTPUT);
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);

  //Play a sound until the user closes the app
  while(1)
  {
    led(HIGH, LOW, LOW);
    delay(3000);
    led(LOW, HIGH, LOW);
    delay(3000);
    led(LOW, LOW, HIGH);
    delay(3000);
  }

  return 0;
}

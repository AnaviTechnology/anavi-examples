/*
 * Simple Arduino sketch for Ai-M61-32S Kit
 * with Bouffalo BL618 RISC-V microcontroller.
 * 
 * Known issues with digitalWrite for 12, 14 and 27
 * using arduino-bouffalo version 1.0.5:
 * https://github.com/bouffalolab/arduino-bouffalo/issues/6
 *
 */

int ledWhite = 29;
int ledBlue = 15;

void setup() {
  Serial.begin(9600);

  pinMode(ledWhite, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  Serial.println("");
  Serial.println("Hello, RISC-V World!");
}
void loop() {
  Serial.println("Turn on the bright white LED.");
  digitalWrite(ledWhite, HIGH);
  digitalWrite(ledBlue, LOW);
  delay(1000);
  Serial.println("Turn on the blue color from the RGB LED.");
  digitalWrite(ledWhite, LOW);
  digitalWrite(ledBlue, HIGH);
  delay(1000);
}

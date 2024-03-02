/**
 * This Arduino sketch is a simple example for
 * ANAVI Gardening Controller with XIAO ESP32C3
 * and 3 capacitive soil moisture sensors. It is
 * suitable for microcontrollers only and the pins
 * must be adjust if using with another board.
 */

#define SOILSEN1 A0
#define SOILSEN2 A1
#define SOILSEN3 A2

#define LED1 D3

void setup() {

  // open serial port, set the baud rate as 115200 bps
  Serial.begin(115200);
  
  // ADC
  pinMode(SOILSEN1, INPUT);
  pinMode(SOILSEN2, INPUT);
  pinMode(SOILSEN3, INPUT);

  // initialize digital pin led as an output
  pinMode(LED1, OUTPUT);
}

int calculatePercentage(int start, int end, int current) {
  // Return 0% if the value is out of range
  if (end > current) {
    return 0;
  }

  // Calculate the total range
  double totalRange = end - start;
  
  // Calculate the current position within the range
  double currentPosition = current - start;
  
  // Calculate the percentage and round it to the nearest integer
  int percentage = static_cast<int>((currentPosition / totalRange) * 100.0 + 0.5);
  
  // Ensure the percentage is within [0, 100]
  if (percentage < 0) {
      return 0;
  } else if (percentage > 100) {
      return 100;
  } else {
      return percentage;
  }
}

void printSoilMoisture(uint8_t id, uint8_t pin) {
  uint32_t soil = analogReadMilliVolts(pin);
  int percentage = calculatePercentage(2250, 1050, soil);
  Serial.print("Soil moisture ");
  Serial.print(id);
  Serial.print(": ");
  Serial.print(percentage);
  Serial.println("%");
}

void loop() {

  printSoilMoisture(1, SOILSEN1);
  printSoilMoisture(2, SOILSEN2);
  printSoilMoisture(3, SOILSEN3);

  // turn the LED on
  digitalWrite(LED1, HIGH);
  delay(500);
  // turn the LED off
  digitalWrite(LED1, LOW);   
  delay(500);
}
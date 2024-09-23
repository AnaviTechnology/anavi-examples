// Pin definitions
// Motor A
// PWM pin for Motor A speed
const int PWMA = 5;
// Motor A direction pin 1
const int AIN1 = 4;
// Motor A direction pin 2
const int AIN2 = 3;

// Motor B
// PWM pin for Motor B speed
const int PWMB = 2;
// Motor B direction pin 1
const int BIN1 = 1;
// Motor B direction pin 2
const int BIN2 = 0;

// Standby
const int STBY = 6; 

int tests[] = {80, 100, 160, 255};

// Function prototypes
void driveMotorA(int speed, bool direction);
void driveMotorB(int speed, bool direction);
void stopMotors();

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set all the motor control pins to output
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  pinMode(STBY, OUTPUT);
  
  // Enable the driver (disable standby)
  digitalWrite(STBY, HIGH);
}

void loop() {
  int arrayLength = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < arrayLength; i++) {

    Serial.print("Start motors forward at: ");
    int percentage = (tests[i]*100) / 255;
    Serial.print(percentage);
    Serial.println("%");
    driveMotorA(tests[i], true);
    driveMotorB(tests[i], true);

    delay(2000);

    Serial.print("Start motors backward at: ");
    Serial.print(percentage);
    Serial.println("%");
    driveMotorA(tests[i], false);
    driveMotorB(tests[i], false);

    delay(2000);

    Serial.println("Stop motors");
    // Stop both motors
    stopMotors();
    delay(2000);
    // Disable standby to save power
    digitalWrite(STBY, HIGH);
  }
}

// Drive Motor A
void driveMotorA(int speed, bool direction) {
  // Set direction
  if (direction) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  // Set speed (0-255)
  analogWrite(PWMA, speed);
}

// Drive Motor B
void driveMotorB(int speed, bool direction) {
  // Set direction
  if (direction) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  // Set speed (0-255)
  analogWrite(PWMB, speed);
}

// Stop both motors
void stopMotors() {
  // Set speed to 0 for both motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);

  // Disable standby to save power
  digitalWrite(STBY, LOW);
}

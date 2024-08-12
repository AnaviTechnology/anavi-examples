#include <Wire.h>

// Define the I2C address of the CYPD3177
#define CYPD3177_I2C_ADDR 0x08

// Define the command/register addresses
#define CYPD_PD_STATUS_REG          0x1008
#define CYPD_DATA_MEMORY_START_ADDR 0x1800
#define CYPD_SELECT_SINK_PDO_CMD    0x1005
#define CYPD_PD_RESPONSE_CMD		0x1400

const uint32_t pdos[] = {
  0x000190C8, // Fixed 5V, 2A
  0x0002D0C8, // Fixed 9V, 2A
  0x0003C0E1, // Fixed 12V, 2.25A
  0x0004B0B4, // Fixed 15V, 1.8A 
  0x00064064  // Fixed 20V, 1.5A
};

String text[] = {
  "5V, 2A",
  "9V, 2A",
  "12V, 2.25A",
  "15V, 1.8A",
  "20V, 1.5A"
};

void setup() {
  // Start the I2C communication
  Wire.begin();

  // Start serial communication for debugging
  Serial.begin(115200);

  // Give some time for startup
  delay(1000);

  Serial.println("CYPD3177 Test Sketch");

}

void loop() {
  // Check PD_STATUS register
  if (checkPDStatus()) {
    Serial.println("PD established contract.");

    // Calculate the number of elements in the array
    size_t num_elements = sizeof(pdos) / sizeof(pdos[0]);
    size_t i = 0;
    while (true) {
        Serial.println("Voltage: " + text[i]);
        selectPDO(pdos[i]);
        delay(5000);
        i++; // Increment the index
        // Restart the loop when the last element is reached
        if (i == num_elements) {
            i = 0;
        }
    }
  } else {
    Serial.println("PD Status does not indicate an established contract.");
  }
}

// Function to check the PD_STATUS register
bool checkPDStatus() {
  uint8_t status[4];
 
  Wire.beginTransmission(CYPD3177_I2C_ADDR);
  Wire.write(CYPD_PD_STATUS_REG & 0xFF);  // Low byte of register
  Wire.write((CYPD_PD_STATUS_REG >> 8) & 0xFF);  // High byte of register
  Wire.endTransmission();

  if (Wire.requestFrom(CYPD3177_I2C_ADDR, 4) != 4) {
    Serial.println("Error: Could not read the complete PD status.");
    return false;
  }

  status[0] = Wire.read();  // Read status bytes
  status[1] = Wire.read();
  status[2] = Wire.read();
  status[3] = Wire.read();
 
  // Check if status is 00 A4 05 00
  if (status[0] == 0x00 && status[1] == 0xA4 && status[2] == 0x05 && status[3] == 0x00) {
    return true;
  }
 
  return false;
}

void selectPDO(uint32_t PDO) {
  // The SNKP signature
  const uint8_t snkp[4] = {0x50, 0x4B, 0x4E, 0x53};
  
  // Start the transmission to the CYPD3177
  Wire.beginTransmission(CYPD3177_I2C_ADDR);
  Wire.write(CYPD_DATA_MEMORY_START_ADDR & 0xFF);  // Low byte of memory start address
  Wire.write((CYPD_DATA_MEMORY_START_ADDR >> 8) & 0xFF);  // High byte of memory start address
 
  // Send "SNKP" signature
  Wire.write(snkp, 4);

  uint32_t pdos[] = { PDO };
  for (int i = 0; i < 7; i++) {
    if (i < sizeof(pdos) / sizeof(pdos[0])) {
      Wire.write((uint8_t*)&pdos[i], 4);
    } else {
      Wire.write(0x00);
      Wire.write(0x00);
      Wire.write(0x00);
      Wire.write(0x00);
    }
  }

  byte status = Wire.endTransmission();

  // Start the transmission to the CYPD3177
  Wire.beginTransmission(CYPD3177_I2C_ADDR);
  Wire.write(CYPD_SELECT_SINK_PDO_CMD & 0xFF);  // Low byte of command
  Wire.write((CYPD_SELECT_SINK_PDO_CMD >> 8) & 0xFF);  // High byte of command
  Wire.write(1);
  Wire.endTransmission();
}

// Function to replace the default sink PDOs
void setSinkPDOs() {
  // The SNKP signature
  const uint8_t snkp[4] = {0x50, 0x4B, 0x4E, 0x53};

  // Start the transmission to the CYPD3177
  Wire.beginTransmission(CYPD3177_I2C_ADDR);
  Wire.write(CYPD_DATA_MEMORY_START_ADDR & 0xFF);  // Low byte of memory start address
  Wire.write((CYPD_DATA_MEMORY_START_ADDR >> 8) & 0xFF);  // High byte of memory start address
 
  // Send "SNKP" signature
  Wire.write(snkp, 4);

  // Send the PDOs
  for (int i = 0; i < 7; i++) {
    if (i < sizeof(pdos) / sizeof(pdos[0])) {
      Wire.write((uint8_t*)&pdos[i], 4);
    } else {
      Wire.write(0x00);
      Wire.write(0x00);
      Wire.write(0x00);
      Wire.write(0x00);
    }
  }

  byte status = Wire.endTransmission();
  Serial.println(status);

  Serial.println("Sink PDOs updated.");
}

// Function to select a specific Sink PDO
void selectSinkPDO(uint8_t index) {
  if (index > 7) {
    Serial.println("Error: Invalid PDO index.");
    return;
  }

  // Start the transmission to the CYPD3177
  Wire.beginTransmission(CYPD3177_I2C_ADDR);
  Wire.write(CYPD_SELECT_SINK_PDO_CMD & 0xFF);  // Low byte of command
  Wire.write((CYPD_SELECT_SINK_PDO_CMD >> 8) & 0xFF);  // High byte of command
  Wire.write(index);  // PDO index
  Wire.endTransmission();

  Serial.print("Selected Sink PDO index: ");
  Serial.println(index);

  Wire.beginTransmission(CYPD3177_I2C_ADDR);
  Wire.write(CYPD_PD_RESPONSE_CMD & 0xFF);  // Low byte of register
  Wire.write((CYPD_PD_RESPONSE_CMD >> 8) & 0xFF);  // High byte of register
  Wire.endTransmission();

  if (Wire.requestFrom(CYPD3177_I2C_ADDR, 4) != 4) {
    Serial.println("Error: Could not read the complete PD status.");
    return false;
  }

  uint8_t status = Wire.read();
  if (status == 0x02) {
    Serial.println("Select PDO successful.");
  }
  else
  {
    Serial.println("ERROR: Select PDO failed.");
  }
}
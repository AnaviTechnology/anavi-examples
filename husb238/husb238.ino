#include <Wire.h>
#include "Adafruit_HUSB238.h"

Adafruit_HUSB238 husb238;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Adafruit HUSB238 Test Sketch");

  // Initialize the HUSB238
  if (husb238.begin(HUSB238_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("HUSB238 initialized successfully.");
  } else {
    Serial.println("Couldn't find HUSB238, check your wiring?");
    while (1);
  }
}

void loop() {
  delay(1000);  // Add a delay to prevent flooding the serial output
  Serial.println(F("----------------------------------------------"));
  
  // Determine whether attached or unattached
  bool attached = husb238.isAttached();
  Serial.print("Attachment Status: ");
  Serial.println(attached ? "Attached" : "Unattached");

  if (! attached) return;
  
  // Test getCCStatus function
  bool ccStatus = husb238.getCCdirection();
  Serial.print("CC Direction: ");
  Serial.println(ccStatus ? "CC1 connected" : "CC2 Connected");

  // Check if we can get responses to our PD queries!
  HUSB238_ResponseCodes pdResponse = husb238.getPDResponse();
  Serial.print("USB PD query response: ");
  switch (pdResponse) {
    case NO_RESPONSE:
      Serial.println("No response");
      break;
    case SUCCESS:
      Serial.println("Success");
      break;
    case INVALID_CMD_OR_ARG:
      Serial.println("Invalid command or argument");
      break;
    case CMD_NOT_SUPPORTED:
      Serial.println("Command not supported");
      break;
    case TRANSACTION_FAIL_NO_GOOD_CRC:
      Serial.println("Transaction fail");
      break;
    default:
      Serial.println("Unknown response code");
      break;
  }

  if (pdResponse != SUCCESS)
    return;

  // Is there a default 5V 'contract' voltage available
  bool contractV = husb238.get5VContractV();
  Serial.print("5V Contract Voltage: ");
  Serial.print(contractV ? "5V" : "Other");
  
  // How much current can we get?
  HUSB238_5VCurrentContract contractA = husb238.get5VContractA();
  Serial.print(" & Current: ");
  switch (contractA) {
    case CURRENT5V_DEFAULT:
      Serial.println("Default current");
      break;
    case CURRENT5V_1_5_A:
      Serial.println("1.5A");
      break;
    case CURRENT5V_2_4_A:
      Serial.println("2.4A");
      break;
    case CURRENT5V_3_A:
      Serial.println("3A");
      break;
    default:
      Serial.println("Unknown current");
      break;
  }


  // What is the actual voltage being output right now?
  HUSB238_VoltageSetting srcVoltage = husb238.getPDSrcVoltage();
  Serial.print("Source Voltage: ");
  switch (srcVoltage) {
    case UNATTACHED:
      Serial.println("Unattached");
      break;
    case PD_5V:
      Serial.println("5V");
      break;
    case PD_9V:
      Serial.println("9V");
      break;
    case PD_12V:
      Serial.println("12V");
      break;
    case PD_15V:
      Serial.println("15V");
      break;
    case PD_18V:
      Serial.println("18V");
      break;
    case PD_20V:
      Serial.println("20V");
      break;
    default:
      Serial.println("Unknown voltage setting");
      break;
  }

  // What is the max current available right now?
  HUSB238_CurrentSetting srcCurrent = husb238.getPDSrcCurrent();
  Serial.print("Source Current: ");
  printCurrentSetting(srcCurrent);
  Serial.println();

  // What voltages and currents are available from this adapter?
  Serial.println("Available PD Voltages and Current Detection Test:");
  for (int i = PD_SRC_5V; i <= PD_SRC_20V; i++) {
    bool voltageDetected = husb238.isVoltageDetected((HUSB238_PDSelection)i);

    switch ((HUSB238_PDSelection)i) {
      case PD_SRC_5V:
        Serial.print("5V");
        break;
      case PD_SRC_9V:
        Serial.print("9V");
        break;
      case PD_SRC_12V:
        Serial.print("12V");
        break;
      case PD_SRC_15V:
        Serial.print("15V");
        break;
      case PD_SRC_18V:
        Serial.print("18V");
        break;
      case PD_SRC_20V:
        Serial.print("20V");
        break;
      default:
        continue;
    }
    Serial.print(voltageDetected ? " Available" : " Unavailable");

    // Loop over currents if voltage is detected
    if (voltageDetected) {
      HUSB238_CurrentSetting currentDetected = husb238.currentDetected((HUSB238_PDSelection)i);
      Serial.print(" - Max current: ");
      printCurrentSetting(currentDetected);
    }
    Serial.println();
  }

  // Override whatever the jumpers on the board say, and get a specific voltage!
  husb238.selectPD(PD_SRC_5V);  // Select 5V
  // Uncomment one of the following lines to select a different PD:
  // husb238.selectPD(PD_SRC_9V);  // Select 9V
  // husb238.selectPD(PD_SRC_12V);  // Select 12V
  //husb238.selectPD(PD_SRC_15V);  // Select 15V
  // husb238.selectPD(PD_SRC_18V);  // Select 18V
  // husb238.selectPD(PD_SRC_20V);  // Select 20V

  // Perform the actual PD voltage request!
  husb238.requestPD();

  // Test getSelectedPD function
  HUSB238_PDSelection selectedPD = husb238.getSelectedPD();
  Serial.print("Currently Selected PD Output: ");
  switch (selectedPD) {
    case PD_NOT_SELECTED:
      Serial.println("Not Selected");
      break;
    case PD_SRC_5V:
      Serial.println("5V");
      break;
    case PD_SRC_9V:
      Serial.println("9V");
      break;
    case PD_SRC_12V:
      Serial.println("12V");
      break;
    case PD_SRC_15V:
      Serial.println("15V");
      break;
    case PD_SRC_18V:
      Serial.println("18V");
      break;
    case PD_SRC_20V:
      Serial.println("20V");
      break;
    default:
      Serial.println("Unknown");
      break;
  }
}


void printCurrentSetting(HUSB238_CurrentSetting srcCurrent) {
  switch (srcCurrent) {
    case CURRENT_0_5_A:
      Serial.print("0.5A ");
      break;
    case CURRENT_0_7_A:
      Serial.print("0.7A ");
      break;
    case CURRENT_1_0_A:
      Serial.print("1.0A ");
      break;
    case CURRENT_1_25_A:
      Serial.print("1.25A ");
      break;
    case CURRENT_1_5_A:
      Serial.print("1.5A ");
      break;
    case CURRENT_1_75_A:
      Serial.print("1.75A ");
      break;
    case CURRENT_2_0_A:
      Serial.print("2.0A ");
      break;
    case CURRENT_2_25_A:
      Serial.print("2.25A ");
      break;
    case CURRENT_2_50_A:
      Serial.print("2.50A ");
      break;
    case CURRENT_2_75_A:
      Serial.print("2.75A ");
      break;
    case CURRENT_3_0_A:
      Serial.print("3.0A ");
      break;
    case CURRENT_3_25_A:
      Serial.print("3.25A ");
      break;
    case CURRENT_3_5_A:
      Serial.print("3.5A ");
      break;
    case CURRENT_4_0_A:
      Serial.print("4.0A ");
      break;
    case CURRENT_4_5_A:
      Serial.print("4.5A ");
      break;
    case CURRENT_5_0_A:
      Serial.print("5.0A ");
      break;
    default:
      break;
  }
}

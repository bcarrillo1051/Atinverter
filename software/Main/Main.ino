/*
  Main.ino - Main code running on Atmega chip
  Created by Zach Kwast, 3/4/25
  Released into the public domain.
*/

#include "Atinverter.h"
#include "Wire.h"
#include "ZMPT101B.h"

Atinverter atinverter;
ZMPT101B adcSensorSixty(60.0);
ZMPT101B adcSensorFifty(50.0);

// Variable initialization 
const int ledPin = 3;
int state = 0;
int sixty_once = 0;
int fifty_once = 0;
int prev_state = 0;

// float DC_volt = 5.23;
// float DC_amp = 6.8;
// float AC_volt = 35.21;
// float AC_amp = 0.95;

void setup() {

  // Initializes I2C communication
  Wire.begin(0x8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);

  // Initializes test LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Initializes pins
  atinverter.setUpPinMode();
  Serial.println("Setup Done");

  voltageSensor.setSensitivity(SENSITIVITY);
  atinverter.setUpSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB

  atinverter.startPWM(false);
}

// The function that completes when I2C communication 
// data is given to this device
void receiveEvent(int howMany) {
  while(Wire.available()) {
    state = Wire.read();
  }
}

// The function that completes when I2C communication 
// data is requested from this device
void requestEvent() {
  Wire.write((byte*)&DC_volt, sizeof(DC_volt));
  Wire.write((byte*)&DC_amp, sizeof(DC_amp));
  Wire.write((byte*)&AC_volt, sizeof(AC_volt));
  Wire.write((byte*)&AC_amp, sizeof(AC_amp));
}

void loop() {
  if (prev_state != state) {
    Serial.println(state);
    prev_state = state;
  }

  // Read all DC voltages and currents
  //unsigned long start = millis();
  DC_volt_avg = atinverter.readAvg(atinverter.readVdc());
  DC_amp_avg = atinverter.readAvg(atinverter.readIdc());
  // unsigned long time = millis() - start;
  // Serial.print(time)
  // Serial.println(" ms")

  // Read the AC values from the SPI ADC
  // 0x00 represents channel 0 on ADC -> AC voltage channel
  // 0x08 represents channel 1 on ADC -> AC current channel
  uint16_t ADC_volt = atinverter.readADC(0x00);
  uint16_t ADC_amp = atinverter.readADC(0x08);

  // Turns LED Off
  if (state == 0) {
    digitalWrite(ledPin, LOW);
    fifty_once = 0;
    sixty_once = 0;
  }
  // Turns LED On 
  else if (state == 1) {
    digitalWrite(ledPin, HIGH);
    fifty_once = 0;
    sixty_once = 0;
  }
  else if (state == 2) {
    // Cycles proreset, needs to be a function
    digitalWrite(PRORESET_PIN, HIGH);
    delay2(3000);
    digitalWrite(PRORESET_PIN, LOW);
    fifty_once = 0;
    sixty_once = 0;
  }
  else if (state == 3) {
    fifty_once = 0;
    sixty_once = 0;
  }
  else if (state == 4) {
    fifty_once = 0;
    sixty_once = 0;
  }
  // 50 Hz Sin Wave Output
  else if (state == 5) {
    if (fifty_once == 0) {
      atinverter.startPWM(true);
      fifty_once = 1;
    }
    sixty_once = 0;
    // Get the converted average value of AC voltage and current
    float AC_volt = adcSensorSixty.getRmsVoltage(1, ADC_volt);
    float AC_amp = adcSensorSixty.getRmsCurrent(1, ADC_amp);

    // Rounding the values to have 2 decimal places
    float round_AC_volt = round(AC_volt*100) / 100;
    float round_AC_amp = round(AC_amp*100) / 100;
  } 
  // 60 Hz Sin Wave Output
  else if (state == 6) {
    if (sixty_once == 0) {
      atinverter.startPWM(false);
      sixty_once = 1;
    }
    fifty_once = 0;
    // Get the converted average value of AC voltage and current
    float AC_volt = adcSensorFifty.getRmsVoltage(1, ADC_volt);
    float AC_amp = adcSensorFifty.getRmsCurrent(1, ADC_amp);

    // Rounding the values to have 2 decimal places
    float round_AC_volt = round(AC_volt*100) / 100;
    float round_AC_amp = round(AC_amp*100) / 100;
  }
  else if (state == 7) {
    fifty_once = 0;
    sixty_once = 0;
  }
  else {
    Serial.println("Invalid State");
  }
}

Serial.print(F("DC Voltage: ")); Serial.println(DC_volt_avg);
  Serial.print(F("DC Current: ")); Serial.println(DC_amp_avg);
  Serial.print(F("AC Voltage: ")); Serial.println(round_AC_volt);
  Serial.print(F("AC Current: ")); Serial.println(round_AC_amp);
  Serial.println();
  delay2(10);

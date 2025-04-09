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
bool isFiftyHz = false;

//Voltage and Current value initalization
float DC_volt_avg = 0;
float DC_amp_avg = 0;
uint16_t ADC_volt;
uint16_t ADC_amp;
float AC_volt = 0;
float AC_amp = 0;
float round_AC_volt;
float round_AC_amp;

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

  adcSensorFifty.setSensitivity(SENSITIVITY);
  adcSensorSixty.setSensitivity(SENSITIVITY);
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
  Wire.write((byte*)&DC_volt_avg, sizeof(DC_volt_avg));
  Wire.write((byte*)&DC_amp_avg, sizeof(DC_amp_avg));
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
  ADC_volt = atinverter.readADC(0x00);
  ADC_amp = atinverter.readADC(0x08);

  if (isFiftyHz) {
    // Get the converted average value of AC voltage and current
    AC_volt = adcSensorFifty.getRmsVoltage(1, ADC_volt);
    AC_amp = 5; // adcSensorFifty.getRmsCurrent(1, ADC_amp);

    // Rounding the values to have 2 decimal places
    round_AC_volt = round(AC_volt*100) / 100;
    round_AC_amp = round(AC_amp*100) / 100;
  }
  else {
    // Get the converted average value of AC voltage and current
    AC_volt = adcSensorSixty.getRmsVoltage(1, ADC_volt);
    AC_amp = 6; //adcSensorSixty.getRmsCurrent(1, ADC_amp);

    // Rounding the values to have 2 decimal places
    round_AC_volt = round(AC_volt*100) / 100;
    round_AC_amp = round(AC_amp*100) / 100;
  }

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
    atinverter.powerCycleGates();
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
  } 
  // 60 Hz Sin Wave Output
  else if (state == 6) {
    if (sixty_once == 0) {
      atinverter.startPWM(false);
      sixty_once = 1;
    }
    fifty_once = 0;
  }
  else if (state == 7) {
    fifty_once = 0;
    sixty_once = 0;
  }
  else {
    Serial.println("Invalid State");
  }

  Serial.print("DC Voltage: "); Serial.println(DC_volt_avg);
  Serial.print("DC Current: "); Serial.println(DC_amp_avg);
  Serial.print("AC Voltage: "); Serial.println(round_AC_volt);
  Serial.print("AC Current: "); Serial.println(round_AC_amp);
  Serial.println();
  atinverter.delay2(10);
}


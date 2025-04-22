/*
  Main.ino - Main code running on Atmega chip
  Created by Zach Kwast, 3/4/25
  Released into the public domain.
*/

#include "Atinverter.h"
#include "Wire.h"

Atinverter atinverter60(60);
Atinverter atinverter50(50);

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
float AC_volt = 0;
float AC_amp = 0;

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
  atinverter60.setUpPinMode();
  atinverter60.initTimer2Delay();

  atinverter60.setSensitivity(SENSITIVITY);
  atinverter50.setSensitivity(SENSITIVITY);
  atinverter60.setUpSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB

  atinverter60.startPWM(false);
  Serial.println("Setup Done");
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
  DC_volt_avg = atinverter60.getAvgDC(true, atinverter60.getVdc());
  DC_amp_avg = atinverter60.getAvgDC(false, atinverter60.getIdc());
  // unsigned long time = millis() - start;
  // Serial.print(time)
  // Serial.println(" ms")


  if (isFiftyHz) {
    // Get the converted average value of AC voltage and current
    AC_volt = atinverter50.getRmsAC(20, true);
    AC_amp = 5; // atinverter50.getRmsCurrent(1, ADC_amp);
    DC_volt_avg = 5;
  }
  if (isFiftyHz == false){
    // Get the converted average value of AC voltage and current
    AC_volt = atinverter60.getRmsAC(20, true);
    AC_amp = 6; //atinverter60.getRmsCurrent(1, ADC_amp);
    DC_volt_avg = 6;
  }

  // Turns gate driver Off
  if (state == 0) {
    digitalWrite(PRORESET_PIN, HIGH);
    digitalWrite(ledPin, HIGH);
    fifty_once = 0;
    sixty_once = 0;
  }
  // Turns gate driver on 
  else if (state == 1) {
    digitalWrite(PRORESET_PIN, LOW);
    digitalWrite(ledPin, LOW);
    fifty_once = 0;
    sixty_once = 0;
  }
  // Cycles proreset which power cycles gate driver
  else if (state == 2) {
    atinverter60.powerCycleGates();
    fifty_once = 0;
    sixty_once = 0;
  }
  // 50 Hz Sin Wave Output
  else if (state == 3) {
    isFiftyHz = true;
    if (fifty_once == 0) {
      atinverter60.startPWM(isFiftyHz);
      fifty_once = 1;
    }
    sixty_once = 0;
  } 
  // 60 Hz Sin Wave Output
  else if (state == 4) {
    isFiftyHz = false;
    if (sixty_once == 0) {
      atinverter60.startPWM(isFiftyHz);
      sixty_once = 1;
    }
    fifty_once = 0;
  }
  else {
    Serial.println("Invalid State");
  }

  Serial.print(F("DC Voltage: ")); Serial.print(DC_volt_avg);
  Serial.print(F(" DC Current: ")); Serial.println(DC_amp_avg);
  Serial.print(F("AC Voltage: ")); Serial.print(AC_volt);
  Serial.print(F(" AC Current: ")); Serial.println(AC_amp);
  Serial.println();
  atinverter60.delay2(1000);
}


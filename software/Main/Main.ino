/*
  Main.ino - Main code running on Atmega chip
  Created by Zach Kwast, 3/4/25
  Released into the public domain.
*/

#include "Atinverter.h"
#include "Wire.h"

// Atinverter initialization 60 and 50 Hz
Atinverter atinverter60(60);
Atinverter atinverter50(50);

// Variable initialization 
const int ledPin = 3;
int state = 0;
int sixty_once = false;
int fifty_once = false;
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
  atinverter60.setUpLEDs();
  atinverter60.initTimer2Delay();

  // Initializes SPI ADC sensitivity and communication (ADC122S021CIMM/NOPB)
  atinverter60.setSensitivity(SENSITIVITY);
  atinverter50.setSensitivity(SENSITIVITY);
  //atinverter60.setUpSPI();

  // Begins PWM at 60 Hz
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

  // Read all DC voltages and currents, averages of 10 samples
  DC_volt_avg = atinverter60.getAvgDC(true, atinverter60.getVdc());
  DC_amp_avg = atinverter60.getAvgDC(false, atinverter60.getIdc());

  if (isFiftyHz) {
    // Get the converted average value of AC voltage and current
    AC_volt = atinverter50.getRmsAC(20, true);
    AC_amp = atinverter50.getRmsAC(20, false);
  }
  if (isFiftyHz == false){
    // Get the converted average value of AC voltage and current
    AC_volt = atinverter60.getRmsAC(20, true);
    AC_amp = atinverter60.getRmsAC(20, false);
  }

  // Turns gate driver Off
  if (state == 0) {
    atinverter60.shutdownGates(MANUAL);
    digitalWrite(ledPin, HIGH);
    fifty_once = false;
    sixty_once = false;
  }
  // Disable PWM
  else if (state == 1) {
    atinverter60.disablePWM();
    fifty_once = false;
    sixty_once = false;
  }
  // Cycles proreset which power cycles gate driver
  else if (state == 2) {
    atinverter60.powerCycleGates();
    fifty_once = false;
    sixty_once = false;
  }
  // 50 Hz Sin Wave Output
  else if (state == 3) {
    isFiftyHz = true;
    if (fifty_once == false) {
      atinverter50.enablePWM();
      atinverter50.startPWM(isFiftyHz);
      atinverter50.turnOnGates();
      digitalWrite(ledPin, LOW);
      fifty_once = true;
    }
    sixty_once = false;
  } 
  // 60 Hz Sin Wave Output
  else if (state == 4) {
    isFiftyHz = false;
    if (sixty_once == false) {
      atinverter60.enablePWM();
      atinverter60.startPWM(isFiftyHz);
      atinverter60.turnOnGates();
      digitalWrite(ledPin, LOW);
      sixty_once = true;
    }
    fifty_once = false;
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


/*
  Main.ino - Main code running on Atmega chip
  Created by Zach Kwast, 3/4/25
  Released into the public domain.
*/

#include "Atinverter.h"
#include "Wire.h"

#define LOOP_RUNS 20

// Atinverter initialization 60 and 50 Hz
Atinverter atinverter60(60);
Atinverter atinverter50(50);

// Variable initialization 
int state = 0;
bool sixty_once = false;
bool fifty_once = false;
bool power_cycle_once = false;
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
  //Serial.begin(9600);

  // Initializes pins
  atinverter60.setUpLEDs();
  atinverter60.initTimer2Delay();

  // Initializes SPI ADC sensitivity and communication (ADC122S021CIMM/NOPB)
  atinverter60.setSensitivity(SENSITIVITY);
  atinverter50.setSensitivity(SENSITIVITY);
  atinverter60.setUpSPI();

  // Begins PWM at 60 Hz
  atinverter60.startPWM(isFiftyHz);
  //Serial.println("Setup Done");
}

// The function that completes when I2C communication 
// data is given to this device
void receiveEvent(int howMany) {
  while(Wire.available()) {
    state = Wire.read();
    //Serial.println(state);
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

  // Read all DC voltages and currents, averages of 10 samples
  DC_volt_avg = atinverter60.getAvgDC(true, atinverter60.getVdc());
  DC_amp_avg = atinverter60.getAvgDC(false, atinverter60.getIdc());

  if (isFiftyHz) {
    // Get the converted average value of AC voltage and current 
    // when the set frquency is 50 Hz
    AC_volt = atinverter50.getRmsAC(LOOP_RUNS, true);
    AC_amp = atinverter50.getRmsAC(LOOP_RUNS, false);
  }
  if (isFiftyHz == false){
    // Get the converted average value of AC voltage and current
    // when the set frquency is 60 Hz
    AC_volt = atinverter60.getRmsAC(LOOP_RUNS, true);
    AC_amp = atinverter60.getRmsAC(LOOP_RUNS, false);
  }

  // Turns gate driver Off
  if (state == 0) {
    atinverter60.shutdownGates(MANUAL);
    atinverter60.set1LED(2, HIGH);
    fifty_once = false;
    sixty_once = false;
    power_cycle_once = false;
  }
  // Disables PWM
  else if (state == 1) {
    atinverter60.disablePWM();
    fifty_once = false;
    sixty_once = false;
    power_cycle_once = false;
  }
  // Cycles proreset which power cycles gate driver
  else if (state == 2) {
    if (power_cycle_once == false) {
      atinverter60.powerCycleGates();
      power_cycle_once = true;
    }
    fifty_once = false;
    sixty_once = false;
  }
  // 50 Hz Sin Wave Output
  else if (state == 3) {
    isFiftyHz = true;
    if (fifty_once == false) {
      atinverter50.enablePWM(); // Enables PWM if it is off
      atinverter50.startPWM(isFiftyHz); // Begins 50 Hz PWM signal
      atinverter50.turnOnGates(); // Confirms gate driver is on
      atinverter60.set1LED(2, LOW); // Sets LED low to match GateShutdown signal
      fifty_once = true;
    }
    sixty_once = false;
    power_cycle_once = false;
  } 
  // 60 Hz Sin Wave Output
  else if (state == 4) {
    isFiftyHz = false;
    if (sixty_once == false) {
      atinverter60.enablePWM(); // Enables PWM if it is off
      atinverter60.startPWM(isFiftyHz); // Begins 50 Hz PWM signal
      atinverter60.turnOnGates(); // Confirms gate driver is on
      atinverter60.set1LED(2, LOW); // Sets LED low to match GateShutdown signal
      sixty_once = true;
    }
    fifty_once = false;
    power_cycle_once = false;
  }
  else {
    //Serial.println("Invalid State");
    fifty_once = false;
    sixty_once = false;
    power_cycle_once = false;
  }

  // Serial.print(F("DC Voltage: ")); Serial.print(DC_volt_avg);
  // Serial.print(F(" DC Current: ")); Serial.println(DC_amp_avg);
  // Serial.print(F("AC Voltage: ")); Serial.print(AC_volt);
  // Serial.print(F(" AC Current: ")); Serial.println(AC_amp);
  // Serial.println();
  //atinverter60.delay2(1000);
}


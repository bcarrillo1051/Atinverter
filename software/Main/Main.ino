/*
  ============================================================================
  File        : Main.ino
  Project     : Atinverter Control Firmware
  Target MCU  : ATmega328P
  Author      : Zach Kwast
  Created     : March 4, 2025
  License     : Public Domain

  Description : 
    Main firmware for the Atinverter system running on the ATmega328P.
    This code handles I2C communication with a master controller (e.g. 
    Raspberry Pi), monitors DC and AC electrical parameters via SPI ADC, 
    and controls the output waveform frequency and inverter states.
    
    Key Features:
      - I2C slave communication (receive & request handlers)
      - AC/DC voltage and current measurement
      - Inverter gate driver and PWM signal control
      - Support for 50Hz and 60Hz output sine waves

  Dependencies:
    - Atinverter.h : Handles inverter control logic and hardware abstraction
    - Wire.h       : I2C library for communication

  ============================================================================
*/

#include "Atinverter.h"
#include "Wire.h"

#define LOOP_RUNS 20

// Atinverter object initialization
Atinverter Atinverter;

// Variable initialization 
int state = 5;
int prev_state;
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
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);

  // Initializes pins
  Atinverter.setUpLEDs();
  Atinverter.initTimer2Delay();

  // Initializes SPI ADC sensitivity and communication (ADC122S021CIMM/NOPB)
  Atinverter.setSensitivity(SENSITIVITY);
  Atinverter.setUpSPI();

}

// The function that completes when I2C communication 
// data is given to this device
void receiveEvent(int howMany) {
  while(Wire.available()) {
    prev_state = state;
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
  if (state == 0) {
    state = prev_state;
  }

  // Read all DC voltages and currents, averages of 10 samples
  DC_volt_avg = Atinverter.getAvgDC(true, Atinverter.getVdc());
  DC_amp_avg = Atinverter.getAvgDC(false, Atinverter.getIdc());

  if (isFiftyHz) {
    // Get the converted average value of AC voltage and current 
    // when the set frquency is 50 Hz
    AC_volt = Atinverter.getRmsAC(LOOP_RUNS, true);
    AC_amp = Atinverter.getRmsAC(LOOP_RUNS, false);
  }
  if (isFiftyHz == false){
    // Get the converted average value of AC voltage and current
    // when the set frquency is 60 Hz
    AC_volt = Atinverter.getRmsAC(LOOP_RUNS, true);
    AC_amp = Atinverter.getRmsAC(LOOP_RUNS, false);
  }

  // Turns gate driver Off
  if (state == 5) {
    Atinverter.shutdownGates(MANUAL);
    Atinverter.set1LED(2, HIGH);
    fifty_once = false;
    sixty_once = false;
    power_cycle_once = false;
  }
  // Disables PWM
  else if (state == 1) {
    Atinverter.disablePWM();
    fifty_once = false;
    sixty_once = false;
    power_cycle_once = false;
  }
  // Cycles proreset which power cycles gate driver
  else if (state == 2) {
    if (power_cycle_once == false) {
      Atinverter.powerCycleGates();
      power_cycle_once = true;
    }
    fifty_once = false;
    sixty_once = false;
  }
  // 50 Hz Sin Wave Output
  else if (state == 3) {
    isFiftyHz = true;
    if (fifty_once == false) {
      Atinverter.enablePWM(); // Enables PWM if it is off
      Atinverter.startPWM(50); // Begins 50 Hz PWM signal
      Atinverter.turnOnGates(); // Confirms gate driver is on
      Atinverter.set1LED(2, LOW); // Sets LED low to match GateShutdown signal
      fifty_once = true;
    }
    sixty_once = false;
    power_cycle_once = false;
  } 
  // 60 Hz Sin Wave Output
  else if (state == 4) {
    isFiftyHz = false;
    if (sixty_once == false) {
      Atinverter.enablePWM(); // Enables PWM if it is off
      Atinverter.startPWM(60); // Begins 60 Hz PWM signal
      Atinverter.turnOnGates(); // Confirms gate driver is on
      Atinverter.set1LED(2, LOW); // Sets LED low to match GateShutdown signal
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
  //Atinverter.delay2(1000);
}


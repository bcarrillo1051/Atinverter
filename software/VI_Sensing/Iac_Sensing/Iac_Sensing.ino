/****************************************************************************** 
 * File:        Iac_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-4-17
 * Description: Senses the AC output current and provides a readout 
 *              on the serial monitor. It is sensed using the ADC122S021.
 ******************************************************************************/
#include "Atinverter.h"

#define LOOP_RUNS 20

Atinverter atinverter; // Atinverter class instance

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));

  atinverter.setSensitivity(SENSITIVITY); // Scale RMS ADC values to real-world signal amplitude
  atinverter.setUpSPI(); // Set up SPI interface and associated pins
  atinverter.startPWM(60); // Set registers and begin PWM generation at 50Hz or 60Hz

  // Timer 2 Setup
  atinverter.initTimer2Delay(); // Set timer 2 registers to proper init values
}

void loop() {
  // Sample AC RMS readings continuously with delay
  float Iac_RMS = atinverter.getRmsAC(false, LOOP_RUNS); // true = Vac, false = Iac, 20 periods
  Serial.print(F("Iac(RMS): ")); Serial.print(Iac_RMS); Serial.println("A");
  atinverter.delay2(2000);
}


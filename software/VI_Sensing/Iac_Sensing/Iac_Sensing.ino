/****************************************************************************** 
 * File:        Iac_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-4-17
 * Description: Senses the AC output current and provides a readout 
 *              on the serial monitor. It is sensed using the ADC122S021.
 ******************************************************************************/
#include "Atinverter.h"

#define LOOP_RUNS 20

// Atinverter class instance
Atinverter atinverter(60);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  atinverter.setSensitivity(SENSITIVITY);

  atinverter.startPWM(false); // 60Hz, true 50Hz
  SPI.begin(); // Configures SCK, CS, and MOSI to outputs
  atinverter.initTimer2Delay(); // Set up Timer2 registers to proper init values
}

void loop() {
    float Iac_RMS = atinverter.getRmsAC(false, LOOP_RUNS); // true = Vac, false = Iac, 20 
    Serial.print(F("Iac(RMS): ")); Serial.print(Iac_RMS); Serial.println("A");
    atinverter.delay2(2000);
}
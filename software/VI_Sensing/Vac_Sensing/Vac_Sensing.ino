/****************************************************************************** 
 * File:        Vac_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-28
 * Description: Senses the AC output voltage and provides a readout 
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
  atinverter.setUpSPI();
  atinverter.startPWM(false); // 60Hz, true 50Hz
  atinverter.initTimer2Delay(); // Set up Timer2 registers to proper init values
}

void loop() {
    float Vac_RMS = atinverter.getRmsAC(true, LOOP_RUNS); // true = Vac, false = Iac, 20 
    Serial.print(F("Vac(RMS): ")); Serial.print(Vac_RMS); Serial.println("V");
    atinverter.delay2(2000);
}
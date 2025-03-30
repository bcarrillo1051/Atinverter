/****************************************************************************** 
 * File:        Vac_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-28
 * Description: Senses the AC output voltage and provides a readout 
 *              on the serial monitor. It is sensed using the ADC122S021.
 ******************************************************************************/
#include "Atinverter.h"
                                    // * 1000 to convert mV/A to V/A 

#include <SPI.h>
// Atinverter class instance
Atinverter atinverter;

void setup() {
  atinverter.setUpSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB
}

void loop() {
  int ADC_val = atinverter.readADC();
  Serial.print(F("ADC Value : ")); Serial.print(ADC_val); // Print ADC from ADC
  delay(1000);
}
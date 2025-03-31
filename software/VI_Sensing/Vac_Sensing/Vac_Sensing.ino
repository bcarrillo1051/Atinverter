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
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));

  atinverter.startPWM(false); // 60Hz
  atinverter.setUpSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB
}

unsigned long previousMillis = 0;
const long interval = 20000; // 5 second interval

void loop() {
unsigned long currentMillis = millis();

  // Check if 5 seconds have passed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read ADC and print
    int ADC_val = atinverter.readADC();
    Serial.print(F("ADC Value : ")); Serial.println(ADC_val);
    Serial.println();
  }
}
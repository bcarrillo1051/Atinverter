/******************************************************************************
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-03-26 
 * Description: Senses the DC input voltage and computes a moving average from
 *              10 samples of the raw DC voltage. Prints both readouts on the 
 *              serial monitor.
 ******************************************************************************/
#include "Atinverter.h"

// Atinverter class instance
Atinverter atinverter;

// Initialize Serial Monitor
void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Voltage Sensing."));
}

// Main loop: print both raw and averaged Vdc values
void loop() {
  float raw_Vdc = atinverter.readVdc();
  Serial.print(F("Raw Vdc : ")); Serial.print(raw_Vdc); Serial.println(F("V")); // Print unaveraged Vdc

  float avg_Vdc = atinverter.readAvgVdc(raw_Vdc);
  Serial.print(F("Avg Vdc : ")); Serial.print(avg_Vdc); Serial.println(F("V")); // Print averaged Vdc

  Serial.println();
  delay(500); // Adjust to change Vdc print/update rate (in milliseconds)
}
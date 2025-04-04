/****************************************************************************** 
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-26
 * Description: Senses the DC input current and computes a moving average from
 *              10 samples of the raw DC current. The input DC current is sensed 
 *              using the TMCS1108A4BQDR hall effect current sensor. The reading
 *              from the sensor is an analog voltage proportional to the current.
 *              Prints both readouts on the serial monitor.            
 ******************************************************************************/

#include "Atinverter.h"

// Atinverter class instance
Atinverter atinverter; // Uses A0 by default for V_DC_PIN

// Initialize Serial Monitor
void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Current Sensing."));
}

// Main loop: print both raw and averaged Idc values
void loop() {
  float raw_Idc = atinverter.readIdc();
  Serial.print(F("Raw Idc : ")); Serial.print(raw_Idc); Serial.println(F("A")); // Print unaveraged Idc 
  
  float avg_Idc = atinverter.readAvgIdc(raw_Idc);
  Serial.print(F("Avg Idc : ")); Serial.print(avg_Idc); Serial.println(F("A")); // Print averaged Idc 
  
  Serial.println();
  delay(500);
}
/****************************************************************************** 
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-26
 * Description: Senses the DC input current and computes a moving average for
 *              a default sample count of 10. The sample count is a macro  
 *              called DEFAULT_MA_SAMPLES that can adjusted in the file 
 *              Atinverter.h. The input DC current is sensed using the
 *              TMCS1108A4BQDR hall effect current sensor. The reading from 
 *              the sensor is an analog voltage proportional to the current.
 *              Prints both readouts on the serial monitor.            
 ******************************************************************************/

#include "Atinverter.h"

Atinverter atinverter; // Atinverter class instance

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Current Sensing."));

  atinverter.startPWM(60); // Set registers and begin PWM generation at 50Hz or 60Hz
  
  atinverter.initTimer2Delay(); // Initialize timer 2 registers
}

// Main loop: read and print both raw and averaged Idc values
void loop() {
  float raw_Idc = atinverter.getIdc();
  Serial.print(F("Raw Idc : ")); Serial.print(raw_Idc); Serial.println(F("A")); // Print unaveraged Idc

  float avg_Idc = atinverter.getAvgDC(false, raw_Idc); // false = Idc, true = Vdc
  Serial.print(F("Avg Idc : ")); Serial.print(avg_Idc); Serial.println(F("A")); // Print averaged Idc
  
  Serial.println();
  atinverter.delay2(500); // Print rate in ms
}
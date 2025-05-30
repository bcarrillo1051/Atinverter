/******************************************************************************
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-03-26 
 * Description: Senses the DC input voltage and computes a moving average for
 *              a default sample count of 10. The sample count is a macro  
 *              called DEFAULT_MA_SAMPLES that can adjusted in the file 
 *              Atinverter.h. Prints both readouts on the serial monitor.               
 ******************************************************************************/
#include "Atinverter.h"

Atinverter atinverter; // Atinverter class instance

// Initialize Serial Monitor and set moving average samples
void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Voltage Sensing."));
  
  atinverter.startPWM(60); // Set up registers and begin PWM generation at 50Hz or 60Hz

  atinverter.initTimer2Delay(); // Initialize timer 2 registers
}

// Main loop: read and print both raw and averaged Vdc values
void loop() {
  float raw_Vdc = atinverter.getVdc();
  Serial.print(F("Raw Vdc : ")); Serial.print(raw_Vdc); Serial.println(F("V")); // Print unaveraged Vdc

  float avg_Vdc = atinverter.getAvgDC(true, raw_Vdc); // true = Vdc, false = Idc
  Serial.print(F("Avg Vdc : ")); Serial.print(avg_Vdc); Serial.println(F("V")); // Print averaged Vdc

  Serial.println();
  atinverter.delay2(500); // Adjust to change Vdc print rate (in milliseconds)
}
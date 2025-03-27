/******************************************************************************
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-03-26 
 * Description: Senses the DC input voltage and provides a readout 
 *              on the serial monitor. The input voltage is sensed using
 *              a 120kΩ / 10kΩ voltage divider (Rvs1 and Rvs2).
 *              If your measured DC value seems off by a few tenths of a volt,
 *              use a multimeter to get the exact resistor values and update 
 *              them in Atinverter.cpp for better accuracy.
 ******************************************************************************/
#include "Atinverter.h"

// Constants for ADC conversion
#define VREF 5.0 // Reference voltage for ADC (in volts)
#define ADC_RESOLUTION_BITS 10
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION_BITS) - 1) // 1023 for 10-bit ADC

// Atinverter class instance
Atinverter atinverter; // Uses A0 by default for V_DC_PIN
                        
// Parameters for moving average
const int num_readings = 10; // Number of samples in moving average
float readings [num_readings]; // Circular buffer for Vdc samples
int read_index = 0; // Current index in the buffer
float total = 0; // Running total of readings

// Initialize Serial Monitor
void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Voltage Sensing."));
}

// Converts raw ADC value to Vdc using voltage divider formula
float read_Vdc() {
  int digital_val = analogRead(atinverter.V_DC_PIN);  // Read raw ADC value
  float Vdc_sense = (VREF * digital_val) / (ADC_MAX_VALUE); // Convert to sensed voltage
  float Vdc = Vdc_sense * (atinverter.Rvs1 + atinverter.Rsv2) / atinverter.Rsv2; // Scale to actual input voltage
  return Vdc;
}

// Returns and prints the instantaneous (unaveraged) Vdc
float raw_Vdc(){
  float Vdc = read_Vdc();
  Serial.print(F("Raw Vdc : ")); Serial.print(Vdc); Serial.println(F("V")); // Print unaveraged Vdc
  return Vdc;
}

// Returns and prints the moving average of Vdc
float average_Vdc() {
  float Vdc = read_Vdc();
  float Vdc_average;

  readings[read_index] = Vdc; // Replace oldest reading with recent reading
  total += readings[read_index]; // Add recent reading to total sum

  read_index++;
  read_index %= num_readings; // Ensure that read_index wraps back to 0 when it reaches num_readings

  Vdc_average = total / num_readings; // Calculate the moving average
  total-= readings[read_index]; // Subtract recent reading from the total sum

  Serial.print(F("Avg Vdc : ")); Serial.print(Vdc_average); Serial.println(F("V")); // Print averaged Vdc  
  return Vdc_average;
}

// Main loop: print both raw and averaged Vdc values
void loop() {
  raw_Vdc();
  average_Vdc();
  Serial.println();
  delay(500); // Adjust to change Vdc print/update rate (in milliseconds)
}
/****************************************************************************** 
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-25
 * Description: Senses the DC input voltage and provides a readout 
 *              on the serial monitor. The input DC voltage is sensed using
 *              a 120kΩ with 10kΩ voltage divider (Rvs1 and Rvs2 respectively).
 *              If you notice the true DC value is off by a couple tenths of a
 *              volt, make sure you measure the exact resistance of Rvs1 and
 *              Rvs2 and adjust in Atinverter.cpp to get more accurate results.
 ******************************************************************************/
#include "Atinverter.h"

// Parameters for ADC
#define VREF 5.0
#define ADC_RESOLUTION_BITS 10
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION_BITS) - 1) // 1024 for ATMEGA328P ADCs
// float VREF = 5.0;
// float ADC_MAX_VALUE = 1023.0;
int digital_val = 0; // store the digital value read

float Vdc_sense; // to store read DC sense value
float Vdc; // store true DC value at the input

// Atinverter class instance
Atinverter atinverter; // preset sense pin is A0
                        
// Parameters for moving average
const int num_readings  = 10; // moving average of 10 samples
float readings [num_readings]; // array to hold Vdc readings
int read_index = 0; // index of array
float total = 0; // sum of Vdc values

void setup() {
  Serial.begin(9600); // setup serial
  Serial.println(F("Initialize Input DC Voltage Sensing."));
}

float raw_Vdc(){
  digital_val = analogRead(atinverter.V_DC_PIN); // read node at voltage divider
  //Serial.print(F("Dig Val : ")); Serial.println(digital_val);
  Vdc_sense = (VREF * digital_val) / (ADC_MAX_VALUE); // Convert to digital Vdc reading to analog
  //Serial.print(F("Vdc Sense : ")); Serial.println(Vdc_sense);
  Vdc = Vdc_sense * (atinverter.Rvs1 + atinverter.Rsv2) / atinverter.Rsv2; // Obtain Vdc input voltage
  Serial.print(F("Raw Vdc : ")); Serial.print(Vdc); Serial.println(F("V")); // To print unaveraged Vdc
  return Vdc;
}

float average_Vdc() {
  float Vdc_average; // to store the average input DC voltage reading 

  digital_val = analogRead(atinverter.V_DC_PIN); // read node at voltage divider
  Vdc_sense = (VREF * digital_val) / (ADC_MAX_VALUE); // Convert to digital Vdc reading to analog
  Vdc = Vdc_sense * (atinverter.Rvs1 + atinverter.Rsv2) / atinverter.Rsv2; // Obtain Vdc input voltage

  readings[read_index] = Vdc; // store the current Vdc reading
  total = total + readings[read_index]; // add Vdc reading to total sum
  read_index++; // increment index to set up for next reading
  if (read_index >= num_readings) { // if the index exceeds the number of readings
    read_index = 0; // wrap around to the beginning (circular buffer behavior)
  }
  Vdc_average = total / num_readings; // calculate the average
  total = total - readings[read_index]; // subtract the last reading

  Serial.print(F("Avg Vdc : ")); Serial.print(Vdc_average); Serial.println(F("V")); // To print unaveraged Vdc  
  return Vdc_average;
}

void loop() {
  raw_Vdc();
  average_Vdc();
  Serial.println();
  delay(500); // Modify this value to customize the Vdc print rate (in ms)
}
/****************************************************************************** 
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-26
 * Description: Senses the DC input current and provides a readout 
 *              on the serial monitor. The input DC voltage is sensed using
 *              the TMCS1108A4BQDR hall effect current sensor. The reading from
 *              sensor is an analog voltage proportional to the current.
 ******************************************************************************/
#include "Atinverter.h"

#define SENSOR_GAIN_MV_PER_A 400.0  // Sensitivity for A4 variant (400 mV per A)

// Parameters for ADC
#define VREF 5.0
#define ADC_RESOLUTION_BITS 10
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION_BITS) - 1) // 1024 for ATMEGA328P ADCs
int digital_val = 0; // store the digital value read

// Atinverter class instance
Atinverter atinverter; // preset sense pin is A0

float raw_Idc(){
  digital_val = analogRead(atinverter.I_DC_PIN); // read I_DC_PIN using built-in ADC
  float v_Idc_sense = (VREF * digital_val) / (ADC_MAX_VALUE); // convert digital Idc reading to analog (Note: voltage reading, not current)
  Serial.print(F("v_Idc_sense : ")); Serial.print(v_Idc_sense); Serial.println(F("V")); // print Vout 

  float Vout_0A = 0.5 * VREF; // zero current output voltage for TMCS1108A4B, see page 3 of datasheet
  Serial.print(F("Vout_0A : ")); Serial.print(Vout_0A); Serial.println(F("V")); // print Vout_0A 
  
  // Using transfer function of the TMCS1108, compute the Idc current, see page 12 of datasheet
  float Idc = (v_Idc_sense - Vout_0A) / SENSOR_GAIN_MV_PER_A * 1000; // * 1000 to convert mV/A to V/A 
  Serial.print(F("Raw Idc : ")); Serial.print(Idc); Serial.println(F("A")); // To print unaveraged Idc 

  // float voltage_diff = v_Idc_sense - offset; // Remove 2.5V offset, See page 29 of datasheet
  // float current = voltage_diff * 1000.0 / SENSOR_GAIN_MV_PER_A // convert voltage difference to current, mV to A
  return Idc;
}

float average_Idc() {
  float Vdc_average; // to store the average input DC voltage reading 

  digital_val = analogRead(atinverter.V_DC_PIN); // read node at voltage divider
  Vdc_sense = (VREF * digital_val) / (ADC_MAX_VALUE); // convert digital Vdc reading to analog
  Vdc = Vdc_sense * (atinverter.Rvs1 + atinverter.Rsv2) / atinverter.Rsv2; // Obtain Vdc input voltage

  readings[read_index] = Vdc; // store the current Vdc reading
  total = total + readings[read_index]; // add Vdc reading to total sum
  read_index++; // increment index to set up for next reading
  if (read_index >= num_readings) { // if the index exceeds the number of readings
    read_index = 0; // wrap around to the beginning (circular buffer behavior)
  }
  Vdc_average = total / num_readings; // calculate the average
  total = total - readings[read_index]; // subtract the last reading

  Serial.print(F("Avg Vdc : ")); Serial.print(Vdc_average); Serial.println(F("V")); // To print averaged Vdc  
  return Vdc_average;
}


void setup() {
  Serial.begin(9600); // Turn on serial monitor, baud rate = 9600s
  Serial.println(F("Initialize Input DC Current Sensing."));
}

void loop() {
  read_Idc();
  Serial.println();
  delay(500);
}
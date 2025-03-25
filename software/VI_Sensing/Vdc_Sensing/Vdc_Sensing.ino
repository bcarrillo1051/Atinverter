/****************************************************************************** 
 * File:        Vdc_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-25
 * Description: Senses the DC input voltage and provides a readout 
 *              on the serial monitor.
 *
 ******************************************************************************/
#include "Atinverter.h"
                       // 120kohm with 10ok ohm voltage divider, sensing DC voltage
Atinverter atinverter; // create an Atinverter object, preset sense pin is A0
                        
// Parameters for moving average
const int numReadings  = 10; // moving average of 10 samples
float readings [numReadings]; // array to hold Vdc readings
int readIndex = 0; // index of array
float total = 0; // sum of Vdc values

// Parameters for ADC
int digital_val = 0; // store the digital value read
float Vref = 5.0; // Arduino is 5V logic
// 1023 

float Vdc_sense; // to store read DC sense value
float Vdc; // store true DC value at the input

void setup() {
  Serial.begin(9600); // setup serial
  Serial.println(F("Initialize Input DC Voltage Sensing."));
}

float smooth() { /* function smooth */
  ////Perform average on sensor readings
  float average; // to store the average voltage reading
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the sensor:
  digital_val = analogRead(atinverter.V_DC_PIN); // read voltage divider between DC input and ATMEGA328P
  Vdc_sense = (Vref *  digital_val) / (1023); // Convert to analog, 1023 is the ADC resolution (2^n - 1)
  Vdc = Vdc_sense * (atinverter.Rvs1 + atinverter.Rsv2) / (float)atinverter.Rsv2; // Obtain true DC input voltage
  readings[readIndex] = Vdc;
  // add value to total:
  total = total + readings[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
  return average;
}

void loop() {
  digital_val = analogRead(atinverter.V_DC_PIN); // read voltage divider between DC input and ATMEGA328P
  Vdc_sense = (Vref *  digital_val) / (1023); // Convert to analog, 1023 is the ADC resolution (2^n - 1)
  Vdc = Vdc_sense * (atinverter.Rvs1 + atinverter.Rsv2) / (float)atinverter.Rsv2; // Obtain true DC input voltage
  Serial.print(F("Vdc     : ")); Serial.println(Vdc);
  Serial.print(F("Vdc avg : ")); Serial.println(smooth());
  delay(200);
}
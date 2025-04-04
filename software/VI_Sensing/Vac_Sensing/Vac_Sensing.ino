/****************************************************************************** 
 * File:        Vac_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-28
 * Description: Senses the AC output voltage and provides a readout 
 *              on the serial monitor. It is sensed using the ADC122S021.
 ******************************************************************************/
#include "Atinverter.h"
#include "SPI.h"
#include "ZMPT101B.h"

#define SENSITIVITY 1.0f

// Atinverter class instance
Atinverter atinverter;

ZMPT101B voltageSensor(60.0);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  voltageSensor.setSensitivity(SENSITIVITY);

  //atinverter.startPWM(false); // 60Hz
  atinverter.setUpSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB
}

void loop() {
    //atinverter.disablePWM();
    //uint16_t ADC_val = atinverter.readADC(0x08);
    uint16_t ADC_val = atinverter.readADC();
    //atinverter.enablePWM();
    Serial.print(F("ADC Value : ")); Serial.println(ADC_val);
    
    // read the voltage and then print via Serial.
    float AC_val = voltageSensor.getRmsVoltage(1, ADC_val);
    Serial.print(F("AC Value : ")); Serial.println(AC_val);Serial.println();
    delay(500);
}
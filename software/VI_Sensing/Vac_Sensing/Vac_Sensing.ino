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

#define SENSITIVITY 37.81f

// Atinverter class instance
Atinverter atinverter;

ZMPT101B voltageSensor(60.0);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  voltageSensor.setSensitivity(SENSITIVITY);

  atinverter.startPWM(false); // 60Hz
  atinverter.setUpSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB
  atinverter.initTimer2Delay();
}

void loop() {
    float Vac_RMS = voltageSensor.getRmsVoltage(20);
    Serial.print(F("Vac(RMS): ")); Serial.print(Vac_RMS); Serial.println("V");
    atinverter.delay2(2000);
}
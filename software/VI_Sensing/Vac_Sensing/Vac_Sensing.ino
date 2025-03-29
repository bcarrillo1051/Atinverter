/****************************************************************************** 
 * File:        Vac_Sensing.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-28
 * Description: Senses the AC input current and provides a readout 
 *              on the serial monitor. The input DC voltage is sensed using
 *              the TMCS1108A4BQDR hall effect current sensor. The reading from
 *              sensor is an analog voltage proportional to the current.
 ******************************************************************************/
#include "Atinverter.h"
                                    // * 1000 to convert mV/A to V/A 

#include <SPI.h>

// Atinverter class instance
Atinverter atinverter;

atinverter.VI_AC_CS_PIN

float read_Vac(){
  
}


void setup() {
  atinverter.setupSPI(); // Configures SPI protocol for ADC122S021CIMM/NOPB
}

void loop() {

}
/****************************************************************************** 
 * File:        60Hz_PWM_Inverter.ino
 * Author:      Bryan Carrillo
 * Date:        10-16-23
 * Description: This program generates a 60Hz sinusoidal waveform using PWM. It
 *              uses an array of discrete sinusoid samples and alternates
 *              the PWM output between digital pins 5 and 6. Timer1 is used to
 *              generate a phase-correct PWM signal with the calculated duty 
 *              cycles based on the sinusoidal array. Timer0 is used to generate
 *              interrupts at a frequency to update the PWM signal, ensuring
 *              a smooth transition through the sinusoidal samples.
 ******************************************************************************/

#include "Atinverter.h"

Atinverter atinverter(60); // Atinverter 60Hz class instance

void setup() {
  atinverter.startPWM(); // Begin PWM generation
}

void loop() {

}
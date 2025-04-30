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
 *
 * Notes:       Two timers are used in this implementation:
 *              - Timer0: Configured in CTC mode to generate periodic interrupts. 
 *                This interrupt controls the timing of when the PWM duty cycle 
 *                is updated, ensuring the correct frequency and smooth sinusoidal 
 *                output.
 *
 *              - Timer1: Configured in phase-correct PWM mode to generate the 
 *                PWM signal that controls the output voltage. It outputs the 
 *                signal alternately on pins 5 and 6, based on the duty cycle 
 *                values from the sinusoidal array.
 * 
 *                For Timer1, the hardware PWM output for Channel A (corresponding 
 *                to OCR1A) is mapped to pin 5 on the Arduino (digital pin 5). 
 *                For PWM output pin 6, the channel B is OCR1B
 *                This mapping is fixed by the MCU's internal architecture.
 ******************************************************************************/

#include "Atinverter.h"

Atinverter atinverter (60); // Atinverter 60Hz class instance

void setup() {
  atinverter.startPWM(false); // false = 60Hz, true = 50Hz
}

void loop() {

}

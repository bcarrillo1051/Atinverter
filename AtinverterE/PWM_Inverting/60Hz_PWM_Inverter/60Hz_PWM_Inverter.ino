/****************************************************************************** 
 * File:        50Hz_PWM_Inverter.c
 * Author:      eprojectszone and Bryan Carrillo
 * Date:        10-16-23
 * Description: This program generates a 60Hz sinusoidal waveform using PWM. It
 *              creates an array of discrete sinusoid samples and alternates
 *              the PWM output between digital pins 9 and 10. Timer1 is used to
 *              generate a phase-correct PWM signal with the calculated duty 
 *              cycles based on the sinusoidal array. Timer0 is used to generate
 *              interrupts at a frequency to update the PWM signal, ensuring
 *              a smooth transition through the sinusoidal samples.
 * 
 * Version:     1.0
 *              Initial version.
 *
 * Notes:       Two timers are used in this implementation:
 *              - Timer1: Configured in phase-correct PWM mode to generate the 
 *                PWM signal that controls the output voltage. It outputs the 
 *                signal alternately on pins 9 and 10, based on the duty cycle 
 *                values from the sinusoidal array.
 * 
 *              - Timer0: Configured in CTC mode to generate periodic interrupts. 
 *                This interrupt controls the timing of when the PWM duty cycle 
 *                is updated, ensuring the correct frequency and smooth sinusoidal 
 *                output.
 * 
 *                For Timer1, the hardware PWM output for Channel A (corresponding 
 *                to OCR1A) is mapped to pin 9 on the Arduino (digital pin 9). 
 *                For PWM output pin 10, the channel B is OCR1B
 *                This mapping is fixed by the MCU's internal architecture.
 ******************************************************************************/

int sin_i = 0; // Index for sinPWM array value
int pwm_i = 0; // Index for PWM value
int OK = 0; // Flag to toggle between pins 5 and 6

int sinPWM[] = {1,3,6,9,12,15,18,21,24,26,29,32,35,38,41,44,47,50,53,56,59,62,65,68,71,73,76,79,82,85,88,91,93,96,
99,102,104,107,110,112,115,118,120,123,126,128,131,133,136,138,141,143,146,148,151,153,155,158,160,
162,165,167,169,171,173,176,178,180,182,184,186,188,190,192,194,196,197,199,201,203,205,206,208,210,
211,213,214,216,217,219,220,222,223,224,226,227,228,229,230,232,233,234,235,236,237,238,239,239,240,
241,242,243,243,244,244,245,246,246,247,247,247,248,248,248,249,249,249,249,249,249,249,249,249,249,
249,249,248,248,248,247,247,247,246,246,245,244,244,243,243,242,241,240,239,239,238,237,236,235,234,233,
232,230,229,228,227,226,224,223,222,220,219,217,216,214,213,211,210,208,206,205,203,201,199,197,196,194,
192,190,188,186,184,182,180,178,176,173,171,169,167,165,162,160,158,155,153,151,148,146,143,141,138,136,
133,131,128,126,123,120,118,115,112,110,107,104,102,99,96,93,91,88,85,82,79,76,73,71,68,65,62,59,56,53,50,
47,44,41,38,35,32,29,26,24,21,18,15,12,9,6,3,1};

void setup() {
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  cli(); // Disable interrupts

  // Set up Timer/Counter0 (8 bit = 256)
  TCCR0A = 0; // Reset control register A
  TCCR0B = 0; // Reset control register B
  TCNT0 = 0; // Reset the counter
  // 0b allow me to write bits in binary
  TCCR0A = 0b10100001; // Phase correct PWM mode
  TCCR0B = 0b00000001; // No prescaler

  // Set up Timer/Counter1 (16 bit = 65,536)
  TCCR1A = 0; // Reset control register A
  TCCR1B = 0; // Reset control register B
  TCNT1 = 0; // Reset the counter
  OCR1A = 509; // Compare match value to generate interrupts at the desired freq
  TCCR1B = 0b00001001; // WGM12 bit is 1 and no prescaler, CTC Mode (Clear timer on compare match)

  TIMSK1 |= (1 << OCIE1A); // Enable compare match interrupts

  sei(); // Enable interrupts
}

// Handles Timer1 compare match interrupts
ISR(TIMER1_COMPA_vect) { // Interrupt when timer 1 match with OCR1A value
  if (sin_i > 260 && OK == 0) { // Final value from vector for pin 6
    sin_i = 0; // Go to first value of vector
    OK = 1; // Enable pin 5
  }
  if (sin_i > 260 && OK == 1) { // final value from vector for pin 5
    sin_i = 0; // Go to first value of vector
    OK = 0; // Enable pin 6
  }
  pwm_i = sinPWM[sin_i]; // pwm_i takes the value from vector corresponding to position sin_i (sin_i is zero indexed)
  sin_i = sin_i + 1; // Go to the next position
  if (OK == 0) {
    OCR0B = 0; // Make pin 5 0
    OCR0A = pwm_i; // Enable pin 6 to corresponding duty cycle
  }
  if (OK == 1) {
    OCR0A = 0; // Make pin 6 0
    OCR0B = pwm_i; // Enable pin 5 to corresponding duty cycle
  }
}

void loop() {

}

/****************************************************************************** 
 * File:        50Hz_PWM_Inverter.c
 * Author:      eprojectszone and Bryan Carrillo
 * Date:        10-16-23
 * Description: This program generates a 50Hz sinusoidal waveform using PWM. It
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
int OK = 0; // Flag to toggle PWM between pins 5 and 6

int sinPWM[] = {1,2,5,7,10,12,15,17,19,22,24,27,30,32,34,37,39,42,
44,47,49,52,54,57,59,61,64,66,69,71,73,76,78,80,83,85,88,90,92,94,97,99,
101,103,106,108,110,113,115,117,119,121,124,126,128,130,132,134,136,138,140,142,144,146,
148,150,152,154,156,158,160,162,164,166,168,169,171,173,175,177,178,180,182,184,185,187,188,190,192,193,
195,196,198,199,201,202,204,205,207,208,209,211,212,213,215,216,217,219,220,221,222,223,224,225,226,227,
228,229,230,231,232,233,234,235,236,237,237,238,239,240,240,241,242,242,243,243,244,244,245,245,246,246,
247,247,247,248,248,248,248,249,249,249,249,249,255,255,255,255,249,249,249,249,249,248,
248,248,248,247,247,247,246,246,245,245,244,244,243,243,242,242,241,240,240,239,238,237,237,236,235,234,
233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,217,216,215,213,212,211,209,208,207,205,204,
202,201,199,198,196,195,193,192,190,188,187,185,184,182,180,178,177,175,173,171,169,168,166,164,162,160,
158,156,154,152,150,148,146,144,142,140,138,136,134,132,130,128,126,124,121,119,117,115,113,110,108,106,
103,101,99,97,94,92,90,88,85,83,80,78,76,73,71,69,66,64,61,59,57,54,52,49,47,44,42,39,37,34,32,30,
27,24,22,19,17,15,12,10,7,5,2,1};

void setup() {
  Serial.begin(9600);

  pinMode(5, OUTPUT); // Set digital pin 5 (physical pin 11) to PWMA output
  pinMode(6, OUTPUT); // Set digital pin 6 (physical pin 12) to PWMB output
  pinMode(8, OUTPUT); // Set digital pin 8 (physical pin 14) to !GateSD output

  digitalWrite(8, HIGH); // Enable gate drivers (!GateSD = 0 and !GateSD = 1 turns off and on respectively)
  
  cli(); // Disable interrupts

  // Set up Timer/Counter1 (16 bit = 65,536)
  TCCR0A = 0; // Reset control register A
  TCCR0B = 0; // Reset control register B
  TCNT0 = 0; // Reset the counter
  TCCR0A = 0b10100001; // Phase correct pwm mode on 8 bits
  TCCR0B = 0b00000001; // No prescaler

  // Set up Timer/Counter0 (8 bit = 256)
  TCCR1A = 0b1000010; // Reset control register A
  TCCR1B = 0; // Reset control register B
  TCNT1 = 0; // Reset the value
  OCR1A = 63; // Compare match value to generate interrupts at the desired freq
  TCCR1B = 0b00001001; // WGM12 bit is 1 and no prescaler, CTC Mode (Clear timer on compare match)
  
  TIMSK1 |=(1 << OCIE1A); // Enable compare match interrupts
  
  sei(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect){ // Interrupt when timer 0 match with OCR0A value
  
  if (sin_i > 313 && OK == 0){ // Final value from vector for pin 9
  sin_i = 0; // Go to first value of vector
  OK = 1; // Enable pin 10
  }
  
  if (sin_i > 313 && OK == 1) { // Final value from vector for pin 10
  sin_i = 0; // Go to first value of vector
  OK = 0; // Enable pin 9
  }
  
  pwm_i = sinPWM[sin_i]; // pwm_i take the value from vector corresponding to position sin_i (sin_i is zero indexed)
  sin_i = sin_i + 1; // Go to the next position
  
  if (OK == 0) {
  OCR0B = 0; // Make pin 10 zero
  OCR0A = pwm_i; // Enable pin 9 to corresponding duty cycle
  }
  
  if (OK == 1) {
  OCR0A = 0; // Make pin 9 zero
  OCR0B = pwm_i; // Enable pin 10 to corresponding duty cycle
  }
  
}

void loop() {

}

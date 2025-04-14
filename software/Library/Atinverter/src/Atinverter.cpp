/*
  AtverterE.cpp - Library for AtinverterE pin and function mapping
  Created by Bryan Carrillo, 10/18/24
  Released into the public domain.
*/

#include "Atinverter.h"

/**
 * @brief Constructor for the AtinverterE class.
 *        Initializes any necessary components or variables.
 */
// Initialize ZMPT101B with provided frequency using member initializer list
Atinverter::Atinverter(float frequency)
{
  period = 1000 / frequency; // 60Hz = 16666 µs (or ~16.7 milliseconds)
}

/**
 * @brief Sets the pin states for the inverter.
 *        This function sets the pin modes for all the necessary GPIO pins.
 */
void Atinverter::setUpPinMode()
{
	// --- LED Pins ---
  pinMode(LED1R_PIN, OUTPUT);
  pinMode(LED1G_PIN, OUTPUT);
  pinMode(LED2R_PIN, OUTPUT);
  pinMode(LED2G_PIN, OUTPUT);

  // --- Internal ADC (ATMEGA328P) Voltage and Current Sensing Pins ---
  pinMode(V_DC_PIN, INPUT);
  pinMode(I_DC_PIN, INPUT);

  // --- PWM Pins ---
  pinMode(PWM_A_PIN, OUTPUT);
  pinMode(PWM_B_PIN, OUTPUT);
  
	// --- I2C Communication Pins

  // --- Overvoltage and Reset Circuitry Reset Pin ---
  pinMode(PRORESET_PIN, OUTPUT);
  digitalWrite(PRORESET_PIN, LOW);

	// --- Gate Driver Pin ---
  pinMode(GATESD_PIN, INPUT);

  pinMode(VI_AC_MISO_PIN, INPUT);
}

/**
 * @brief Controls the state of a specified LED.
 *
 * @param LED   The pin number of the LED to control.
 * @param state The desired state for the LED (HIGH to turn on, LOW to turn off).
 */
void Atinverter::set1LED(int LED, int state)
{
  digitalWrite(LED, state); // Set LED to the specified state
}

/**
 * @brief Sequentially cycles through four LEDs, turning each on and off with a delay.
 * 
 * This function turns on each LED in order with a delay between each,
 * then turns them off in reverse order, also with a delay.
 *
 * @param t_delay Delay time in milliseconds between each LED toggle.
 */
void Atinverter::set2LED(int t_delay) {
  digitalWrite(LED1G_PIN, HIGH); // Turn LED1 green on
  delay(t_delay);
  digitalWrite(LED1R_PIN, HIGH); // Turn LED1 red on
  delay(t_delay);
  digitalWrite(LED2G_PIN, HIGH); // Turn LED2 green on
  delay(t_delay);
  digitalWrite(LED2R_PIN, HIGH); // Turn LED2 red on
  delay(t_delay);

  digitalWrite(LED2R_PIN, LOW); // Turn LED2 red off
  delay(t_delay);
  digitalWrite(LED2G_PIN, LOW); // Turn LED2 green off
  delay(t_delay);
  digitalWrite(LED1R_PIN, LOW); // Turn LED1 red off
  delay(t_delay);
  digitalWrite(LED1G_PIN, LOW); // Turn LED1 green off
  delay(t_delay);
}

/**
 * @brief Converts a raw ADC reading into an instantaneous (non-averaged) DC voltage.
 * 
 * Applies the voltage divider formula to convert the ADC value to the actual
 * DC input voltage based on the known resistor values.
 * 
 * @return Instantaneous measured DC voltage (Vdc).
 */
float Atinverter::readVdc() 
{
  int digital_val = analogRead(V_DC_PIN);  // Read raw ADC value
  float Vdc_sense = (VREF * digital_val) / (ADC_ATMEGA328P_MAX_VALUE); // Convert to sensed voltage
  float Vdc = Vdc_sense * (Rvs1 + Rsv2) / Rsv2; // Scale to actual input voltage
  return Vdc;
}

/**
 * @brief Returns and prints the instantaneous (unaveraged) Idc
 * @return Measured DC current
 */
float Atinverter::readIdc()
{
  int digital_val = analogRead(I_DC_PIN); // Read raw ADC value
  float Vout = (VREF * digital_val) / (ADC_ATMEGA328P_MAX_VALUE); // Convert to analog voltage

  float Vout_0A = 0.5 * VREF; // Compute the zero current output voltage for TMCS1108A4BDR, see page 3 of datasheet
  
  float Idc = (Vout - Vout_0A) / SENSOR_GAIN_MV_PER_A * MV_TO_V; // Use transfer function to compute Idc, see page 12 of datasheet 
  return Idc;
}

/**
 * @brief Computes the moving average of a pre-defined sample count value for a given signal
 * @param signal Unaveraged (instantaneous) measured signal
 * @return Averaged signal
 */
float Atinverter::readAvg(float signal) {

  readings[read_index] = signal; // Replace oldest reading with recent reading
  total += readings[read_index]; // Add recent reading to total sum

  read_index++; // Increment index since we want to continue filling the circular buffer
  if (read_index >= num_readings){ // Ensure that read_index wraps back to 0 when it reaches num_readings
    read_index = 0;
  } 

  float avg_signal = total / num_readings; // Calculate the moving average
  total -= readings[read_index]; // Subtract recent reading from the total sum
  
  return avg_signal;
}

/**
 * @brief Sets up the SPI protocol between the ADC122S021 and the ATMEGA328P 
 */
void Atinverter::setUpSPI() {
  // CS Handling
  pinMode(VI_AC_CS_PIN, OUTPUT); // Set CS to OUTPUT
  digitalWrite(VI_AC_CS_PIN, HIGH); // Ensure comms is not active
  SPI.begin(); // Configures SCK and MOSI to outputs, MISO to input
}

/**
 * @brief Provides the ADC readout from the ADC122S021
 * @param control_byte ADC channel selection, channel 1 = 0x00, channel 2 = 0x08
 * @return int A 12-bit ADC value ranging from 0 to 4095.
 */
int Atinverter::readADC(uint8_t control_byte) {
  
  // Begin SPI communication
  digitalWrite(VI_AC_CS_PIN, LOW); // SPI transfer begins with chip select low
  SPI.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE0)); // Configure and start comms
  
  // First transfer - send control byte and receive 4 MSB bits of data
  uint16_t data = SPI.transfer(control_byte); // Send channel selection bits, also receive 4 MSB bits
  data <<= 8; // Left shift received MSB byte by 8 bits (0000 0110) -> (0000 0110 0000 0000)
  
  // Second transfer - send dummy byte and receive 8 LSB bits of data
  data |= SPI.transfer(0x00); // Transmit dummy byte, only care about receiving remaining LSB byte
  
  // End SPI communication
  digitalWrite(VI_AC_CS_PIN, HIGH); // SPI transfer ends with chip select high 
  SPI.endTransaction();

  data &= 0x0FFF; // Mask to keep only the 12 LSBs (valid ADC data), discard upper 4 bits
  return data;
}

// --- PWM ---
// Define the static variables
bool Atinverter::is50Hz = true;  // Default to 50Hz
int Atinverter::sin_i = 0;
int Atinverter::pwm_i = 0;
int Atinverter::OK = 0;

// Define the sinusoidal 50Hz sample array (312 points)
const int Atinverter::sin50HzPWM[] = {1,2,5,7,10,12,15,17,19,22,24,27,30,32,34,37,39,42,
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


// Define the sinusoidal 60Hz sample array (261 points)
const int Atinverter::sin60HzPWM[] = { 
    1,3,6,9,12,15,18,21,24,26,29,32,35,38,41,44,47,50,53,56,59,62,65,68,71,73,76,79,
    82,85,88,91,93,96,99,102,104,107,110,112,115,118,120,123,126,128,131,133,136,138,
    141,143,146,148,151,153,155,158,160,162,165,167,169,171,173,176,178,180,182,184,
    186,188,190,192,194,196,197,199,201,203,205,206,208,210,211,213,214,216,217,219,
    220,222,223,224,226,227,228,229,230,232,233,234,235,236,237,238,239,239,240,241,
    242,243,243,244,244,245,246,246,247,247,247,248,248,248,249,249,249,249,249,249,
    249,249,249,249,249,249,248,248,248,247,247,247,246,246,245,244,244,243,243,242,
    241,240,239,239,238,237,236,235,234,233,232,230,229,228,227,226,224,223,222,220,
    219,217,216,214,213,211,210,208,206,205,203,201,199,197,196,194,192,190,188,186,
    184,182,180,178,176,173,171,169,167,165,162,160,158,155,153,151,148,146,143,141,
    138,136,133,131,128,126,123,120,118,115,112,110,107,104,102,99,96,93,91,88,85,82,
    79,76,73,71,68,65,62,59,56,53,50,47,44,41,38,35,32,29,26,24,21,18,15,12,9,6,3,1
};

/**
 * @brief Allows for turning on the PWM at run time  
 */
void Atinverter::enablePWM() {
  sei(); // Enable interrupts to resume PWM
}

/**
 * @brief Allows for turning off the PWM at run time  
 */
void Atinverter::disablePWM() {
  cli(); // Disable interrupts to stop PWM
}

/**
 * @brief Initializes PWM and Timers for 50Hz ot 60Hz      
 */
void Atinverter::startPWM(bool is50HzMode) {

  cli(); // Disable interrupts during setup
  is50Hz = is50HzMode; // Set the correct frequency mode

  // Set PWM pins as outputs
  pinMode(PWM_A_PIN, OUTPUT);
  pinMode(PWM_B_PIN, OUTPUT);

  // Set !GATESD pin as OUTPUT and pull high so that gate drivers are enabled
  pinMode(GATESD_PIN, OUTPUT); 
  digitalWrite(GATESD_PIN, HIGH); // (!GateSD = 0 and !GateSD = 1 turns off and on respectively)

  // Set up Timer0 for Phase Correct PWM, (8 bit = 256)
  TCCR0A = 0; // Reset control register A
  TCCR0B = 0; // Reset control register B
  TCNT0 = 0; // Reset the counter
  TCCR0A = 0b10100001; // Phase correct PWM mode
  TCCR0B = 0b00000001; // No prescaler

  // Set up Timer1 for (16 bit = 65,536)// Timer1 for Interrupts
  TCCR1A = is50Hz ? 0b1000010 : 0; // Reset control register A
  TCCR1B = 0; // Reset control register B
  TCNT1 = 0; // Reset the counter
  OCR1A = is50Hz ? 624 : 509; // Compare match value, 50Hz = 63, 60Hz = 509
  TCCR1B = 0b00001001; // CTC Mode, No prescaler (clear timer on compare match)

  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare match interrupts

  sei(); // Enable interrupts
}

/**
 * @brief Interrupt Service Routine (ISR) for Timer1 Compare Match.
 * This ISR is triggered when Timer1's counter (TCNT1) matches the OCR1A value.
 * It is responsible for generating a 60Hz PWM output using precomputed
 * sinusoidal values from the sinPWM array.
 */
ISR(TIMER1_COMPA_vect) {
  Atinverter::pwmISR(); // Call the static ISR method to handle PWM logic
}

/**
 * @brief PWM ISR Logic for Timer1 Compare Match Interrupts.
 * 
 * This method controls the PWM waveform generation using the values
 * stored in the sinPWM50Hz or sinPWM60Hz array. It alternates the output between pins 5 and 6,
 * simulating a sinusoidal waveform using phase-correct PWM. 
 * 
 * - Pin 5 (OCR0B) and Pin 6 (OCR0A) are used as outputs.
 * - `sin_i` tracks the current index in the sinPWM array.
 * - `OK` toggles to alternate between the two output pins (pin 6 on: OK = 0, pin 5 on: OK = 1)
 * - After reaching the end of the array (260 samples), `sin_i` resets to 0.
 */
void Atinverter::pwmISR() {
  const int* sinPWM = is50Hz ? sin50HzPWM : sin60HzPWM; // Use flag to select which samples to use
  int maxIndex = is50Hz ? 313 : 260; // Use flag to select the max index for 50Hz/60Hz mode respectively

  // Check if the end of the array is reached and toggle between pins
  if (sin_i > maxIndex && OK == 0) { // Final value from array for pin 6
    sin_i = 0; // Reset to the beginning of the PWM array
    OK = 1; // Switch to Pin 5 (OCR0B)
  }
  else if (sin_i > maxIndex && OK == 1) {
    sin_i = 0; // Reset to the beginning of the PWM array
    OK = 0; // Switch to Pin 6 (OCR0A)
  }

  // Get the current PWM value from the array
  pwm_i = sinPWM[sin_i];
  sin_i++; // Move to the next sample

  // Apply the PWM value to the corresponding pin while disabling the other
  if (OK == 0) {
    OCR0B = 0; // Ensure Pin 5 is OFF
    OCR0A = pwm_i;  // Set PWM on Pin 6
  }
  else {
    OCR0A = 0; // Ensure Pin 6 is OFF
    OCR0B = pwm_i; // Set PWM on Pin 5
  }
}

// --- Timer2-based Delay Variable ---
// Used to implement a custom 1ms tick counter using Timer2.
// This allows for non-`delay()` based timing separate from the default Arduino millis().
volatile unsigned long Atinverter::timer2Millis = 0;

/**
 * @brief Initializes Timer2 to generate an interrupt every 1ms.
 *        This sets up Timer2 in CTC mode with a 64 prescaler to produce a compare match
 *        every 250 timer ticks (1ms at 16MHz). Each compare match triggers an interrupt
 *        that increments the timer2Millis counter.
 */
void Atinverter::initTimer2Delay() {
  cli(); // Disable interrupts

 // 1. Reset all timer-related registers
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  // Set compare match register for 1ms interval
  // f_cpu = 16 MHz, prescaler = 64 → 16,000,000 / 64 = 250,000 ticks/sec
  // We want 1ms = 1000Hz → 250,000 / 1000 = 250

  // 2. Set compare match value
  OCR2A = 249; // match at 250 ticks

  // 3. Configure mode
  TCCR2A |= (1 << WGM21); // Set CTC mode
  TCCR2B |= (1 << CS22); // Set prescaler = 64

  // 4. Enable interrupt on compare match
  TIMSK2 = (1 << OCIE2A); // enable compare interrupt

  sei(); // Re-enable interrupts
}

/**
 * @brief Blocking delay function based on Timer2's millisecond counter.
 *        Waits until the specified number of milliseconds has passed using the timer2Millis variable.
 *        This function blocks execution and should be used sparingly in time-sensitive applications.
 * @param ms Number of milliseconds to wait
 */
void Atinverter::delay2(unsigned long ms) {
  unsigned long start = timer2Millis;
  while ((timer2Millis - start) < ms) {
    // Busy-wait loop until desired delay is reached
  }
}

/**
 * @brief Returns the current millisecond count from Timer2. Useful for creating
 *        non-blocking delays or timestamps independent of Arduino's millis().
 * @return Current millisecond value from Timer2 counter
 */
unsigned long Atinverter::millis2() {
  return timer2Millis;
}

/**
 * @brief Interrupt Service Routine (ISR) for Timer2 Compare Match A.
 *        This ISR triggers every 1ms and increments the timer2Millis counter.
 *        Acts as the heartbeat for delay2() and millis2() functions.
 */
ISR(TIMER2_COMPA_vect) {
  Atinverter::timer2Millis++;
}



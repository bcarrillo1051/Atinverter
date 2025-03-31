/*
	Atinverter.h = Library for Atinverter pin and functions
	Created by Bryan Carrillo, 4/23/24
	Released into the public domain (Open Source DC to AC Inverter)
*/

#ifndef Atinverter_h
#define Atinverter_h

#include "Arduino.h"
#include <SPI.h>

// --- For Idc_Sensing.ino ---
// * 1000 to convert mV/A to V/A 
#define SENSOR_GAIN_MV_PER_A 400.0  // Sensitivity for A4 variant (400 mV per A)

// --- For Vdc_Sensing.ino ---
// Constants for ADC conversion
#define VREF 5.0 // Reference voltage for ADC (in volts)
#define ADC_RESOLUTION_BITS 10
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION_BITS) - 1) // 1023 for 10-bit ADC

// --- For Vac_Sensing.ino ---
#define ADC_SCALE 4095.0  // 12-bit ADC has a range of 0-4095

/**
 * @class Atinverter
 * @brief This class handles the control of the Atinverter, including pin setup
 *        and LED management.
 */
class Atinverter {

  public:
	Atinverter(); // Constructor

	// --- LED Pins ---
	static const int LED1R_PIN = 2; // Red LED 1 | PD2
	static const int LED1G_PIN = 3; // Green LED 1 | PD3
	static const int LED2R_PIN = 4; // Red LED 2 | PD4
	static const int LED2G_PIN = 7; // Green LED 2 | PD7

	// --- Methods ---
	void setUpPinMode(); // Sets appropriate pinMode() for each const pin
	
	float readVdc();
	float readAvgVdc(float Vdc);
	float readIdc();
	float readAvgIdc(float Idc);

	void setUpSPI(); // Sets up SPI protocol for external ADC
	int readADC();

	void set1LED(int led, int state); // Sets 1 LED to HIGH or LOW
	void set2LED(int t_delay); // Cycles 4 LEDs

	void enablePWM(); // Start PWM generation
	void disablePWM(); // Stop PWM generation
	void startPWM(bool is50HzMode); // Setup PWM and timers and start PWM generation
	static void pwmISR(); // Interrupt Service Routine logic

  private:
	// const = value is read-only and cannot be changed anywhere
	// static = one shared value across all instances of the class

	// --- External ADC (ADC122S021CIMM/NOPB) Voltage and Current Sensing Pins ---
	static const int VI_AC_CS_PIN = 10; // SPI chip select | PB2
	static const int VI_AC_MOSI_PIN = 11; // Send control bits data | PB3
	static const int VI_AC_MISO_PIN = 12; // Receive AC output V/I sense | PB4
	static const int VI_AC_SCLK_PIN = 13; // SPI clock signal | PB5

	// --- Internal ADC (ATMEGA328P) Voltage and Current Sensing Pins ---
	static const int V_DC_PIN = A0; // DC input voltage sense | PC0
	static const int I_DC_PIN = A1; // DC input current sense | PC1

	// --- PWM Pins ---
	const int PWM_A_PIN = 5; // Positive half cycle PWM | PD5
	const int PWM_B_PIN = 6; // Negative half cycle PWM | PD6

	// --- I2C Communication Pins
	const int I2C_SDA_PIN = A4; // Data line between ATMEGA328P and Raspberry Pi | PC4
	const int I2C_SCL_PIN = A5; // Clock line between ATMEGA328P and Raspberry Pi | PC5

	// --- Overvoltage and Reset Circuitry Reset Pin ---
	const int PRORESET_PIN = 9; // Resets the gate drivers, supersedes GATESD pin operation | PB1

	// --- Gate Driver Pin ---
	const int GATESD_PIN = 8; // Gate driver shut down mechanism | PB0

	// --- DC Input Voltage Sampling Resistor Values ---
	static const unsigned long Rvs1 = 120000; // Adjust this if needed as per measurement
	static const unsigned int Rsv2 = 9900; // Adjust this if needed as per measurement

	// --- Parameters for Moving Average ---
	static const int num_readings = 10; // Number of samples in moving average
	float readings[num_readings]; // Circular buffer for Vdc samples
	int read_index = 0; // Current index in the buffer
	float total = 0; // Running total of readings

	// --- Parameters for PWM ---
	static bool is50Hz; // Frequency mode: true for 50Hz, false for 60Hz
	static int sin_i; // Index for sinPWM array value
	static int pwm_i; // Index for PWM value
	static int OK; // Flag to toggle between pins 5 and 6
	static const int sin50HzPWM[312]; // Sinusoidal 50Hz array samples
	static const int sin60HzPWM[261]; // Sinusoidal 60Hz array samples 
};

#endif


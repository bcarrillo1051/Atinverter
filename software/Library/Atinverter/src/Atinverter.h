/*
	Atinverter.h = Library for Atinverter pin and functions
	Created by Bryan Carrillo, 4/23/24
	Released into the public domain (Open Source DC to AC Inverter)
*/

#ifndef ATINVERTER_H
#define ATINVERTER_H

#include "Arduino.h"
#include <SPI.h>

// --- Current Sensor Parameters ---
#define SENSOR_GAIN_MV_PER_A 400.0f // Sensitivity for A4 variant (400 mV per A)
#define MV_TO_V 1000.0f

// --- ADC Parameters ---
#define VREF 5.0f // Both chips powered with 5V 
#define ADC_ATMEGA328P_MAX_VALUE 1023.0f // For ATMEGA328P (internal ADC)
#define ADC_122S021_MAX_VALUE 4095.0f // For ADC122S021 (external SPI ADC)

// --- Moving Average Parameter ---
#define MA_SAMPLES 10 // Default moving average sample count, adjust if needed

// --- AC Voltage Sensing Parameters ---
#define DEFAULT_FREQUENCY 50.0f
#define DEFAULT_SENSITIVITY 500.0f

/**
 * @class Atinverter
 * @brief This class handles the control of the Atinverter, including pin setup
 *        and LED management.
 */
class Atinverter {

  public:
	Atinverter(uint16_t frequency = DEFAULT_FREQUENCY); // Constructor

	// --- LED Pins ---
	static const int LED1R_PIN = 2; // Red LED 1 | PD2
	static const int LED1G_PIN = 3; // Green LED 1 | PD3
	static const int LED2R_PIN = 4; // Red LED 2 | PD4
	static const int LED2G_PIN = 7; // Green LED 2 | PD7

	// --- Methods ---
	void setUpPinMode();

	void set1LED(int led, int state);
	void set2LED(int t_delay);
	
	float getVdc();
	float getIdc();
	float getAvgDC(int signal_type, float signal);

	void setUpSPI(); // Sets up SPI protocol for external ADC
	int getADC(uint8_t control_byte);

	void enablePWM(); // Start PWM generation
	void disablePWM(); // Stop PWM generation
	void startPWM(bool is50HzMode); // Setup PWM and timers and start PWM generation
	static void pwmISR(); // Interrupt Service Routine logic

	void initTimer2Delay();
	void delay2(unsigned long ms);
	unsigned long millis2();

	// --- Timer2 increment ---
	static volatile unsigned long timer2Millis;

	// --- AC Voltage Sensing ---
	void setSensitivity(float value);
	float getRmsVoltage(uint8_t loopCount = 1);

  private:

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

	// --- Parameters for Vdc Moving Average ---
	static const int Vdc_num_readings = MA_SAMPLES; // Vdc samples in moving average
	float Vdc_readings[MA_SAMPLES]; // Circular buffer for signal samples
	int Vdc_read_index = 0; // Current index in the Vdc buffer
	float Vdc_total = 0; // Running total of Vdc readings

	// --- Parameters for Idc Moving Average ---
	static const int Idc_num_readings = MA_SAMPLES; // Samples in moving average
	float Idc_readings[MA_SAMPLES]; // Circular buffer for signal samples
	int Idc_read_index = 0; // Current index in the Idc buffer
	float Idc_total = 0; // Running total of Idc readings

	// --- Parameters for PWM ---
	static bool is50Hz; // Frequency mode: true for 50Hz, false for 60Hz
	static int sin_i; // Index for sinPWM array value
	static int pwm_i; // Index for PWM value
	static int OK; // Flag to toggle between pins 5 and 6
	static const int sin50HzPWM[312]; // Sinusoidal 50Hz array samples
	static const int sin60HzPWM[261]; // Sinusoidal 60Hz array samples 

	uint32_t period;
	float 	 sensitivity = DEFAULT_SENSITIVITY;
	int getZeroPoint();
};

#endif


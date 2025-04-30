/*
	Atinverter.h = Library for Atinverter pin and functions
  	Created by Bryan Carrillo, Zach Kwast, 10/18/24
	Released into the public domain (Open Source DC to AC Inverter)
*/

#ifndef ATINVERTER_H
#define ATINVERTER_H

#include <Arduino.h>
#include <SPI.h>

// --- TMCS1108 Current Sensor Parameters ---
#define SENSOR_GAIN_MV_PER_A 400.0f // Sensitivity for A4 variant (400 mV per A)
#define MV_TO_V 1000.0f // Scaling factor from mV to V
#define VOUT_0A 2.5f // Zero current output voltage

// --- ADC Parameters ---
#define VREF 5.0f // Reference voltage for both ADCs
#define ADC_ATMEGA328P_MAX_VALUE 1023.0f // For ATMEGA328P (internal ADC)
#define ADC_122S021_MAX_VALUE 4095.0f // For ADC122S021 (external SPI ADC)
#define VAC_ADC_CHANNEL 0x00 // ADC122S021 channel 1
#define IAC_ADC_CHANNEL 0x08 // ADC122S021 channel 2
#define CLOCK_FREQUENCY 1000000 // Clock used to drive ADC122S021

// --- AC RMS Parameters ---
#define DEFAULT_FREQUENCY 60.0f // When no frequency is specified for Atinverter instance
#define DEFAULT_SENSITIVITY 500.0f // When no sensitivity is specified for AC RMS calculation
#define SENSITIVITY 37.81f // Tuned value after testing, adjust as needed

// --- Moving Average Parameter ---
#define MA_SAMPLES 10 // Default moving average sample count, adjust as needed

// --- Safety Parameters ---
#define MAX_DC_CURRENT 2.5
#define MAX_AC_CURRENT 2.5

// shutdown error codes for convenience and bookkeeping. user-defined codes start at 4
enum ShutdownCodes
{
  // is not shut down = -1
  HARDWARE = 0,
  MANUAL = 1,
  OVERCURRENT = 2,
  OVERTEMPERATURE = 3,
  NUM_PRESETCODES
};

/**
 * @class Atinverter
 * @brief This class handles the control of the Atinverter, including pin setup
 *        and LED management.
 */
class Atinverter {

  public:
  	// - Constructor -

	Atinverter(uint16_t frequency = DEFAULT_FREQUENCY);

	// - Methods -

	// --- LED Blinking ---
	void setUpLEDs();
	void set1LED(int led, int state);
	void cycleLEDs(int t_delay);
	
	// --- DC Voltage and Current Sensing ---
	float getVdc();
	float getIdc();
	float getAvgDC(bool isVdc, float signalValue);

	// --- AC Voltage and Current Sensing ---
	void setUpSPI();
	void setSensitivity(float value);
	int getADC(uint8_t control_byte);
	float getRmsAC(uint8_t loopCount, bool isVac);

	// --- PWM 50Hz/60Hz Inversion ---
	void startPWM(bool is50HzMode);
	static void pwmISR();
	void enablePWM();
	void disablePWM();

	// --- Timer 2 Delay ---
	void initTimer2Delay();
	void delay2(unsigned long ms);
	unsigned long millis2();

	// --- Gate Shutdown ---
	void shutdownGates(int shutdownCode);
	void turnOnGates();
	void powerCycleGates();

	// --- Overcurrent Protection ---
	void checkOverCurrent(float dcCurrent, float acCurrent);

	// - Member Variables -

	// --- LED Blinking ---
	static const int LED1R_PIN = 2; // Red LED 1 pin | PD2
	static const int LED1G_PIN = 3; // Green LED 1 pin | PD3
	static const int LED2R_PIN = 4; // Red LED 2 pin | PD4
	static const int LED2G_PIN = 7; // Green LED 2 pin | PD7

	// --- Timer 2 Delay ---
	static volatile unsigned long timer2Millis; // Timer 2 Increment

  private:

	// - Methods -

	// --- AC Voltage and Current Sensing ---
	int getZeroPoint(uint8_t control_byte);

	// - Member Variables -

	// --- AC Voltage and Current Sensing ---
	// Pin Definitions
	static const int VI_AC_CS_PIN = 10;   // SPI chip select pin | PB2
	static const int VI_AC_MOSI_PIN = 11; // Send control bits data pin | PB3
	static const int VI_AC_MISO_PIN = 12; // Receive AC output V/I sense pin | PB4
	static const int VI_AC_SCLK_PIN = 13; // SPI clock signal pin | PB5

	// Parameters AC Sensing
	uint32_t period; // Period of PWM type (50Hz or 60Hz)
	float 	 sensitivity = DEFAULT_SENSITIVITY; // Tune this value to achieve proper sensing

	// --- DC Voltage and Current Sensing ---
	// DC Voltage and Current Pin Definitions
	static const int V_DC_PIN = A0;           // DC input voltage sense pin | PC0
	static const int I_DC_PIN = A1;           // DC input current sense pin | PC1
	// DC Voltage Sensing Resistor Values
	static const unsigned long Rvs1 = 120000; // DC voltage sensing resistor 1, adjust as needed
	static const unsigned int Rsv2 = 9900;    // DC voltage sensing resistor 2, adjust as needed
	// Parameters for Vdc Moving Average
	static const int Vdc_num_readings = MA_SAMPLES; // Vdc samples in moving average
	float Vdc_readings[MA_SAMPLES];                 // Circular buffer for Vdc samples
	int Vdc_read_index = 0;                         // Current index in the Vdc buffer
	float Vdc_total = 0;                            // Running total of Vdc readings
	// Parameters for Idc Moving Average
	static const int Idc_num_readings = MA_SAMPLES; // Idc damples in moving average
	float Idc_readings[MA_SAMPLES];                 // Circular buffer for Idc samples
	int Idc_read_index = 0;                         // Current index in the Idc buffer
	float Idc_total = 0;                            // Running total of Idc readings

	// --- PWM 50Hz/60Hz Inversion ---
	// PWM Pin Definitions=
	const int PWM_A_PIN = 5;    // Positive half cycle PWM pin | PD5
	const int PWM_B_PIN = 6;    // Negative half cycle PWM pin | PD6
	const int GATESD_PIN = 8;   // Gate driver shut down mechanism pin | PB0
	const int PRORESET_PIN = 9; // Overvoltage and Reset Circuitry Reset pin | PB1
	
	// Parameters for PWM
	static bool is50Hz;               // Frequency mode: true for 50Hz, false for 60Hz
	static int sin_i;                 // Index for sinPWM array value
	static int pwm_i;                 // Index for PWM value
	static int OK;                    // Flag to toggle between pins 5 and 6
	static const int sin50HzPWM[312]; // Sinusoidal 50Hz array samples
	static const int sin60HzPWM[261]; // Sinusoidal 60Hz array samples

	// --- I2C Communication ---
	const int I2C_SDA_PIN = A4;  // Data line between ATMEGA328P and Raspberry Pi pin | PC4
	const int I2C_SCL_PIN = A5;  // Clock line between ATMEGA328P and Raspberry Pi pin | PC5

	// --- Gate Shutdown ---
	int _shutdownCode = 0;
};

#endif


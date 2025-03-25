/*
	Atinverter.h = Library for Atinverter pin and functions
	Created by Bryan Carrillo, 4/23/24
	Released into the public domain (Open Source DC to AC Inverter)
*/

#ifndef Atinverter_h
#define Atinverter_h

#include "Arduino.h"

/**
 * @class Atinverter
 * @brief This class handles the control of the Atinverter, including pin setup
 *        and LED management.
 */
class Atinverter
{
  public:
    Atinverter(); // constructor

	// static = one shared value across all instances of the class
	// const  = value is read-only and cannot be changed anywhere
	static const int LED1R_PIN = 2;
	static const int LED1G_PIN = 3;
	static const int LED2R_PIN = 4;
	static const int LED2G_PIN = 7;

	// Atinverter Methods (usable functions)
    void setupPinMode(); // sets appropriate pinMode() for each const pin
    void set1LED(int led, int state); // sets 1 LED to HIGH or LOW
	void set2LED(int t_delay); // Cycles 4 LEDs

  private:

	// --- PWM Pins ---
	const int PWM_A_PIN = 5; // positive half cycle PWM
	const int PWM_B_PIN = 6; // negative half cycle PWM

	// --- Voltage and Current Sensing Pins ---
	// Using ATMEGA328P ADCs
	const int V_DC_PIN = 14; // DC input voltage sense
	const int I_DC_PIN = 15; // DC input current sense
	// Using external ADC
	const int VI_AC_CS_PIN = 10; // SPI chip select
	const int VI_AC_MOSI_PIN = 11; // send control bits data
	const int VI_AC_MISO_PIN = 12; // receive AC output V/I sense
	const int VI_AC_SCLK_PIN = 13; // SPI clock signal

	// --- Gate Driver Pin ---
	const int GATESD_PIN = 8; // gate driver shut down mechanism

	// --- Overvoltage and Reset Circuitry Reset Pin ---
	const int PRORESET_PIN = 17; // resets the gate drivers, superceds GATESD pin operation

	// --- I2C Communication Pins
	const int I2C_SDA_PIN = A4; // data line between ATMEGA328P and Raspberry Pi
	const int I2C_SCL_PIN = A5; // clock line between ATMEGA328P and Raspberry Pi
};

#endif


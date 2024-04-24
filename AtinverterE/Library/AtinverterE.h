/*
	Atinverter.h = Library for Atinverter pin and functions
	Created by Bryan Carrillo, 4/23/24
	Released into the public domain (Open Source DC to AC Inverter)
*/

#ifndef Atinverter_h
#define Atinverter_h


// LED pins
const int LED1R_PIN = 2; // Physical pin 4
const int LED2R_PIN = 3; // Physical pin 5
const int LED1G_PIN = 4; // Physical pin 6
const int LED2G_PIN = 7; // Physical pin 13

// PWM pins
const int PWM_A_PIN = 5; // Physical pin 11
const int PWM_B_PIN = 6; // Physical pin 12

// Voltage and current sensing pins
const int V_DC_PIN = 14; // Physical pin 23
const int I_DC_PIN = 15; // Physical pin 24
const int V_AC_PIN = 16; // Physical pin 25, no voltage sampling for this design
const int I_AC_PIN = 17; // Physical pin 26

// Reset Protection Circuit, enables the gate driver
const int PRORESET_PIN = 17; // Physical pin 26


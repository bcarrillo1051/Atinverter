/*
  AtverterE.cpp - Library for AtinverterE pin and function mapping
  Created by Bryan Carrillo, 10/18/24
  Released into the public domain.
*/

#include "AtinverterE.h"

/**
 * @brief Constructor for the AtinverterE class.
 * Initializes any necessary components or variables.
 */
AtinverterE::AtinverterE() { // Define the contructor
}

/**
 * @brief Sets the pin modes for the inverter.
 * This function sets the pin modes for all the necessary GPIO pins.
 */
void AtinverterE::setupPinMode()
{
  pinMode(LED1R_PIN, OUTPUT);
  pinMode(LED1G_PIN, OUTPUT);
  pinMode(LED2R_PIN, OUTPUT);
  pinMode(LED2G_PIN, OUTPUT);
  pinMode(PRORESET_PIN, OUTPUT);
  pinMode(V_DC_PIN, INPUT);
  pinMode(I_DC_PIN, INPUT);
  pinMode(V_AC_PIN, INPUT);
  pinMode(I_AC_PIN, INPUT);
}

/**
 * @brief Sets the output type for LEDs of the atinverter.
 * Sets one of the LEDs to a given state (HIGH or LOW)
 */
void Atinverter::setLED(int led, int state)
{
  digitalWrite(led, state);
}
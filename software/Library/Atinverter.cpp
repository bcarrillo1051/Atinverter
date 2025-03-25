/*
  AtverterE.cpp - Library for AtinverterE pin and function mapping
  Created by Bryan Carrillo, 10/18/24
  Released into the public domain.
*/

#include "Atinverter.h"

/**
 * @brief Constructor for the AtinverterE class.
 * Initializes any necessary components or variables.
 */
Atinverter::Atinverter() { // Define the contructor
}

/**
 * @brief Sets the pin modes for the inverter.
 * This function sets the pin modes for all the necessary GPIO pins.
 */
void Atinverter::setupPinMode()
{
  pinMode(Atinverter::LED1R_PIN, OUTPUT);
  pinMode(Atinverter::LED1G_PIN, OUTPUT);
  pinMode(Atinverter::LED2R_PIN, OUTPUT);
  pinMode(Atinverter::LED2G_PIN, OUTPUT);
  pinMode(PRORESET_PIN, OUTPUT);
  pinMode(V_DC_PIN, INPUT);
  pinMode(I_DC_PIN, INPUT);

  // DEFINE RESET OF PINS
}

/**
 * @brief Takes 1 of 4 available LEDs and the state to be applied
 *        Sets the LED to the provided state
 */
void Atinverter::set1LED(int LED, int state)
{
  digitalWrite(LED, state); // set LED to state (HIGH or LOW)
}

/**
 * @brief Takes a delay time to cycle
 *        4 LEDs on and off
 */
void Atinverter::set2LED(int t_delay) {
  digitalWrite(Atinverter::LED1G_PIN, HIGH);   // turn LED1 green on
  delay(t_delay);                              // wait delay time
  digitalWrite(Atinverter::LED1R_PIN, HIGH);   // turn LED1 red on
  delay(t_delay);                              // wait delay time
  digitalWrite(Atinverter::LED2G_PIN, HIGH);   // turn LED2 green on
  delay(t_delay);                              // wait delay time
  digitalWrite(Atinverter::LED2R_PIN, HIGH);   // turn LED2 red on
  delay(t_delay);                              // wait delay time

  digitalWrite(Atinverter::LED2R_PIN, LOW);   // turn LED2 red off
  delay(t_delay);                              // wait delay time
  digitalWrite(Atinverter::LED2G_PIN, LOW);   // turn LED2 green off
  delay(t_delay);                              // wait delay time
  digitalWrite(Atinverter::LED1R_PIN, LOW);   // turn LED1 red off
  delay(t_delay);                              // wait delay time
  digitalWrite(Atinverter::LED1G_PIN, LOW);   // turn LED1 green off
  delay(t_delay);                              // wait delay time
}


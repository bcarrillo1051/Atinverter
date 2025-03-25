/****************************************************************************** 
 * File:        Blink.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-25
 * Description: Allows for blinking either a red or green LED
 *              Serves the purpose of testing MCU to verify control of GPIOs
 *
 ******************************************************************************/
#include "Atinverter.h"

Atinverter atinverter; // Create an Atinverter object
int PERIOD = 1000; // Set up duration of LED on or off (ms)

void setup() {
  atinverter.setupPinMode(); // Sets pins to inputs or outputs
}

void loop() {

  // --- Atinverter.set1LED ---
  // Turning on
  atinverter.set1LED(atinverter.LED1R_PIN, HIGH); // Test 1st Red LED ON
  atinverter.set1LED(atinverter.LED2G_PIN, HIGH); // Test 2nd Green LED ON
  delay(PERIOD);
  // Turning off
  atinverter.set1LED(atinverter.LED1R_PIN, LOW); // Test 1st Red LED ON
  atinverter.set1LED(atinverter.LED2G_PIN, LOW); // Test 2nd Green LED ON
  delay(PERIOD);
  atinverter.set2LED(PERIOD);
}
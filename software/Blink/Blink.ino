/****************************************************************************** 
 * File:        Blink.ino
 * Author:      Bryan Carrillo
 * Date:        2025-3-25
 * Description: Allows for blinking either a red or green LED
 *              Serves the purpose of testing MCU to verify control of GPIOs
 ******************************************************************************/
#include "Atinverter.h"

Atinverter atinverter; // Create an Atinverter instance 
int PERIOD = 1000; // Delay time in milliseconds

void setup() {
  atinverter.setUpLEDs(); // Set 4 LED GPIOs as OUTPUTs
}

void loop() {
  // --- Atinverter.set1LED ---
  // Turning on
  atinverter.set1LED(atinverter.LED1R_PIN, HIGH); // Test 1st Red LED ON
  atinverter.set1LED(atinverter.LED2G_PIN, HIGH); // Test 2nd Green LED ON
  delay(PERIOD); // Wait PERIOD
  // Turning off
  atinverter.set1LED(atinverter.LED1R_PIN, LOW); // Test 1st Red LED ON
  atinverter.set1LED(atinverter.LED2G_PIN, LOW); // Test 2nd Green LED ON
  delay(PERIOD); // Wait PERIOD

  // --- Atinverter.cycleLEDs ---
  atinverter.cycleLEDs(PERIOD); // Cycle LEDs every PERIOD
}
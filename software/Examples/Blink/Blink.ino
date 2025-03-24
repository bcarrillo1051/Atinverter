/****************************************************************************** 
 * File:        Blink.ino
 * Author:      Bryan Carrillo
 * Date:        2024-10-18
 * Description: Allows for blinking either a red or green LED
 *              Serves the purpose of testing MCU to verify control of GPIOs
 * Version:     1.0
 *              Initial version.
 *
 ******************************************************************************/

#include "AtinverterE.h"

AtinverterE atinverterE; // Make an AtinverterE objec
int PERIOD = 2000; // Set up duration of LED on or off

void setup() {
  atinverterE.setupPinMode();
}

/**
 * @brief Takes one LED, either red or green, turns them on for some time
 *        then turns them off for some time
 */
void ledTest(int led) {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(PERIOD);             // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(PERIOD);             // wait for a second
}

/**
 * @brief Takes two LEDs, one red and one green and turns them on together
 *        waits some time then turns them off together
 */
void ledTest2(int ledr, int ledg) {
  digitalWrite(ledr, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(ledg, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(PERIOD);              // wait for a second
  digitalWrite(ledr, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(ledg, LOW);    // turn the LED off by making the voltage LOW
  delay(PERIOD);              // wait for a second
}


void loop() {
  ledTest(LED1R_PIN); // Test 1st Red LED
  ledTest(LED1G_PIN); // Test 1st Green LED
  ledTest(LED2R_PIN); // Test 2nd Red LED
  ledTest(LED2G_PIN); // Test 2nd Green LED
  ledTest2(LED1R_PIN, LED1G_PIN); // Test 1st Red and Green LED together
  ledTest2(LED2R_PIN, LED2G_PIN); // Test 2nd Red and Green LED together
}
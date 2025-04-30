---
title: Blink
layout: default
parent: Modules
nav_order: 1
---

# **Blink Module Program**
---

**Purpose:** This program demonstrates how to utilize the Atinverter library's LED control features in a standalone application. This module is useful for confirming the operational state of the microcontroller, hardware connections, and library integration.

**Pseudocode:**
1. Create a new Atinverter instance
2. Set up the period of time for delaying LED operations
3. Set the GPIO pins for LEDs as outputs
4. Turn on LED 1
5. Turn on LED 2
6. Wait for the defined period
7. Turn off LED 1
8. Turn off LED 2
9. Wait for the defined period
10. Cycle through LEDs with delay

**Implementation of `Blink.ino`:**
```cpp
#include "Atinverter.h"

Atinverter atinverter;
int PERIOD = 1000;

void setup() {
  atinverter.setUpLEDs();
}

void loop() {
  atinverter.set1LED(atinverter.LED1R_PIN, HIGH);
  atinverter.set1LED(atinverter.LED2G_PIN, HIGH);
  delay(PERIOD);

  atinverter.set1LED(atinverter.LED1R_PIN, LOW);
  atinverter.set1LED(atinverter.LED2G_PIN, LOW);
  delay(PERIOD);

  atinverter.cycleLEDs(PERIOD);
}
```
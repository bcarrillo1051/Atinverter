---
title: Blink
layout: default
parent: Modules
nav_order: 1
---

# **Blink Module Program**
---

**Purpose:** This program demonstrates how to utilize the Atinverter library's LED control features in a standalone application. This module is useful for confirming the operational state of the microcontroller, hardware connections, and library integration.

## `Blink.ino`

**Pseudocode:**
1. Include the Atinverter library
2. Create a new Atinverter instance
3. Set up the period of time for delaying LED operations
4. Set the GPIO pins for LEDs as outputs
5. Turn on LED 1
6. Turn on LED 2
7. Wait for the defined period
8. Turn off LED 1
9. Turn off LED 2
10. Wait for the defined period
11. Cycle through LEDs with delay

**Implementation:**
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
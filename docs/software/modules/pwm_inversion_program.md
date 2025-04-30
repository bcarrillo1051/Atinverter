---
title: PWM Inversion
layout: default
parent: Modules
nav_order: 3
mathjax: true
---

# **50Hz/60Hz PWM Inversion Module Programs**
---

**Purpose:** The 50Hz PWM Inversion feature demonstrates how to generate low-level pin operations for LED control functionality within the Atinverter library. It includes methods for setting up GPIO pins as outputs, controlling individual LEDs, and cycling through multiple LEDs in a predefined sequence.

## `50Hz_PWM_Inverter.ino`

**Pseudocode:**

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter(50); // Atinverter 50Hz class instance

void setup(){
  atinverter.startPWM(true); // true = 50Hz, false = 60Hz
}

void loop(){

}
```
<br>

---

# AC Current Sensing Module Program

## `60Hz_PWM_Inverter.ino`

**Purpose:**

**Psuedocode:**

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter (60); // Atinverter 60Hz class instance

void setup() {
  atinverter.startPWM(false); // false = 60Hz, true = 50Hz
}

void loop() {

}
```


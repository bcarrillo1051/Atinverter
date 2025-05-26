---
title: PWM Inversion
layout: default
parent: Modules
nav_order: 3
mathjax: true
---

# **50/60Hz PWM Inversion Program Modules**
---

**Purpose:** These two standalone programs (`50Hz_PWM_Inverter.ino` and `60Hz_PWM_Inverter.ino`) exhibits how to utilize the Atinverter library’s PWM generation functionality for the PWM inverter. Each file initializes the PWM for at a switching frequency of 31,372kHz and fundamental frequency of 50Hz or 60Hz, which are required for AC waveform generation.

## `50Hz_PWM_Inverter.ino`

**Pseudocode:**
1. Include the Atinverter library
2. Create an Atinverter instance
3. Begin PWM operation at the specified frequency of 50Hz

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter;

void setup(){
  atinverter.startPWM(50);
}

void loop(){

}
```

---

## `60Hz_PWM_Inverter.ino`

**Psuedocode:**
1. Include the Atinverter library
2. Create an Atinverter instance
3. Begin PWM operation at the specified frequency of 60Hz

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter;

void setup() {
  atinverter.startPWM(60);
}

void loop() {

}
```


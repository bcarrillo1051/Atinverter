---
title: PWM Inversion
layout: default
parent: Modules
nav_order: 3
mathjax: true
---

# **50Hz/60Hz PWM Inversion Module Programs**
---

**Purpose:** These two standalone programs (`50Hz_PWM_Inverter.ino` and `60Hz_PWM_Inverter.ino`) exhibits how to utilize the Atinverter library’s PWM generation functionality for the PWM inverter. Each file initializes the PWM for at a switching frequency of 31,372kHz and fundamental frequency of 50Hz or 60Hz, which are required for AC waveform generation.

## `50Hz_PWM_Inverter.ino`

**Pseudocode:**
1. Include the Atinverter library
2. Create an Atinverter instance set to 50Hz mode
3. Begin PWM generation

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter(50);

void setup(){
  atinverter.startPWM();
}

void loop(){

}
```

---

## `60Hz_PWM_Inverter.ino`

**Psuedocode:**
1. Include the Atinverter library
2. Create an Atinverter instance set to 60Hz mode
3. Begin PWM generation

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter(60);

void setup() {
  atinverter.startPWM();
}

void loop() {

}
```


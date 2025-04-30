---
title: AC V/I Sensing
layout: default
parent: Modules
nav_order: 5
mathjax: true
---

# **AC V/I Sensing Module Program**
---

**Purpose:**
These two standalone programs (`Vac_Sensing.ino` and `Iac_Sensing.ino`) demonstrate how to use the Atinverter library’s AC sensing features to measure either the RMS voltage or RMS current output of the PWM inverter. Both modules validate correct SPI communication with the ADC122S021, PWM generation for inverter operation, signal processing using the root mean square (RMS) method, and employ the Timer 2 based delaying feature.

## `Vac_Sensing.ino`

**Pseudocode:**

**Implementation:**
```cpp
#include "Atinverter.h"
#define LOOP_RUNS 20

// Atinverter class instance
Atinverter atinverter(60);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  atinverter.setSensitivity(SENSITIVITY);
  atinverter.setUpSPI();
  atinverter.startPWM(false);
  atinverter.initTimer2Delay();
}

void loop() {
    float Vac_RMS = atinverter.getRmsAC(true, LOOP_RUNS);
    Serial.print(F("Vac(RMS): ")); Serial.print(Vac_RMS); Serial.println("V");
    atinverter.delay2(2000);
}
```

---

## `Iac_Sensing.ino`

**Psuedocode:**

**Implementation:**
```cpp
#include "Atinverter.h"

#define LOOP_RUNS 20

// Atinverter class instance
Atinverter atinverter(60);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  atinverter.setSensitivity(SENSITIVITY);

  atinverter.startPWM(false); // 60Hz, true 50Hz
  atinverter.setUpSPI(); // Configures SCK, CS, and MOSI to outputs
  atinverter.initTimer2Delay(); // Set up Timer2 registers to proper init values
}

void loop() {
    float Iac_RMS = atinverter.getRmsAC(false, LOOP_RUNS); // true = Vac, false = Iac, 20 
    Serial.print(F("Iac(RMS): ")); Serial.print(Iac_RMS); Serial.println("A");
    atinverter.delay2(2000);
}
```


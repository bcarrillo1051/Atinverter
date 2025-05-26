---
title: DC V/I Sensing
layout: default
parent: Modules
nav_order: 4
mathjax: true
---

# **DC V/I Sensing Program Modules**
---

**Purpose:** These two standalone programs (`Vdc_Sensing.ino` and `Idc_Sensing.ino`) showcase how to use the Atinverter library’s DC sensing features to monitor the input DC voltage and current of the PWM inverter. Both modules verify hardware connections, retrieve both raw and averaged sensor values, and employ the Timer 2 based delaying feature.

## `Vdc_Sensing.ino`

**Pseudocode:**
1. Include the Atinverter library
2. Create a new Atinverter instance
3. Initialize the serial monitor for displaying DC voltage readings
4. Begin PWM operation at a specified frequency of 50Hz or 60Hz
5. Initialize Timer 2 for time-based delay functionality
6. Read the raw input DC voltage
7. Print raw input DC voltage to the serial monitor
8. Pass the most recent reading to the `getAvgDC` method to calculate the running average
9. Print averaged input DC voltage to the serial monitor
10. Wait a designated delay before printing the next reading

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Voltage Sensing."));
  atinverter.startPWM(60);
  atinverter.initTimer2Delay();
}

void loop() {
  float raw_Vdc = atinverter.getVdc();
  Serial.print(F("Raw Vdc : ")); Serial.print(raw_Vdc); Serial.println(F("V"));

  float avg_Vdc = atinverter.getAvgDC(true, raw_Vdc);
  Serial.print(F("Avg Vdc : ")); Serial.print(avg_Vdc); Serial.println(F("V"));

  Serial.println();
  atinverter.delay2(500);
}
```

---

## `Idc_Sensing.ino`

**Pseudocode:**
1. Include the Atinverter library
2. Create a new Atinverter instance set to 50Hz or 60Hz mode
3. Initialize the serial monitor for displaying DC current readings
4. Begin PWM operation at a specified frequency of 50Hz or 60Hz
5. Initialize Timer 2 for time-based delay functionality
6. Read the raw input DC current
7. Print raw input DC current to the serial monitor
8. Pass the most recent reading to the `getAvgDC` method to calculate the running average
9. Print averaged input DC current to the serial monitor
10. Wait a designated delay before printing the next reading

**Implementation:**
```cpp
#include "Atinverter.h"

Atinverter atinverter;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Current Sensing."));
  atinverter.startPWM(60);
  atinverter.initTimer2Delay();
}

void loop() {
  float raw_Idc = atinverter.getIdc();
  Serial.print(F("Raw Idc : ")); Serial.print(raw_Idc); Serial.println(F("A"));

  float avg_Idc = atinverter.getAvgDC(false, raw_Idc);
  Serial.print(F("Avg Idc : ")); Serial.print(avg_Idc); Serial.println(F("A"));
  
  Serial.println();
  atinverter.delay2(500);
}
```
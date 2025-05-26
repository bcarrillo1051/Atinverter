---
title: AC V/I Sensing
layout: default
parent: Modules
nav_order: 5
mathjax: true
---

# **AC V/I Sensing Program Modules**
---

**Purpose:**
These two standalone programs (`Vac_Sensing.ino` and `Iac_Sensing.ino`) demonstrate how to use the Atinverter library’s AC sensing features to measure either the RMS voltage or RMS current output of the PWM inverter. Both modules validate correct SPI communication with the ADC122S021, PWM generation for inverter operation, signal processing using the root mean square (RMS) method, and employ the Timer 2 based delaying feature.

## `Vac_Sensing.ino`

**Pseudocode:**
1. Include the Atinverter library
2. Create a new Atinverter instance
3. Initialize the serial monitor for displaying AC voltage readings
4. Set sensitivity factor that scales the AC RMS readings
5. Set up SPI interface and associated pins
6. Begin PWM operation at a specified frequency of 50Hz or 60Hz
7. Initialize Timer 2 for time-based delay functionality
8. Continuously read the averaged output AC voltage
9. Wait a designated delay before printing the next reading

**Implementation:**
```cpp
#include "Atinverter.h"
#define LOOP_RUNS 20

Atinverter atinverter;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  atinverter.setSensitivity(SENSITIVITY);
  atinverter.setUpSPI();
  atinverter.startPWM(60);
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

**Pseudocode:**
1. Include the Atinverter library
2. Create a new Atinverter instance
3. Initialize the serial monitor for displaying AC current readings
4. Set sensitivity factor that scales the AC RMS readings
5. Set up SPI interface and associated pins
6. Begin PWM operation at a specified frequency of 50Hz or 60Hz
7. Configure Timer 2 registers to enable delaying using `delay2` method
8. Continuously read the averaged output AC current
9. Wait a designated delay before printing the next reading

**Implementation:**
```cpp
#include "Atinverter.h"

#define LOOP_RUNS 20

Atinverter atinverter;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Output AC Voltage Sensing."));
  atinverter.setSensitivity(SENSITIVITY);

  atinverter.startPWM(60);
  atinverter.setUpSPI();
  atinverter.initTimer2Delay();
}

void loop() {
    float Iac_RMS = atinverter.getRmsAC(false, LOOP_RUNS);
    Serial.print(F("Iac(RMS): ")); Serial.print(Iac_RMS); Serial.println("A");
    atinverter.delay2(2000);
}
```
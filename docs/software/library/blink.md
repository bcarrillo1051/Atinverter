---
title: Blink
layout: default
parent: Library
nav_order: 4
---

# **Blink Software**
---

# Overview

This feature of the library is primarily useful for testing the bootloaded state of the ATMEGA328P or for basic LED blinking operations.
<br>
<br>

---

# Pin Assignments

This table shows the mapping of the LEDs and their corresponding ATMEGA328P pin assignments:

| Pin Description | Pin Name (Code) | Pin Number | Pin State |
|:---------------:|:---------------:|:----------:|:---------:|
| Red LED 1       | LED1R_PIN       | 2          | OUTPUT    |
| Green LED 1     | LED1G_PIN       | 3          | OUTPUT    |
| Red LED 2       | LED2R_PIN       | 4          | OUTPUT    |
| Green LED 2     | LED2G_PIN       | 7          | OUTPUT    |

<br>

---

# Library Structure

**Implementation in `Atinverter.h`:**
```cpp
// LED Pin Definitions
static const int LED1R_PIN = 2;
static const int LED1G_PIN = 3;
static const int LED2R_PIN = 4;
static const int LED2G_PIN = 7;

// Methods
void setUpLEDs();
void set1LED(int LED, int state);
void cycleLEDs(int t_delay);
```
<br>

---

# Method Descriptions

## `void setUpLEDs()`

**Purpose:** Configures all LED pins as outputs.

**Psuedocode:**
1. Configure the state of all 4 LED pins as outputs for driving.

**Implementation in `Atinverter.cpp`:**
```cpp
void Atinverter::setUpLEDs() {
  pinMode(LED1R_PIN, OUTPUT);
  pinMode(LED1G_PIN, OUTPUT);
  pinMode(LED2R_PIN, OUTPUT);
  pinMode(LED2G_PIN, OUTPUT);
}
```
<br>

## `void set1LED(int LED, int state)`

**Purpose:** Sets a user-selected LED pin as on or off.

**Pseudocode:**
1. Sets a user-selected LED pin as on (HIGH) or off (LOW).

**Implementation in `Atinverter.cpp`:**
```cpp
void Atinverter::set1LED(int LED, int state) {
  digitalWrite(LED, state);
}
```
<br>

## `void cycleLEDs(int t_delay)`

**Purpose:** Sequentially cycles through four LEDs, turning each on and off with a delay.

**Pseudocode:**
1. Turns on each LED in order with a delay in between in milliseconds
2. Turns off each LED in reverse order with a delay in between in milliseconds

**Implementation in `Atinverter.cpp`:**
```cpp
void Atinverter::cycleLEDs(int t_delay) {
  digitalWrite(LED1G_PIN, HIGH);
  delay(t_delay);
  digitalWrite(LED1R_PIN, HIGH);
  delay(t_delay);
  digitalWrite(LED2G_PIN, HIGH);
  delay(t_delay);
  digitalWrite(LED2R_PIN, HIGH);
  delay(t_delay);

  digitalWrite(LED2R_PIN, LOW);
  delay(t_delay);
  digitalWrite(LED2G_PIN, LOW);
  delay(t_delay);
  digitalWrite(LED1R_PIN, LOW);
  delay(t_delay);
  digitalWrite(LED1G_PIN, LOW);
  delay(t_delay);
}
```
<br>

---

# Blink Module Program

**Purpose:** Demonstrates the use of the Blink methods in an Arduino sketch.

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
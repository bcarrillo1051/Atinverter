---
title: Blink
layout: default
parent: Features
nav_order: 1
---

# **Blink Library Feature**
---

## 📋 Overview

The Blink feature abstracts low-level pin operations for LED control functionality within the Atinverter library. It includes methods for setting up GPIO pins as outputs, controlling individual LEDs, and cycling through multiple LEDs in a predefined sequence.

---

## 📌 Pin Assignments

<br>

<div style="text-align: left;">
    <h7><b>Table X.</b> ATMEGA328P Pin Configuration for LED Control </h7>
</div>

| Pin Description | Pin Name (Code) | Pin Number | Pin State |
|:---------------:|:---------------:|:----------:|:---------:|
| Red LED 1       | LED1R_PIN       | 2          | OUTPUT    |
| Green LED 1     | LED1G_PIN       | 3          | OUTPUT    |
| Red LED 2       | LED2R_PIN       | 4          | OUTPUT    |
| Green LED 2     | LED2G_PIN       | 7          | OUTPUT    |

---

## 📂 Header File Definitions

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

## 📝 Method Descriptions

## `void setUpLEDs()`

**Purpose:** Configures all LED pins as outputs.

**Pseudocode:**
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

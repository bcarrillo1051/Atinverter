---
title: DC V/I Sensing
layout: default
parent: Library
nav_order: 5
mathjax: true
---

<!-- To enable math equation formatting -->
{% if page.mathjax %}
  <!-- Polyfill for older browsers (optional) -->
  <script type="text/javascript" async 
    src="https://polyfill.io/v3/polyfill.min.js?features=es6">
  </script>

  <!-- MathJax v3 for LaTeX rendering -->
  <script type="text/javascript" async 
    id="MathJax-script" 
    src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js">
  </script>

  <!-- Custom MathJax Configuration -->
  <script type="text/javascript">
    MathJax = {
      tex: {
        inlineMath: [['$', '$'], ['\\(', '\\)']],
        displayMath: [['$$', '$$'], ['\\[', '\\]']],
      }
    };
  </script>
{% endif %}

# **DC Voltage and Current Sensing Software**
---

# Overview

The input DC voltage and current sensing is achieved seamlessly through onboard ADC pins of the ATMEGA328P, along with TMCS1108A4BQDR current sensor. These values can be obtained through a computation using raw readouts of the ADC or for improved accuracy, through a running average sampling method.
<br>
<br>

---

# Pin Assignments

This table expresses the dedicated analog pins used for the design and their respective state.

| Pin Description    | Pin Name (Code) | Pin Number | Pin State |
|:------------------:|:---------------:|:----------:|:---------:|
| DC Voltage Sensing | V_DC_PIN        | A0         | INPUT     |
| DC Current Sensing | I_DC_PIN        | A1         | INPUT     |

<br>

---

# Library Structure

**Implementation in `Atinverter.h`:**
```cpp
// TMCS1108 Current Sensor Parameters
#define SENSOR_GAIN_MV_PER_A 400.0f // Sensitivity for A4 variant (400 mV per A)
#define MV_TO_V 1000.0f // Scaling factor from mV to V
#define VOUT_0A 2.5f // Zero current output voltage

// ADC Parameters
#define VREF 5.0f // Reference voltage for both ADCs
#define ADC_ATMEGA328P_MAX_VALUE 1023.0f // For ATMEGA328P (internal ADC)

// Moving Average Samples
#define MA_SAMPLES 10

// DC Voltage and Current Pin Definitions
static const int V_DC_PIN = A0;
static const int I_DC_PIN = A1;

// DC Voltage Sensing Resistor Values
static const unsigned long Rvs1 = 120000;
static const unsigned int Rsv2 = 9900;

// Parameters for Vdc Moving Average
static const int Vdc_num_readings = MA_SAMPLES;
float Vdc_readings[MA_SAMPLES];
int Vdc_read_index = 0;
float Vdc_total = 0;

// Parameters for Idc Moving Average
static const int Idc_num_readings = MA_SAMPLES;
float Idc_readings[MA_SAMPLES];
int Idc_read_index = 0;
float Idc_total = 0;

// Method Declarations
float getVdc();
float getIdc();
float getAvgDC(bool isVdc, float signalValue);
```
<br>

---

# Method Descriptions

## `getVdc()`

**Purpose:** Senses and returns the input DC voltage.

**Psuedocode:**
1. Read raw ADC value from ATMEGA328P analog pin connected to sensing voltage
2. Translate the digital ADC reading into its analog voltage equivalent
- $$Sensed\ Voltage = \left( \frac{Reference\ Voltage \times ADC\ Reading}{2^{(ADC\ Bits)} - 1} \right)$$
3. Convert the sensed voltage to the DC input voltage using rearranged voltage divider
- $$Input\ DC\ Voltage = Sensed\ Voltage \times \frac{(Top\ Resistor + Bottom\ Resistor)}{Top\ Resistor}$$
4. Return the calculated DC input voltage

**Implementation in `Atinverter.cpp`:**
```cpp
float Atinverter::getVdc() {
  int digital_val = analogRead(V_DC_PIN);
  float Vdc_sense = (VREF * digital_val) / (ADC_ATMEGA328P_MAX_VALUE);
  float Vdc = Vdc_sense * (Rvs1 + Rsv2) / Rsv2;
  return Vdc;
}
```
<br>

## `getIdc()`

**Purpose:** Senses and returns the input DC current.

**Psuedocode:**
1. Read raw ADC value from ATMEGA328P analog pin connected to TMCS1108 analog output
2. Translate the digital ADC reading into its analog voltage equivalent
- $$Output\ Voltage = \left( \frac{Reference\ Voltage \times ADC\ Reading}{2^{(ADC\ Bits)} - 1} \right)$$
3. Compute the input current using the transfer function from the TMCS1108 datasheet\
(equation 1 on page 12)
- $$Input\ DC\ Current = \left( \frac{Output\ Voltage - Zero\ Current\ Output\ Voltage}{Sensitivity} \right)$$
4. Return the calculated DC input current

**Implementation in `Atinverter.cpp`:**
```cpp
float Atinverter::getIdc() {
  int digital_val = analogRead(I_DC_PIN);
  float Vout = (VREF * digital_val) / (ADC_ATMEGA328P_MAX_VALUE);
  float Idc = (Vout - VOUT_0A) / SENSOR_GAIN_MV_PER_A * MV_TO_V;
  return Idc;
}
```
<br>

## getAvgDC(bool isVdc, float signalValue)

**Purpose:** Computes and returns the moving average for the input DC voltage or current based on a pre-defined sample count.

**Psuedocode:**
1. Decide which sampling array will be utilized based on user input
2. Store the most recent value received in the buffer
3. Add the readings to the running total
4. Increment the sampling array index
5. Check if read index is above the buffer size
  - If yes, reset buffer index
  - If no, continue
6. Compute the average signal using total over the number of readings
7. Substract the oldest reading to prepare for new readng
8. Return the averaged signal

**Implementation in `Atinverter.cpp`:**
```cpp
float Atinverter::getAvgDC(bool isVdc, float signalValue) {
  const int num_readings = isVdc ? Vdc_num_readings : Idc_num_readings;
  float* readings = isVdc ? Vdc_readings : Idc_readings;
  int& read_index = isVdc ? Vdc_read_index : Idc_read_index;
  float& total = isVdc ? Vdc_total : Idc_total;

  readings[read_index] = signalValue;
  total += readings[read_index];

  read_index++;
  if (read_index >= num_readings){
    read_index = 0;
  } 

  float avg_signal = total / num_readings;
  total -= readings[read_index];

  return avg_signal;
}
```
<br>

---

# DC Voltage Sensing Module Program

**Purpose:** Demonstrates the use of the DC Voltage Sensing methods in an Arduino sketch.

**Psuedocode:**
1. Create a new Atinverter instance with desired frequency (50Hz or 60Hz)
2. Initialize the serial monitor for displaying DC voltage readings
3. Begin PWM operation at desired frequency
4. Configure Timer 2 registers to enable delaying using `delay2` method
5. Read the raw input DC voltage
6. Print raw input DC voltage to the serial monitor
7. Pass the most recent reading to the `getAvgDC` method to calculate the running average
8. Print averaged input DC voltage to the serial monitor
9. Wait a designated delay before printing the next reading

**Implementation of `Vdc_Sensing.ino`:**
```cpp
#include "Atinverter.h"

Atinverter atinverter (60);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Voltage Sensing."));
  atinverter.startPWM(false);
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
<br>

---

# DC Current Sensing Module Program

**Purpose:** Demonstrates the use of the DC Current Sensing methods in an Arduino sketch.

**Psuedocode:**
1. Create a new Atinverter instance with desired frequency (50Hz or 60Hz)
2. Initialize the serial monitor for displaying DC current readings
3. Begin PWM operation at desired frequency
4. Configure Timer 2 registers to enable delaying using `delay2` method
5. Read the raw input DC current
6. Print raw input DC current to the serial monitor
7. Pass the most recent reading to the `getAvgDC` method to calculate the running average
8. Print averaged input DC current to the serial monitor
9. Wait a designated delay before printing the next reading

**Implementation of `Idc_Sensing.ino`:**
```cpp
#include "Atinverter.h"

Atinverter atinverter (60);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize Input DC Current Sensing."));
  atinverter.startPWM(false);
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
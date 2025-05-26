---
title: DC V/I Sensing
layout: default
parent: Features
nav_order: 2
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

# **DC V/I Sensing Library Feature**
---

## 📋 Overview

The DC Voltage and Current Sensing feature provides an interface for monitoring the input voltage and current of the PWM inverter. It utilizes the onboard ADC pins of the ATMEGA328P, along with TMCS1108A4BQDR Hall-effect current sensor. Voltage is measured through a resistive voltage divider, while current is measured using the analog output of the current sensor. These signals can be obtained through methods which use raw readouts of the ADC or for improved accuracy, through a running average sampling method.
<br>
<br>

---

## 📌 Pin Assignments

<br>

<div style="text-align: left;">
    <h7><b>Table X.</b> ATMEGA328P Pin Configuration for DC Sensing </h7>
</div>

| Pin Description    | Pin Name (Code) | Pin Number | Pin State |
|:------------------:|:---------------:|:----------:|:---------:|
| DC Voltage Sensing | V_DC_PIN        | A0         | INPUT     |
| DC Current Sensing | I_DC_PIN        | A1         | INPUT     |

<br>

---

## 📂 Header File Definitions

**Implementation in `Atinverter.h`:**
```cpp
// TMCS1108 Current Sensor Parameters
#define SENSOR_GAIN_MV_PER_A 400.0f
#define MV_TO_V 0.001f
#define VOUT_0A 2.5f

// ADC Parameters
#define VREF 5.0f
#define ADC_ATMEGA328P_MAX_VALUE 1023.0f

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

## 📝 Method Descriptions

## `getVdc()`

**Purpose:** Senses and returns the input DC voltage.

**Pseudocode:**
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

**Pseudocode:**
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

## `getAvgDC(bool isVdc, float signalValue)`

**Purpose:** Computes and returns the moving average for the input DC voltage or current based on a pre-defined sample count.

**Pseudocode:**
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

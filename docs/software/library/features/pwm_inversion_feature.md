---
title: PWM Inversion
layout: default
parent: Features
nav_order: 4
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

# **50Hz/60Hz PWM Inversion Library Feature**
***

## 📋 Overview


---

## 📌 Pin Assignments

This table demonstrates PWM pins and their corresponding ATMEGA328P pin states:

| Pin Description | Pin Name (Code) | Pin Number | Pin State |
|:---------------:|:---------------:|:----------:|:---------:|
| PWM A Side      | PWM_A_PIN       | 5          | OUTPUT    |
| PWM B Side      | PWM_B_PIN       | 6          | OUTPUT    |
| Red LED 2       | GATESD_PIN      | 8          | INPUT     |
| Green LED 2     | PRORESET_PIN    | 9          | OUTPUT    |

<br>

---

## 📂 Library Structure

**Implementation in `Atinverter.h`:**
```cpp
// PWM Pin Definitions=
const int PWM_A_PIN = 5;
const int PWM_B_PIN = 6;
const int GATESD_PIN = 8;
const int PRORESET_PIN = 9;

// Parameters for PWM
static bool is50Hz;
static int sin_i;
static int pwm_i;
static int OK;
static const int sin50HzPWM[312];
static const int sin60HzPWM[261];

// Methods
void startPWM(bool is50HzMode);
static void pwmISR();
void enablePWM();
void disablePWM();
```
<br>

---

## 📝 Method Descriptions

## ``

**Purpose:**

**Pseudocode:**
1.

**Implementation in `Atinverter.cpp`:**
```cpp

```
<br>

## ``

**Purpose:** Sets a user-selected LED pin as on or off.

**Pseudocode:**
1.

**Implementation in `Atinverter.cpp`:**
```cpp
```
<br>

## ``

**Purpose:** 

**Pseudocode:**

**Implementation in `Atinverter.cpp`:**
```cpp

}
```


## PWM 50Hz
$$f_{sw} = 31,372Hz$$
$$f_{sw} = \frac{1}{f_{sw}} = 31.875\mu s$$ 





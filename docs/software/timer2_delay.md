---
title: Timer 2 Delay
layout: default
parent: Software
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


# **⏱️ Timer 2 Delay**
***

An interface is required to handle **program delays** for time-sensitive operations such as **AC voltage sensing** and **serial communication**, among other tasks.

{: .highlight-yellow }
> 💭 **"Why not simply use the Arduino core functions `delay()`, `millis()`, or `micros()` to accomplish this?"**


Although that might seem like the most straightforward approach, the **ATMEGA328P Timer0** — used by these core functions — is also responsible for **PWM generation** in our hardware, as detailed in the [PWM Control](PWM_control) section.

If we attempt to use the core delay functions (which reconfigure Timer0 registers), while simultaneously using Timer0 to handle PWM interrupts, we risk introducing **conflicting register states**, leading to erratic behavior or malfunctioning PWM output — not catastrophic, but definitely not ideal 😅.

{: .highlight-yellow }
> 💭 **"Why don't you create a software-based delay program with an incrementer?"**
```cpp
unsigned long counter = 0;
while (counter < SOME_VALUE) {
  counter++;
}
```


To implement `delay2()` and `millis2()`, it’s not practical to rely on a simple software incrementer (e.g., counter++) inside a loop to create timing. This approach would require trial-and-error tuning of the maximum value to match a desired delay, which is inefficient, non-deterministic, and highly sensitive to compiler optimizations or clock speed changes.

---

## 💡 The Workaround

To resolve this, we developed custom timing functions — `delay2()` and `millis2()` — based on **Timer2** from the ATMEGA328P. These serve as effective substitutes for the Arduino core library timing functions, providing a **millisecond-based timing mechanism** that operates **independently of Timer0**.

---

## 🧠 Understanding the Timers on the ATMEGA328P

The ATMEGA328P includes **three timers**, each with different bit widths:

1. **Timer0**: 8-bit  
2. **Timer1**: 16-bit  
3. **Timer2**: 8-bit

{: .highlight-yellow }
> ❓ **"What do the 'bits' actually mean?"**

The number of **bits** determines how high a timer can count before it overflows and restarts.

Think of it like a stopwatch:
- An **8-bit timer** can count from `0` to `255` (that’s $2^8 - 1$)
- A **16-bit timer** can count all the way from `0` to `65,535` ($2^{16} - 1$)

A higher bit-width means longer timing intervals or higher resolution is possible — but for a simple 1ms tick, even an 8-bit timer like Timer2 is more than sufficient.

---

## ⚙️ Timer2 Delay Initialization

We need to use the handy timer2 first configure the timer 2 such that its interrupts will fire every 1ms. 

{: .highlight-yellow }
> ❓ **"Why trigger the interrupts every 1ms? Can't you use other "**
This is necessary because this will provide a 1:1 mapping of the between the timer counts and the effective millisecond time frame that we wish to delay by.


```cpp
// Reset Timer2 control registers
TCCR2A = 0;
TCCR2B = 0;
TCNT2  = 0;
```



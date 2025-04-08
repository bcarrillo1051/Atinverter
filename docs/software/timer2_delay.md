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

Although that might seem like the most straightforward approach, the **ATMEGA328P Timer0** — used by these core functions — is also responsible for **PWM generation** in our hardware. This is described in detail in the [PWM Control](PWM_control) section.

If we attempt to use the core delay functions (which reconfigure Timer0 registers), while simultaneously using Timer0 to handle PWM interrupts, we risk introducing **conflicting register states**. This also translates to erratic behavior in our PWM output which although may not be catastrophic, it is certainly not desired 😅.

{: .highlight-yellow }
> 💭 **"Why don't you create a software-based delay program with an incrementer?"**

```cpp
unsigned long counter = 0;
while (counter < SOME_VALUE) {
  counter++;
}
```

To implement `delay2()` and `millis2()`, it’s not practical to only rely on a simple software incrementer (e.g., `counter++`) inside a loop to create timing. This would be an approach that requires trial-and-error tuning of a maximum value to achieve a desired delay. In addition, this is inefficient, non-deterministic, and highly sensitive to compiler optimizations or clock speed changes.

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
> 💭 **"What do the 'bits' actually mean?"**

The number of **bits** determines how high a timer can count before it overflows and restarts.

Think of it like a clock ⏱️:
- An **8-bit timer** can count from `0` to `255` ($2^8 - 1$)
- A **16-bit timer** can count from `0` to `65,535` ($2^{16} - 1$)

A higher bit-width means higher resolution is possible — but for a simple 1ms tick, an 8-bit timer like Timer2 is more than sufficient.

---

## ⚙️ Timer2 Delay Initialization

We need to use the handy timer2 to configure the timer 2 such that its interrupts will fire every 1ms. 

{: .highlight-yellow }
> 💭 **"Why trigger the interrupts every 1ms? Can't you use another time frame?"**

Yes, but the better question is why would you reasonably do that? 
1. ☑️ Arduino's built-in `millis()` and `delay()` functions are based on 1ms resolution.
2. 🔄 Simpler math and high enough resolution (1:1 mapping)
    - 500ms -> count to 500 ticks where each counter tick is 1ms instead of 500ms -> count to 2000 ticks where each tick is 0.25ms
3. ⚙️ Low CPU Overhead
    - Operating the CPU at higher frequency can overwhelm the CPU and be more power intensive

Okay now that I hopefully convinced you of why we need 1ms interrupts, here is how we do it:
As per the registers described in pages 127-134 of the [ATMEGA328P datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf), these are what we will need to configure. There are 9 registers in total, but we do not need to configure of all of them. 

<br>
The registers of interest and what is needed to do is as follows:

{: .highlight-green }
🔧 TCCR2A – Timer/Counter Control Register A

**Reset TCCR2A**: 

Avoids unexpected behavior from previously set states and allows for a known clean slate.
```cpp
  TCCR2A = 0; // Reset Timer2 TCCR2A control register
```

**What It Controls**:
- **Compare Output Modes `COM2A[1:0], COM2B[1:0]`** — what happens to OC2A/OC2B (digital 11/3) pins on compare match with TCNT2
- **Waveform Generation Mode `WGM2[1:0]`** — selects the timer's counting behavior (Normal, PWM/Phase Correct, CTC, or Fast PWM)


**What To Set**:
1. Clear Timer on Compare Match (CTC) mode using WGM21 bit

```cpp
  TCCR2A |= (1 << WGM21); // Set CTC mode
```
<br>

{: .highlight-green }
🔧 TCCR2B – Timer/Counter Control Register B

**Reset TCCR2B**: 

Avoids unexpected behavior from previously set states and allows for a known clean slate.
```cpp
  TCCR2B = 0; // Reset Timer2 TCCR2B control register
```

**What It Controls**:
- **Force Output Compare `FOC2B, FOC2A`** — forcing a compare match on OC2B/OC2A (digital 3/11) pins
- **Waveform Generation Mode `WGM22`** — Used with TCCR2A for CTC or fast PWM
- **Clock Select `CS2[2:0]`** — Sets the pre-scaler for the timer clock source 

**What To Set**:
1. Prescaler of 64 using CS22 bit

```cpp
  TCCR2B |= (1 << CS22); // Set prescaler = 64
```
<br>

{: .highlight-green }
🔧 TCNT2 – Timer/Counter Register

**Reset TCNT2**: 

Starts counter at 0, why would you start anywhere else? 😅
```cpp
TCNT2  = 0; // Reset Timer2 TCNT2 control register
```

**What It Controls**:
- **Timer Counter `TCNT2[7:0]`** — The current value of Timer2’s counter (0-255)

**What To Set**:
1. Nothing, the timer runs autonomously

<br>

{: .highlight-green }
🔧 TIMSK2 – Timer/Counter2 Interrupt Mask Register

**Reset TIMSK2**: 

Avoids unexpected behavior from previously set states and allows for a known clean slate.
```cpp
TIMSK2 = 0; // Reset Timer2 TIMSK2 control register
```

**What It Controls**:
- **Output Compare Match Interrupt Enables `OCIE2B, OCIE2A`** — Enables an interrupt when the timer value `TCNT2` matches the value in `OCR2B` or `OCR2A`.
- **Waveform Generation Mode `TOIE2`** — Enables an interrupt when timer overflows

**What To Set**:
1. Enable interrupt on Compare Match A using the OCIE2A bit.

```cpp
TIMSK2 = (1 << OCIE2A); // enable compare interrupt
```
<br>

{: .highlight-green }
🔧 OCR2A – Output Compare Register A

**What it controls**:
1. Contains value that is continuously compared to counter value TCNT2

**Why set it to 249**:
1. We actually need to compute this value based on the equation provided in page 121 of the [ATMEGA328P datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf?).

(1) Given equation: $$f_{OCnx} = \frac{f_{clk_{I/O}}}{2 \times N \times (1 + OCRnx)}$$
where:
- $OCRnx$ is the output compare register value 
- $f_{OCnx}$ is the frequency of the timer/counter
- $f_{clk_{I/O}}$ is the frequency of the clock source
- N is the prescale factor (1, 8, 32, 64, 128, 256, or 1024) 

(2) Rearranged equation to solve for OCR2A value: $$OCRnx = \frac{f_{clk_{I/O}}}{2 \times N \times f_{OCnx}} - 1$$
$$OCRnx = \frac{16*10^6}{2 \times 64 \times 1000} - 1 = 249$$





```cpp
TCNT2  = 0; // Reset Timer2 TCNT2 control register
```


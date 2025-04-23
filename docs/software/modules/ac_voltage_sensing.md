---
title: AC Voltage Sensing
layout: default
parent: Modules
nav_order: 7
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

# **AC Voltage Sensing Software Overview**

As mentioned in the [AC Voltage Sensing](../../hardware/a_v2_hw) hardware section, to achieve AC RMS voltage measurement across the load of the power inverter ($V_{\mathrm{AC_load}}$), the signal is:
1. Transformed from a 12V-48$V_{pk}$ AC signal to a composite signal containing both DC and AC components.
2. Scaled down to a 2.5V DC, 0-1V AC peak signal (effectively ranges from 1.5V to 3.5V), to maintain compatibilibity with the ADC122S021 input channels

Now that this signal is conditioned appropriately for sampling, the next step is properly configure the SPI bus on the ATMEGA328P to allow communication between the ADC122S021.

## **Interfacing ADC122S021 with ATMEGA328P Using SPI**

### Outline of SPI Communication

The ATMEGA328P operates as the master/controller and the ADC122S021 as the slave/peripheral. As it conventional in SPI communication, the ATMEGA328P initiates communication by setting **CS** low. The communication is conducted in 16 clock pulses of the **SCLK** frequency, corresponding to two byte data frames. 

Since SPI inherently is **full-duplex**, these two data frames are used for:
1. Sending control data through MOSI (**DIN of ADC122S021**) to specify the channel to read from 
2. Receiving conversion data through MISO (**DOUT of ADC122S021**)

It should be highlighted that the only useful data for **DIN** and **DOUT** are the first 8 most signicant bits (MSB) and the last 12 least significant bits (LSB). 

Given that ADC122S021 utilizes a successive approximation register (SARs) topology, it employs the use of a track and hold mechanism. The voltage samples are tracked for the first 4 clock cycles and held for data transmission during the subsequent 12 clock cycles.

Once the 12-bit conversion data is received by the ATMEGA328P, the CS line is enabled high to terminate communication with the ADC122S021.

The timing diagram of the ADC122S021 provides an insightful visual representation of the SPI communication and reflects the information covered.

![ADC122S021 Operational Timing Diagram](../../images/ADC122S021_timing_diagram.png)

### Configuring SPI Protocol

This section describes SPI pins and clock frequency configuration to ensure successful communication.

The following table delineates the ATMEGA328P pin type, number, and state to be set:

| Pin Type | Digital Pin Number    | State   |
|:--------:|:---------------------:|:-------:|
| CS       | 10                    | OUTPUT  |
| MOSI     | 11                    | OUTPUT  | 
| MISO     | 12                    | INPUT   | 
| SCLK     | 13                    | OUTPUT  |

Configuring all SPI pins on the ATMEGA328P can be achieved by including the Arduino built-in library `SPI.h` in a program. In this case, the library is included in our user-defined library `Atinverter.h`.

```cpp
#include <SPI.h>
```

M

CS, MOSI, SCLK

```cpp
SPI.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE0)); // Configure and start comms
```

### ⌚ Choosing the Appropriate SPI Clock Source

We have to ensure that our SPI clock rate is suitable for the communication with the ADC. This means that the clock source **cannot exceed** the maximum sample rate (throughput) of the device. The table shown below provides the ranges of allow clock frequencies and how that maps to a given sample rate:

| Parameter    | Max Clock Freq    | Range            |
|:------------:|:-----------------:|:----------------:|
| fsclk        | Clock Frequency   | 0.8MHz - 3.2MHz  |
| fs           | Sample Rate       | 50ksps - 200ksps |  

To better understand the relationship between the sample rate and the clock frequency, we can use the following equation:

$$f_{s} = \frac{f_{SCLK}}{16}$$

In our case, the available frequency from the ATMEGA328P is 16MHz, therefore to obtain the maximum allowable sample rate, we need a pre-scaler of 5 to obtain a $f_{SCLK} = 3.2MHz$, but unfortunately the microcontroller only has the following pre-scalers:

| Prescaler     | Arduino Command      | Resulting Frequency  |
|:-------------:|:--------------------:|:--------------------:|
| 2             | SPI_CLOCK_DIV2       | 8 MHz                |
| 4             | SPI_CLOCK_DIV4       | 4 MHz                |
| 8             | SPI_CLOCK_DIV8       | 2 MHz                |
| 16            | SPI_CLOCK_DIV16      | 1 MHz                |
| 32            | SPI_CLOCK_DIV32      | 500 kHz              |
| 64            | SPI_CLOCK_DIV64      | 250 kHz              |
| 128           | SPI_CLOCK_DIV128     | 125 kHz              |

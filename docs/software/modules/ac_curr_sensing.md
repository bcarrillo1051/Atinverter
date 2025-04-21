---
title: AC Current Sensing
layout: default
parent: Modules
nav_order: 3
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


# **AC Output V/I Sensing Considerations**
### Understanding how to interface with the ADC122S021CIMM/NOPB using the SPI protocol and performing voltage and current sensing of the AC output.
***

## ⌚ Choosing the Appropriate SPI Clock Source

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

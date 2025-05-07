---
title: Simulation
layout: default
parent: Atinverter V2 Hardware
nav_order: 4
has_toc: false
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

# **Hardware Simulation**
***

From the various hardware features that were implemented in the Atinverter V2, three of circuit networks were simulated to verify their proper operation:
- AC Voltage Sensing
- Boost Converter
- Overvoltage and Reset Circuitry

## AC Voltage Sensing

The AC voltage sensing topology was simulated using LTSpice, a free SPICE-based software with schematic capture and waveform viewer capabilities. Component types and values were employed to most closely resemble the true circuit. Two AC sources $V_{ac_in+}$ and $V_{ac_in-}$ were implemented to represent the positive and negative phase of the AC signal from across the load.

It should be noted that the waveform magnitude appeared halved the true value (e.g. 24V instead of 48V across $V_{ac_in+}$ and $V_{ac_in-}$) due to the absence of switching-based modulation in the simulation. In the actual circuit, one complimentary leg would be deactived during each half cycle. As a result, the differential input at the AC voltage sensing circuit is double the intended value. To intepret the simulation data correctly, the differntial input voltage needs to be halved to represent the actual signal amplitude correctly.

Two minor modeling adjustments were required to align the simulation with the physical circuit:
- Op-Amp Subtitution: The **OP07** was used in place of the **LM358** to reflect amplification behavior with similar electrical characteristics.
- Potentiometer Emulation: The adjustable gain stage was modeled using two adkustable resistors, $R_{tune1}$ and $R_{tune2}$, to mimic the behavior of a tunable potentiometer.

<p align="center">
<img src="../../images/AC_voltage_sensor_network_simulation.png" alt="AC Voltage Sensing Network Simulation" width="800"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> AC Voltage Sensor Network Simulation Circuit </h7>
</div>

<p align="center">
<img src="../../images/AC_voltage_sensor_network_simulation_plot.png" alt="AC Voltage Sensing Network Simulation Plot " width="800"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> AC Voltage Sensor Network Simulation Signals Plot </h7>
</div>




---
title: Atinverter V2 Hardware
layout: default
parent: Hardware
nav_order: 2
has_toc: false
---

{: .no_toc }

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

# **Atinverter V2 Hardware**
***

The **Atinverter Version 2 (V2)** incorporates a multitude of subsystems that work in tandem to support the board's main function as a **power inverter**. A power inverter, not to be confused with a digital logic inverter, is a type of converter that takes a DC signal and transforms it to an AC signal. 

{: .highlight-yellow }
> 💭 "Why Atinverter?"

With the aim of highlighting the relationship between the ATMEGA328P MCU and the power inverter circuitry, the name is based on: "ATMEGA328P" + "Inverter" = "Atinverter"

To better visualize the core components and the interconnections of the design, consider the following diagram: 

<p align="center">
<img src="../../images/atinverter_v2_block_diagram.png" alt="Atinverter V2 Block Diagram" width="700"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Atinverter High Level Block Diagram </h7>
</div>

## **Systems**
To better understand the full scope of the Atinverter V2, it's helpful to explore the various systems in more detail. Click on each section to dive deeper into how each subsystem contributes to the overall design.

- 🔌 [Power Inverter](atinverter_v2/power_inverter_hw)
- 💡 [LED Blinking](atinverter_v2/LED_blinking_hw)
- 📟 [DC Voltage Sensing](atinverter_v2/dc_volt_sensing_hw)
- 📟 [DC Current Sensing](atinverter_v2/dc_curr_sensing_hw)
- 📟 [AC Voltage Sensing](atinverter_v2/ac_volt_sensing_hw)
- 📟 [AC Current Sensing](atinverter_v2/ac_curr_sensing_hw)
- 🔁 [Inter-Board Communication](atinverter_v2/inter_board_comms)
- 🔼 [Boost Converter](atinverter_v2/boost_conv_hw)
- 🔽 [Buck Converter](atinverter_v2/buck_conv_hw)

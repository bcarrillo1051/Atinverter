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

The **Atinverter Version 2 (V2)** incorporates a multitude of subsystems that work in tandem to support the board’s main function as a **power inverter**. A power inverter, not to be confused with a digital logic inverter, is a type of converter that takes a **DC input** and transforms it to an **AC output**.

{: .highlight-yellow }
> 💭 "Why Atinverter?"

The name “Atinverter” reflects the close relationship between the ATMEGA328P microcontroller (MCU) and the inverter circuitry, combining "ATMEGA328P" and "inverter" into a single term.

The diagram below illustrates the core components and the interconnections of the design:

<p align="center">
<img src="../../images/atinverter_v2_block_diagram.png" alt="Atinverter V2 Block Diagram" width="700"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Atinverter High Level Block Diagram </h7>
</div>

## **Systems**
To better understand the full scope of the Atinverter V2, it's helpful to explore the various systems in more detail. Click on each section to dive deeper into how each subsystem contributes to the overall design.

- 🔌 [Power Inverter](power_inverter_hw)
- 📟 [DC Voltage Sensing](dc_volt_sensing_hw)
- 📟 [DC Current Sensing](dc_curr_sensing_hw)
- 📟 [AC Voltage Sensing](ac_volt_sensing_hw)
- 📟 [AC Current Sensing](ac_curr_sensing_hw)
- ⚡ [Overvoltage and Reset Protection](ov_reset_prot)
- 🔽 [Buck Converter](buck_conv_hw)
- 🔼 [Boost Converter](boost_conv_hw)
- 💡 [LED Blinking](LED_blinking_hw)
- 🔁 [Inter-Board Communication](inter_board_comms)

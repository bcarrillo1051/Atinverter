---
title: LED Blinking
layout: default
parent: Atinverter V2 Hardware
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

# **LED Blinking Hardware**
---

The LED circuit is effectively **four LEDs** that are independently controlled using **four different GPIO pins** of the ATMEGA328P. 

<p align="center">
<img src="../../images/LED_bootload_circuit.png" alt="LED Bootload Circuit" width="350"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> LED Blinking Circuit </h7>
</div>


These LEDs are cycled on and off using a simple blink program providing a trivial aesthetic effect, but its primary purpose is the **functional verification** of the ATMEGA328P. The visual indicator allows the user to determine if the microcontroller is **bootloaded**, which means it is capable of receiving code uploads. This check is particularly important as discrete ATMEGA328P chips are typically sold **without** a pre-installed bootloader. 
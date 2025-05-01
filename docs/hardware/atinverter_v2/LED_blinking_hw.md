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


These LEDs are cycled on and off using a blink program providing a trivial aesthetic effect, but its primary purpose is a **functional verfication** of ATMEGA328P. Using a visual indictator allows us to determine if the MCU is bootloaded, which is a necessary step when working with a **brand-new chip**.
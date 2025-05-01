---
title: AC Current Sensing
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

# **AC Current Sensing Hardware**
---

Sensing the current from the AC load is similarly achieved to sensing the DC input. On the AC side, the TMCS1108 hall-effect current sensor is likewise employed. What differs is how the signal is analyzed after being sampled by current sensor. In this case, the output analog voltage that maps to the load current is not being delivered directly to ATMEGA328P for processing and calculation, but rather to the second input channel of the ADC122S021.

<p align="center">
<img src="../../images/AC_current_sensing_block_diagram.png" alt="AC Current Sensing Block Diagram" width="800"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> AC Current Sensing Block Diagram </h7>
</div>

To gain more insight into how the algorithm for computing the AC load current is performed using the ADC122S021 sampling, please refer to the section on [AC Current Sensing](../software/modules/ac_current_sensing).
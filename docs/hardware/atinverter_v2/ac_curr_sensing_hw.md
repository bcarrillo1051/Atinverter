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

Sensing the current from the AC output load is conceptually similar to sensing the DC input current, but includes an additional intermediate step. Instead of directly providing analog voltage samples that correspond to a current reading from the TMCS1108 to the ATMEGA328P, the samples are first delivered to the second input channel of the ADC122S021.

<p align="center">
<img src="../../images/AC_current_sensing_block_diagram.png" alt="AC Current Sensing Block Diagram" width="800"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> AC Current Sensing Block Diagram </h7>
</div>

The ADC122S021 offers higher levels of resolution and better sampling performance compared to the ATMEGA328P’s built-in ADC channels. This improvement is particularly beneficial for monitoring the AC output current signal, which carries greater importance for waveform control and feedback than the DC input current. The control loop feedback, which is to be implemented in future iterations of the board, is fundamentally reliant on an accurate and high-resolution output current sampling mechanism. Ultimately, by leveraging a higher-performance ADC, the system will be better equipped to support real-time control decisions and more precise actuation of the inverter output.


To gain more insight into how the algorithm for computing the AC load current is performed using the ADC122S021, please refer to the [AC V/I Sensing Library Feature](../../software/library/features/ac_vi_sensing_feature) section.
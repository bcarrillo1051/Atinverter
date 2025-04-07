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


# **Timer 2 Delay**
***

An interface is required to handle program delays for time-sensitive operations such as AC voltage sensing and serial communication, among other tasks.

{: .highlight-yellow }
> "Why not simply use the `delay()` or `millis()` functions to accomplish this?"

Although it sounds like that would be the most straightforward way to go about it, unfortunately the ATMEGA328P timer used for the implementation of these functions, namely, Timer0 is being used for the implementation of the PWM generation as specified in [PWM Control](PWM_control) 


The `delay2()` method serves as an excellent substitute for the Arduino built-in `delay()` function. This  because  on the hardware constraints 

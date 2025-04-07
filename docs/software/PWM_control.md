---
title: PWM Control
layout: default
parent: Software
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

# **PWM Control**
***

The main objective of the PWM code is generate the necessary PWM signals that will allow us to produce a 50Hz or 60Hz sine wave.

How this is actually accomplished is specifically through two main stages: An H-bridge and an LC filter network. 

In the H-bridge, the switching signals are delivered from the ATMEGA328P to the gate drivers which drive the MOSFETs. The characteristic frequency of the switching signals (AKA switching frequency) was chosen to be $$f_{sw} = 31,372Hz$$. The supplied switching signals also have a variable duty cycle that aims to emulate the progressive increasing and decreasing behavior of a sine wave.



The H-bridge is simply the stages that allows us to have complimentary switching where a switching control signal controls a power signal that is provided to the load. 

The signal is averaged is through the LC filter. The LC filter removes the switching frequency and the fundamental frequency remains.




Let's understand the way in which the PWM code operates. 

## PWM 50Hz
$$f_{sw} = 31,372Hz$$
$$f_{sw} = \frac{1}{f_{sw}} = 31.875\mu s$$ 





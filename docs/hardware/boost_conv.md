---
title: Boost Converter
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

## Programming the Output Voltage
***
The output voltage of the boost converter is given by the following equation:

$$R_{1} = \left( \frac{V_{OUT}}{V_{REF}} - 1 \right) \times R_{2}$$

Desire an output voltage of 12V, internal reference voltage is 795mV, choose R2, solve for R1. 

- $V_{OUT} = 12V$
- $V_{REF} = 795mV$
- $R_{2} = 71.5k\Omega$

$$R_{1} \approx 1M\Omega$$

## Inductor Selection
***
To determine the inductor current (input current draw):

$$I_{L(DC)} = \frac{V_{OUT} \times I_{OUT}}{V_{IN} \times \eta}$$

- $V_{OUT}$ = 12V
- $V_{IN}$ = 5V
- $\eta$ = 0.8 (80% recommended by datasheet)
- $I_{OUT}$ = ?

Need to find the load current that will be drawn. This boost converter is only driving the two IR2302 gate drivers for our PWM Inverter H-bridge design so if we find the current consumed by these drivers, we will effectively find the output load to drive. The current consumption of a gate driver is based upon the static and dynamic current consumption. As per the IR2302 datasheet, the max possible Quiescent Vcc supply current $I_{QCC}$ = 1.6mA. Therefore, $I_{static}$ = 1.6mA. The dynamic current consumed by the device can be found by the following relation between the total gate charge of the MOSFET and the switching frequency:

$I_{dynamic}​ = Q_{g} ​× f_{sw}$

As per the IRFB4020PBF MOSFET datasheet, the maximum total gate charge is 29nC and the switching frequency of operation of the gate drivers is 31,372Hz.
- $Q_{g} = 29nC$
- $f_{sw} = 31,372Hz$

Using these values, the dynamic current consumption of one MOSFET is:

$I_{dynamic}​ = 0.91mA$

Considering that the PWM inverter topology is based on 4 MOSFETs with only 2 operating at any given time simultaneously, this means the total dynamic current is $2 \times I_{dynamic}​ = 1.82mA$. Additionally, the quiescent current is dictated by two gate drivers so the total static current is $2 \times I_{static}​ = 3.2mA$.

$I_{LOAD}​ = I_{STATIC} ​+ I_{DYNAMIC}$<br>
$I_{OUT}​ = 5.02mA$

Now that we have the total load current $I_{OUT}$ = 5.02mA, then we can solve for the inductor current using the following values:

- $V_{OUT} = 12V$
- $V_{IN} = 5V$
- $I_{OUT} = 5.02mA$
- $\eta$ = 0.8 (80% recommended by datasheet)

$$I_{L(DC)} = 15.06mA$$

The inductor ripple current is calculated with Equation 3 for an asynchronous boost converter in continuous conduction mode (CCM).

$$\Delta I_{L(\text{P-P})} = \frac{V_{IN} \times (V_{OUT} + 0.8V - V_{IN})}{L \times f_{SW} \times (V_{OUT} + 0.8V)}$$


where:
- ΔIL(P-P) is inductor ripple current 
- L is inductor value 
- f SW is switching frequency 
- VOUT is output voltage 
- VIN is input voltage



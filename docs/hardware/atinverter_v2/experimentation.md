---
title: Experimentation
layout: default
parent: Atinverter V2 Hardware
nav_order: 4
has_toc: false
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

# **Experimentation**
***
 
The experimentation is predicated on the sequential testing of the fully populated Atinverter V2 PCB as well as its integration with various software modules to evaluate and diagnose system-level functionality. 

Standard PCB assembly practices were observed throughout the process, including component orientation checks, solder joint inspections, thorough visual examination, and continuity testing.

To successfully validate the operation of the Atinverter V2 device, verification of the following core subsystems is required: 
- 5V Buck and 12V Boost Power Supplies
- ATMEGA328P Bootloaded Status
- PWM Inversion
- DC Voltage and Current Sensing
- AC Voltage and Current Sensing

In the next few sections, each subsystem is reported on their verification status, both from a hardware and software functionality standpoint.

## Workbenches
In order to conduct proper testing, a reliable work bench was operated to conduct the various

## 5V Buck and 12V Boost Power Supplies

<br>
<p align="center">
<img src="../../images/experimentation/power_supplies/5V_buck_output.png" alt="R-78HE5.0-0.3 5V Buck Output" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> R-78HE5.0-0.3 5V Buck Output </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/power_supplies/12V_boost_output.png" alt="TLV61046ADBVR 12V Boost Output" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> TLV61046ADBVR 12V Boost Output </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/power_supplies/12V_boost_output_ripple.png" alt="TLV61046ADBVR 12V Boost Output Ripple" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> TLV61046ADBVR 12V Boost Output Ripple </h7>
</div>
<br>

## ATMEGA328P Bootloaded Status

- Verification of `Blink.ino' software and ATMEGA328P bootloaded state

<br>
<p align="center">
<img src="../../images/experimentation/LED_blinking.png" alt="Atinverter V2 LED Blinking Visual" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Atinverter V2 Visual LED Blinking </h7>
</div>
<br>

ATMEGA328P is functional and performs LED blinking as expected from the 'Blink.ino' program

## PWM Inversion

### PWM Generation

- Verification of `50Hz_PWM_Inverter.ino' and '60Hz_PWM_Inverter.ino software and ATMEGA328P bootloaded state

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/PWMA_and_PWMB_50Hz_f_AC.png" alt="PWMA and PWMB Waveforms 50Hz Fundamental" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> PWMA (Yellow) and PWMB (Green) Waveforms with $f_{sw} = 31,372kHz, f_{ac} = 50Hz$ </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/PWMA_and_PWMB_60Hz_f_AC.png" alt="PWMA and PWMB Waveforms" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> PWMA (Green) and PWMB (Yellow) Waveforms with $f_{sw} = 31,372kHz, f_{ac} = 60Hz$ </h7>
</div>
<br>

### Gate Driver

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/50Hz_12Vin_Gate_Drive_Signals.png" alt="12Vin_Gate_Drive_Signals" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> High Side Switch Gate Drive Waveforms, PWMA_HO (Color) and PWMB_HO (Color), Low Side Switch Gate Drive Waveforms, PWMA_LO (Color) and PWMB_LO (Color), with $V_{DC_{in}} = 12V$ and $f_{ac} = 50Hz$ </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/60Hz_12Vin_Gate_Drive_Signals.png" alt="60Hz PWMA_HO, PWMB_HO,PWMA_LO, and PWMB_LO Waveforms" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> High Side Switch Gate Drive Waveforms, PWMA_HO (Color) and PWMB_HO (Color), Low Side Switch Gate Drive Waveforms, PWMA_LO (Color) and PWMB_LO (Color), with $V_{DC_{in}} = 12V$ and $f_{ac} = 60Hz$ </h7>
</div>
<br>

### H-Bridge and LC Filter

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/12Vin_load_s+_load-_50Hz.png" alt="50Hz Load+ and Load- Voltage Waveforms" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> 50Hz Output Waveform at Load+ (Green) and Load− (Yellow) with $V_{DC_{in}} = 12V$ </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/12Vin_load_s+_load-_AC_Output_50Hz.png" alt="50Hz AC Output Waveform" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> 50Hz Differential AC Output Waveform (Pink) with $V_{DC_{in}} = 12V$ </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/12Vin_load_s+_load-_60Hz.png" alt="60Hz Load+ and Load- Voltage Waveforms" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> 60Hz Output Waveform at Load+ (Green) and Load− (Yellow) with $V_{DC_{in}} = 12V$ </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/pwm_inversion/12Vin_load_s+_load-_AC_Output_60Hz.png" alt="60Hz AC Output Waveform" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> 60Hz Differential AC Output Waveform (Pink) with $V_{DC_{in}} = 12V$</h7>
</div>
<br>

## Sensing Networks

The fundamental frequency of operation (50Hz or 60Hz) should not significantly affect the input current reading voltage and current sensing values for 12V is reflec 

## DC Voltage Sensing

<br>
<p align="center">
<img src="../../images/experimentation/dc_voltage_sensing/12Vin_multimeter.png" alt="12V DC Input" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> 12V DC Input Multimeter Measurement (Color) </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/dc_voltage_sensing/12Vdc_input_testing_steady_state_full_window2.png" alt="12V DC Input" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Arduino IDE Serial Monitor DC Voltage Readouts </h7>
</div>
<br>



## DC Current Sensing

<br>
<p align="center">
<img src="../../images/experimentation/dc_current_sensing/24.5_ohm_load_Idc_readings.png" alt="24.5 Ohm Load Idc Readings" width="180"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Arduino IDE Serial Monitor DC Current Readouts for $24.5\Omega$ Load </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/dc_current_sensing/18_ohm_load_Idc_readings.png" alt="18 Ohm Load Idc Readings" width="180"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Arduino IDE Serial Monitor DC Current Readouts for $18\Omega$ Load </h7>
</div>
<br>

## AC Voltage Sensing

<br>
<p align="center">
<img src="../../images/experimentation/ac_voltage_sensing/12Vin.png" alt="DC Current Input" width="600"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> 12V DC Input Waveform (Color) </h7>
</div>
<br>

<br>
<p align="center">
<img src="../../images/experimentation/ac_voltage_sensing/12Vin_serial_monitor_readouts.png" alt="12V DC Input" width="300"/>
</p>

<div style="text-align: center;">
    <h7><b>Figure X.</b> Arduino Serial Monitor DC Voltage Readouts (Color) </h7>
</div>
<br>

## AC Current Sensing
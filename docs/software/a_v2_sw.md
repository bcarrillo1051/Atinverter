---
title: Atinverter V2 Software
layout: default
parent: Software
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

# **Atinverter V2 Software**
***

The Atinverter V2 codebase is composed of a `Library` folder as well as well various module folders (`Blink`, `Main`, `PWM_Inverting`, and `VI_Sensing`). The Atinverter library contains the object-oriented coding framework that allows for intuitive intialization, control, and sensing of the Atinverter V2 hardware. The various ready-to-run and deployable modules serve the purpose of showcasing the different features of the hardware by incorporating and applying the Atinverter library. The following diagram provides a visual representation of the software hierarchy and the available `C++` and Arduino `.ino` files that the user can utilize to enable Atinverter V2 hardware operation.
<br>
<br>

```mermaid
    graph LR;

    %% Node Definitions
    A(**software**)
    style A fill:skyblue,stroke:white,stroke-width:2px

    B(**Blink**)
    style B fill:skyblue,stroke:white,stroke-width:2px
    C(**Library**)
    style C fill:skyblue,stroke:white,stroke-width:2px
    D(**Main**)
    style D fill:skyblue,stroke:white,stroke-width:2px
    E(**PWM_Inverting**)
    style E fill:skyblue,stroke:white,stroke-width:2px
    F(**VI_Sensing**)
    style F fill:skyblue,stroke:white,stroke-width:2px

    G(**Blink.ino**)
    style G fill:skyblue,stroke:white,stroke-width:2px
    H(**Atinverter.cpp**)
    style H fill:skyblue,stroke:white,stroke-width:2px
    I(**Atinverter.h**)
    style I fill:skyblue,stroke:white,stroke-width:2px
    J(**Main.ino**)
    style J fill:skyblue,stroke:white,stroke-width:2px
    K(**50Hz_PWM_Inverter.ino**)
    style K fill:skyblue,stroke:white,stroke-width:2px
    L(**60Hz_PWM_Inverter.ino**)
    style L fill:skyblue,stroke:white,stroke-width:2px
    M(**Vdc_Sensing.ino**)
    style M fill:skyblue,stroke:white,stroke-width:2px
    N(**Idc_Sensing.ino**)
    style N fill:skyblue,stroke:white,stroke-width:2px
    O(**Vac_Sensing.ino**)
    style O fill:skyblue,stroke:white,stroke-width:2px
    P(**Iac_Sensing.ino**)
    style P fill:skyblue,stroke:white,stroke-width:2px

    %% Node Connections
    A --> B
    A --> C
    A --> D
    A --> E
    A --> F

    B --> G

    C --> H
    C --> I

    D --> J

    E --> K
    E --> L

    F --> M
    F --> N
    F --> O
    F --> P

    %% Change color of arrow heads
    %%{init: {'themeVariables': {'lineColor': 'white'}}}%%

    %% Apply color to the arrows
    %% 0: 1st arrow (A --> B) 1: 2nd arrow (A --> C) 2: 3rd arrow (A --> D) 3: 4th arrow (A --> E) 4: 5th arrow (A --> F)
    linkStyle 0 stroke:white, stroke-width:3px;
    linkStyle 1 stroke:white, stroke-width:3px;
    linkStyle 2 stroke:white, stroke-width:3px;
    linkStyle 3 stroke:white, stroke-width:3px;
    linkStyle 4 stroke:white, stroke-width:3px;
    
    %% 5: 6th arrow (B --> G)
    linkStyle 5 stroke:white, stroke-width:3px;

    %% 6: 7th arrow (C --> H) 7: 8th arrow (C --> I)
    linkStyle 6 stroke:white, stroke-width:3px;
    linkStyle 7 stroke:white, stroke-width:3px;

    %% 8: 9th arrow (D --> J)
    linkStyle 8 stroke:white, stroke-width:3px;

    %% 9: 10th arrow (E --> K) 10: 11th arrow (E --> J)
    linkStyle 9 stroke:white, stroke-width:3px;
    linkStyle 10 stroke:white, stroke-width:3px;

    %% 11: 12th arrow (F --> M) 12: 13th arrow (F --> N) 13: 14th arrow (F --> O) 14: 15th arrow (F --> P)
    linkStyle 11 stroke:white, stroke-width:3px;
    linkStyle 12 stroke:white, stroke-width:3px;
    linkStyle 13 stroke:white, stroke-width:3px;
    linkStyle 14 stroke:white, stroke-width:3px;
```
<div style="text-align: center;">
    <h7><b>Figure X.</b> Atinverter V2 Software Modules and Files</h7>
</div>

---
title: Software
layout: default
nav_order: 1
---

# **Software**
***

The Atinverter codebase is composed of various modules, each contributing to distinct functionalities. \
The following diagram showcases the available features of the Atinverter codebase. Each box in the \
diagram is interactive, allowing you to click and explore detailed information about specific features.

```mermaid
   
   %%{init: {'theme':'base'}}%%
   flowchart TB

    subgraph Future
       G(Rectification)
       H(Droop Control)
        
    end

    subgraph Capabilities
       D(Blink)
       E(PWM Generation 50Hz)
       F(PWM Generation 60Hz)
       I(DC Voltage/Current Sensing)
       J(AC Voltage/Current Sensing)

    end

    subgraph Main
        C(Main)
    end

    subgraph Library
       A(Atintverter.cpp) 
       B(Atinverter.h)
    end


    Library-->Main
    Capabilities-->Main

    %% Change color of arrow heads
    %%{init: {'themeVariables': {'lineColor': 'grey'}}}%%

    %% Apply color to the arrows
    %% 0: 1st arrow (Library --> Main) 1: 2nd arrow (Capabilities --> Main)
    linkStyle 0 stroke:grey, stroke-width:3px;
    linkStyle 1 stroke:grey, stroke-width:3px;

    %% Adding clickable links, Syntax: Destination ref, hover label
    click E "PWM_control" "PWM Generation"
    click F "PWM_control" "PWM Generation"
```
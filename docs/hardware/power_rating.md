---
title: Atinverter V1/2 Power Rating
layout: default
parent: Hardware
nav_order: 4
---

# **Hardware**
***

{: .highlight-green }
MOSFETs (IRFB4020PBF):
- **Continuous Drain Current (I<sub>D</sub>):** 18 A at 25°C
- **Drain-to-Source Voltage (V<sub>DSS</sub>):** 200 V
- **Maximum Power Dissipation (P<sub>D</sub>):** 100 W at 25°C

{: .highlight-green }
Inductors (AIRD-02-221K):
- **Inductance:** 220 µH ​
- **Current Rating:**
  - Continuous: 2.8 A
  - Saturation: 7 A

{: .highlight-green }
Capacitors (1189-4160-1-ND):
- **Rated Voltage** 250V
- **Ripple Current @ Low Frequency:** 188 mA @ 120 Hz 
- **Ripple Current @ High Frequency:** 470 mA @ 100 kHz

Theoretical max:
- P<sub>max</sub> = V<sub>max</sub> * I<sub>max</sub> = 200V * 2.8A = 560W

BUUUTTTT — that's theoretical! Needn to consider:
- Need to consider proper heatsinking for the MOSFETs (18 A is at 25°C, derates with temp)
- Ripple current through cap < 470mA
- Safe design margin: do not run at 100% ratings
- Consider switching trajectory

Reasonably speaking somwhere up to 450W
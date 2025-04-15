---
title: To-Do
layout: default
nav_order: 6
---

# **TO-DO**


- [X] Order NOR gate and verify that logic protection circuitry is operational
- [ ] Fix schematic and GPIO pin of Rpi connected to DTR line which has a 5V pull up (bad since Rpi is 3.3V logic)
- [ ] Verify that schematic, PCB, and 3D model is not missing anything (run DRC again just in case)
- [ ] Load testing captures
- [ ] Modify the startPWM method such that it does not pass a boolean parameter to select between 50Hz or 60Hz PWM (not intuive for the "false" parameter to indicate start 60Hz, sounds more like false means off)
- [ ] Remove seperate pwmISR() method in Timer1ISR since that actually results in longer program operation which is not ideal in the ISR (ideally minize time in ISR)
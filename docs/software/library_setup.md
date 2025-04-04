---
title: Atinverter Library Set Up
layout: default
parent: Software
nav_order: 1
---

# **Atinverter Library Set Up**
### A guide to help your Arduino IDE recognize and use the Atinverter user-defined library
***

You might be thinking:

> “Okay, I downloaded the software files from the GitHub repository — now what? Can I just open `blink.ino` and run the code?”

You're super close! There's just **one important step** that needs to happen first: we need to make sure the **Arduino IDE knows where to find the Atinverter library files** (`Atinverter.cpp`, `Atinverter.h`), or else you’ll run into the infamous:

> `fatal error: Atinverter.h: No such file or directory`

---

## 📁 How Arduino Handles Libraries

The Arduino IDE looks for **user-defined libraries** in a specific directory on your computer. Simply placing `.cpp` or `.h` files next to your `.ino` isn't enough — they must be installed or placed in the correct location.

Here’s where those locations are:

🔍 If you're on Windows, it'll be in:
> `C:\Users<YourUsername>\Documents\Arduino\libraries\`

🔍 If you're on macOS or Linux, it'll be in:
> `~/Documents/Arduino/libraries/`

---

## ✅ What To Do

1. Locate the folder in the Atinverter repo named `Library`.  
   It contains `Atinverter.cpp` and `Atinverter.h`

2. Copy that folder into your Arduino `libraries` directory:
> `C:\Users<YourUsername>\Documents\Arduino\libraries\`

3. Rename the folder from `Library` to:
> `Atinverter`

📸 You should see something like this:

![User Defined Library Directory](../images/user_defined_lib.png)

---

## 🚀 You're Good to Go

Once the Atinverter library is in the right place:
- You can use it in any sketch with:
  ```cpp
  #include "Atinverter.h"
  ```

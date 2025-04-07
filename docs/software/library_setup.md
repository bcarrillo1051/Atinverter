---
title: Atinverter Library Set Up
layout: default
parent: Software
nav_order: 1
---

# **Atinverter Library Set Up**
### A guide to help your Arduino IDE recognize and use the Atinverter user-defined library
---

You might be thinking:

{: .highlight-yellow }
> “Okay, I downloaded the software files from the GitHub repository — now what? Can I just open some random file like `blink.ino` and run the code?”

You're super close! There's just **one important step** that needs to happen first: we need to make sure the **Arduino IDE knows where to find the Atinverter library files** (`Atinverter.cpp`, `Atinverter.h`), or else you’ll run into the infamous:

{: .highlight-red }
> `fatal error: Atinverter.h: No such file or directory`

---

## 📁 How Arduino Handles Libraries

The Arduino IDE looks for **user-defined libraries** in a specific directory on your computer. Simply placing `.cpp` or `.h` files next to your `.ino` isn't enough — they must be placed in the correct location.

Here’s where those locations are:

🔍 If you're on Windows, it'll be in:

{: .highlight-greylt }
> `C:\Users<YourUsername>\Documents\Arduino\libraries\`


🔍 If you're on macOS or Linux, it'll be in:

{: .highlight-greylt }
> `~/Documents/Arduino/libraries/`

---

## ✅ What To Do

1. Go to the `Libraries` folder in the Atinverter repo. There you will see the library named `Atinverter`. This folder contains the source code `Atinverter.cpp` and `Atinverter.h` as well as the metadata `library.properties`. It is located in:

{: .highlight-greylt }
> `\AtInverter\software\Libraries`

2. Copy the `Atinverter` folder to your Arduino `libraries` folder, located as described in \
**How Arduino Handles Libraries**

📸 You should see something like this in your Arduino `libraries` folder:

![User Defined Library Directory](../images/user_defined_lib.png)

---

## 🚀 You're Good to Go

Once the Atinverter library is in the right place:
- You can use it in any sketch with:
  ```cpp
  #include "Atinverter.h"
  ```

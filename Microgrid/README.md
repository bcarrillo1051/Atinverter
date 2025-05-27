Microgrid Connection

Description: 
This folder contains the necessary programs and
instructions to connect a Raspberry Pi as the 
overhead controller to interface with the 
Atverter and Atinverter. The steps on how to setup
the whole system is shown below.


Setup Instructions
These instructions will allow you to set up a Raspberry Pi controlled Microgrid with a blank Atmega328P to be programmable via the USB TTL FTDI 5V cable from the Arduino environment.
Flash Pi with Raspian
Download the Raspi imager and OS from URL: https://www.raspberrypi.com/software/
Insert the microSD card into your computer using an SD card adapter or built-in slot
Open the imager and choose the Raspberry Pi OS (32-bit)
Choose the SD card as the storage device
Preconfigure settings like hostname, password, enable SSH, and wifi and record these values for later use along with the IP address
Flash Raspi OS to micro SD by clicking the write button
After it has finished flashing, put micro SD card in Pi and power it up.
Precursor to connecting the Pi
Below is a list of steps to be able to control the Pi from another computer. There are many different ways of doing this and the described process is just the way things were run for some of this testing.
However, there is always the option to connect a monitor, keyboard, and mouse to the Pi and directly use it like a personal computer. This gives a much more intuitive user interface for those that have not used the ssh command to control devices before. There are also ways to use a VNC to mirror the Pi’s screen to a personal device and control it that way. If this method is used then skip the next steps until downloading the code base.
Allow a computer to be able to display your Pi programs and connect to the Pi
Download these two programs to show programs from your Pi on your personal computer
PuTTY: https://www.putty.org/
VcXsrv: https://vcxsrv.com/
Run both install clients
To begin, run Xlaunch and set these settings
•	Select “Multiple windows”
•	Choose “Start no client”
•	Check “Disable access control” (to allow your Pi to connect easily)
•	Click Finish — VcXsrv starts running in the background (system tray)
Now X-11 forwarding is established on this device
Verify that both devices are on the same wifi network before connecting
Next, launch PuTTY and set these settings
•	Go to Connection > SSH > X11
•	Check “Enable X11 forwarding”
•	Go to Session
•	In the IP address section type pi@[IP Address]
•	Click the Open button
Now a terminal should launch and ask for the password that was input earlier
This terminal now is directly linked to the Pi and can display running apps
Use these commands to download the code base (Open a terminal):
cd Documents
sudo apt update
sudo apt upgrade -y
sudo apt install -y git
git clone https://github.com/bcarrillo1051/Atinverter.git
Download the Arduino IDE
Run this command in the terminal:
sudo apt install arduino -y
This downloads the arduino IDE to allow easy access to the software
Open it by running this command in the Pi terminal or in the PuTTY window:
arduino
Burn the Bootloader to the Atmega Chip
If the board has never been programmed before or the Atmega chip is new, the Arduino bootloader needs to be flashed to the chip
Follow the steps in this tutorial below:
https://www.instructables.com/How-to-Burn-the-Arduino-BootLoader-on-to-a-AtMega3/
Program the Chip on the Atverter
Open a terminal and navigate through the Github to the library files for the Atverter (location)
Copy this folder over to the new Arduino folder created in documents
Navigate to the location of the Atverter main.ino file (location)
Open this file up using the Arduino IDE
Use the USB-to-FTDI cable to connect the Rasberry Pi to the 6 header pins on the Atverter
Press the upload button at the top left of the IDE and confirm a small window pops up saying upload complete
Program the Chip on the Atinverter
Open a terminal and navigate through the Github to the library files for the Atinverter (location)
Copy this folder over to the new Arduino folder created in documents
Navigate to the location of the Atverter main.ino file (location)
Open this file up using the Arduino IDE
Use the USB-to-FTDI cable to connect the Rasberry Pi to the 6 header pins on the Atinverter
Press the upload button at the top left of the IDE and confirm a small window pops up saying upload complete
Hardware Connection
There are two different types of connections that much occur to communicate to the Pi
Atinverter: Directly plug the Pi into the bottom header on the PCB of the Atinverter
Atverter: Plug the USB side of the USB-to-Serial cable into the first Pi USB slot. Connect the other side to the 6 pin FTDI header on the Atverter
Raspberry Pi Control Program
Check if Python is installed on this Pi, otherwise install it
Depending on where the github folder was saved, navigate to the Main.py file location and enter these commands
cd Documents/Atinverter/Microgrid/Raspi_Files
python Main.py
The GUI window should show up and allow anyone to alter how the microgrid functions

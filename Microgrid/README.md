Full Setup Instructions
Setup Instructions
These instructions will allow you to set up a Raspberry Pi-controlled Microgrid with a blank Atmega328P to be programmable via the USB TTL FTDI 5V cable from the Arduino environment.

Flash Pi with Raspberry Pi OS
Download the Raspberry Pi Imager and OS from: https://www.raspberrypi.com/software/

Insert the microSD card into your computer using an SD card adapter or built-in slot.

Open the Imager and choose the Raspberry Pi OS (32-bit).

Choose the SD card as the storage device.

Preconfigure settings like hostname, password, enable SSH, and Wi-Fi. Record these values for later use along with the Pi's IP address.

Click Write to flash the OS to the microSD card.

After flashing, insert the card into the Raspberry Pi and power it up.

Remote Access Setup
The following steps will allow you to control the Raspberry Pi from another computer. You may also directly connect a monitor, keyboard, and mouse for a traditional setup. VNC can also be used for a graphical remote desktop; if using VNC, you may skip the SSH/X11 steps.

Enable Display of Pi GUI Programs on a PC
Download and install:

PuTTY

VcXsrv

Run XLaunch and use these settings:

Select “Multiple windows”

Choose “Start no client”

Check “Disable access control”

Click Finish (VcXsrv will run in the background)

Make sure your PC and Raspberry Pi are on the same Wi-Fi network.

Open PuTTY and:

Go to Connection > SSH > X11, and check “Enable X11 forwarding”

In the Session tab, enter the IP address as pi@<IP Address>

Click Open

Enter the password when prompted. You now have a terminal linked to the Raspberry Pi.

Download the Code Base
In the SSH terminal or directly on the Pi:

cd Documents
sudo apt update
sudo apt upgrade -y
sudo apt install -y git
git clone https://github.com/bcarrillo1051/Atinverter.git
Install the Arduino IDE
Run the following in the Pi terminal:

sudo apt install arduino -y
To open it later:

arduino
Burn the Bootloader to the ATmega328P
If using a new or blank chip, the Arduino bootloader must be installed.

Follow this tutorial:
https://www.instructables.com/How-to-Burn-the-Arduino-BootLoader-on-to-a-AtMega3/

Program the Chip on the Atverter
Navigate to the GitHub repository folder.

Copy the relevant library folder to the Arduino directory in Documents.

Open the main.ino file in the Arduino IDE.

Connect the Raspberry Pi to the Atverter via USB-to-FTDI cable.

Click the upload button in the Arduino IDE and wait for confirmation.

To operate, merely leave the USB-to-FTDI cable attached to the Atverter to communicate using UART commands.

Program the Chip on the Atinverter
Navigate to the GitHub repository folder.

Copy the relevant library folder to the Arduino directory in Documents.

Open the main.ino file in the Arduino IDE.

Connect the Raspberry Pi to the Atinverter via USB-to-FTDI cable.

Click the upload button in the Arduino IDE and wait for confirmation.

Hardware Connection Instructions
Atinverter: Directly plug the Raspberry Pi into the bottom header on the PCB of the Atinverter.

Atverter: Plug the USB side of the USB-to-Serial cable into the Pi, and connect the 6-pin FTDI header to the Atverter.

Run the Raspberry Pi Control Program
Make sure Python is installed. If not:

sudo apt install python3 python3-pip
If using the Atinverter, confirm that I2C communication is enabled on the Pi in the config settings.

Navigate to the program folder and run the main file:

cd Documents/Atinverter/Microgrid/Raspi_Files
python3 main.py
A GUI window should appear, allowing control of the microgrid.
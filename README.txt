This repository contains the anbound-lock firmware project. It's an open source project that contains firmware to turn any anbound lock into a timer lock.
It contains code from https://github.com/AustinHaigh/CCLoader-RPi and https://github.com/JimGeek/TI-CC2541-BLE
An extra command is included, the CCReader command that lets you read firmware from a lock. I've also included a binary file that contains the original firmware,
in case you want to restore your lock to the normal firmware.

What do you need to build and use this:
 - A raspberry pi or pi zero
 - A bit of soldering skill
 - An anbound brand lock, for example: 
   https://www.ebay.com/itm/Bluetooth-Smart-Lock-Anti-Theft-Alarm-Lock-for-Cycling-Motorcycle-Door-Smart-Hom/183806794580
 
How to use it:
  - Open up the lock, you can remove the back cover by turning it once the lock's buckle is open
  - Unscrew the lock and remove the PCB
  - Attach 3.3v pin of raspberry pi to VCC on PCB
  - Attach GND of raspberry pi to GND on PBC
  - Attach DC, DD and RST to GPIO pins on raspberry pi and save them
  - Start raspberry pi and clone this git repository
  - Install sdcc: apt-get install sdcc
  - Run make in the git repository folder: make all
  - use CCLoader to write the timer_lock.bin to the device
  
Once the firmware is on the lock, reassemble it. With the lock assembled, open the shackle. Both red and blue lights will come on, then
the lock will emit blue light from the button. Press one time for each 10 minutes you want the lock to stay locked. So one press = 10 minutes, two = 20, etc.
After a few seconds a red light will come on. Press one time for each hour you want the lock to stay locked. So one press = 1 hour, two = 2 hours etc.
After the red light turns off, close the shackle and press the button. The led will flash in red one time for each hour you set - and in blue one time for each 10 minutes you set.
After this the lights will blink rapidly for a few seconds, while they blink you can still open the lock. Once the lights stop blinking the lock locks itself for the set amount of time
There is no way to open it but to wait.


  

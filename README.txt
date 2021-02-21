This repository contains the anbound-lock firmware project. It's an open source project that contains firmware to turn any anbound lock into a timer lock.
It contains code from https://github.com/AustinHaigh/CCLoader-RPi and https://github.com/JimGeek/TI-CC2541-BLE
An extra command is included in CCLoader: -f . It lets you read firmware from a lock. I've also included a binary file that contains the original firmware,
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
  - Clean up residue: make clean
  - use CCLoader to write the timer_lock.bin to the device : ./CCLoader --DC=21 --DD=20 --RESET=26 timer_lock.bin

Once the firmware is on the lock, reassemble it.

To use the lock: open the shackle and press the button both red and blue lights will blink. If you hold the button down for as long as the red and blue lights blink the lock will go into random mode - and the time you set is the *maximum* time. In random mode a random duration in minutes between 0 and the time you set will be used.
The lock will emit blue light from the button. Press one time for each 10 minutes you want the lock to stay locked. So one press = 10 minutes, two = 20, etc.
After a few seconds a red light will come on. Press one time for each hour you want the lock to stay locked. So one press = 1 hour, two = 2 hours etc.
After a few seconds both red and blue lights will come on. Press one time for each day you want the lock to stay locked. So one press = 1 day, two = 2 day etc.
The total time is days+hours+minutes.

After the lights turn off, close the shackle and press the button. The led will flash in red one time for each hour you set - and in blue one time for each 10 minutes you set, and in red+blue for each day you set.
After this the lights will blink rapidly for a few seconds if the random mode is enabled. If the time was mistaken - you can now still open the lock. Once you hear the lock click it is closed and all you can do is wait until it opens again.

Notice: this is free software. Covered by the GPL. You are free to use it as you see fit, free to modify it as you see fit, and sell it as you see fit. Anyone who thinks otherwise can kiss my ass.

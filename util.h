#include "cc254x_types.h"

/* even if we don't use this interrupt, make sure we have a vector for it
   so that the interrupt table has at least 17 entries, otherwise SDCC makes
   a sorter table and places code just after it :s */
void isr_wdt() __interrupt(17);
uint16 rnd(); //Generate a 16 bits random number
uint16 read_vdd(); //Read the device input voltage
uint16 read_temperature(); //Read the chip temperature sensor
void slow_clockspeed();//set the clockspeed to the slowest option/divider

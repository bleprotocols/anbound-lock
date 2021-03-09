#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#define HOUR_LED 0x20 //red
#define MINUTE_LED 0x10 //blue
#define DAY_LED  0x08
//0x01 and 0x02 are motor forward/reverse

#define MOTOR_FAST 0x01
#define MOTOR_SLOW 0x02
#include "../common/cc254x_types.h"

void open_lock();
void close_lock();
void led_on(const uint16 led);
void led_off(const uint16 led);
void blink(const uint16 led, const uint16 count, const bool longblink);
int flash_leds();

#endif

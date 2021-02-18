#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#define HOUR_LED 0x10
#define MINUTE_LED 0x20

#include "cc254x_types.h"

void open_lock();
void close_lock();
void led_on(const uint16 led);
void led_off(const uint16 led);
void blink(const uint16 led, const uint16 count);
void flash_leds();

#endif

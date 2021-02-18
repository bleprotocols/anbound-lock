#include "cc254x_types.h"
#include "cc254x_map.h"
#include "pm_sleep.h"
#include "util.h"
#include "time.h"

#define NOOP __asm nop __endasm;

#define HOUR_LED 0x10
#define MINUTE_LED 0x20
#define OPEN_RETRIES 50

//Don't leave anything dangling after using output or input pins. This wastes power.
//set pin as output/input before actuating it too.
//REGISTER 0 PIN 0X1 contains the lock's shackle state. It's pull-down, so when the shackle is closed the pin is low.
int shackle_closed() {
    int ret = 0;
    P0INP |= 1;
    P0 |= 1;
    NOOP;
    NOOP;
    NOOP;
    NOOP;
    ret = !(P0 & 1);
    P0 &= ~1;
    P0INP &= ~0X01;
    return ret;
}

//REGISTER 1 BIT 0X10 contains the lock's button state. It's pull-down, so when the button is pressed the pin is low.
int button_pressed() {
    int ret = 0;
    P1INP |= 0X10;
    P1 |= 0X10;
    NOOP;
    NOOP;
    NOOP;
    NOOP;
    ret = !(P1 & 0x10);
    P1INP &= ~0x10;
    P1 &= ~0x10;
    return ret;
}

//REGISTER 1  bit 0x1 is the actuator to open the lock.
void open_lock() {
    P1DIR |= 1;
    P1 |= 1;
    pm_sleep(1, 50);
    P1 &= ~1;
    P1DIR &= ~1;
}

//register 1 bit 0x02 is the actuator to close the lock
void close_lock() {
    P1DIR |= 2;
    P1 |= 2;
    pm_sleep(1, 50);
    P1 &= ~2;
    P1DIR &= ~2;
}

void led_on(const unsigned int led) {
    P0DIR |= led;
    P0 &= ~led;
}

void led_off(const unsigned int led) {
    P0 |= led;
    P0DIR &= ~led;
}

void blink(const unsigned int led, const unsigned int count) {
    unsigned int i = 0;
    for (i = 0; i < count; i++) {
        P0DIR |= led;
        P0 |= led;
        pm_sleep(1, 200);
        P0 &= ~led;
        pm_sleep(1, 300);
        P0 |= led;
        P0DIR &= ~ led;
    }
}

void blink_blue() {
    blink(0x20, 1);
}

int input_count(const unsigned int led) {
    int ret = 0;
    int last_pressed = 0;
    unsigned int i=0;
    led_on(led);

    for (i = 0; last_pressed < 30; last_pressed++) {
        if (button_pressed()) {
            blink(led, 1);
            led_on(led);
            ret++;
            last_pressed=0;
        }

        pm_sleep(1, 100);
    }
    led_off(led);

    return ret;
}

void flash_leds() {
    unsigned int i = 0;
    for (i = 0; i < 70; i++) {
        switch (i % 5) {
        case 0:
            led_on(HOUR_LED);
            break;

        case 1:
            led_on(MINUTE_LED);
            break;

        case 2:
            led_off(HOUR_LED);
            break;

        case 3:
            led_off(MINUTE_LED);
            break;
        }

        pm_sleep(1, 30);
    }

    led_off(HOUR_LED);
    led_off(MINUTE_LED);
}

void sleep_minutes(const unsigned long int minutes) {
    unsigned long int i;
    unsigned int n;
    for (i = 0; i < minutes; i++) {
        for(n=0;n<6;n++){
          pm_sleep(0,9910);
          if (!shackle_closed() && !shackle_closed() && !shackle_closed()) return;
        }

        blink(MINUTE_LED, 1);
    }
}

int main(void) {
    unsigned long int minutes = 0;
    unsigned long int hours = 0;
    unsigned int i=0;
    
    //slow down our device
    open_lock();
   slow_clockspeed();


    while (1) {
        if (button_pressed()) {
            if (shackle_closed()) {
                open_lock();
                pm_sleep(0, 1000);

                blink(MINUTE_LED, minutes);
                blink(HOUR_LED, hours);

                pm_sleep(0, 2000);
                flash_leds();

                if (shackle_closed()) {
                    close_lock();

                    sleep_minutes(minutes * 10);
                    sleep_minutes(hours * 60);

                    minutes = 0;
                    hours = 0;

                    for (i = 0; i < OPEN_RETRIES; i++) {
                        open_lock();
                        pm_sleep(0, 10000);
                        if (!shackle_closed() && !shackle_closed() && !shackle_closed()) break;
                    }
                }
            } else {
                led_on(HOUR_LED);
                led_on(MINUTE_LED);
                pm_sleep(1, 2000);
                led_off(HOUR_LED);
                led_off(MINUTE_LED);
                minutes = input_count(MINUTE_LED);
                hours = input_count(HOUR_LED);
            }
        }

        pm_sleep(0, 1000);
    }
}

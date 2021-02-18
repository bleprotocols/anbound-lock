#include "cc254x_types.h"
#include "cc254x_map.h"
#include "pm_sleep.h"
#include "util.h"
#include "time.h"
#include "input.h"
#include "output.h"

#define OPEN_RETRIES 50


int16 input_count(const uint16 led)
{
    int16 ret = 0;
    uint16 last_pressed = 0;
    uint16 i = 0;
    led_on(led);

    for (i = 0; last_pressed < 30; last_pressed++) {
        if (button_pressed()) {
            blink(led, 1);
            led_on(led);
            ret++;
            last_pressed = 0;
        }

        pm_sleep(1, 100);
    }

    led_off(led);
    return ret;
}


void sleep_minutes(const uint32 minutes)
{
    uint32 i = 0;
    unsigned int n;

    for (i = 0; i < minutes; i++) {
        for (n = 0; n < 6; n++) {
            pm_sleep(0, 9910);

            if (!shackle_closed() && !shackle_closed() && !shackle_closed()) {
                return;
            }
        }

        blink(MINUTE_LED, 1);
    }
}

void safe_open_lock()
{
    uint8 i = 0;

    for (i = 0; i < OPEN_RETRIES; i++) {
        open_lock();
        pm_sleep(0, 10000);

        if (!shackle_closed() && !shackle_closed() && !shackle_closed()) {
            break;
        }
    }
}

int main(void)
{
    uint32 minutes = 0;
    uint32 hours = 0;
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
                    safe_open_lock();
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

#include "../common/cc254x_types.h"
#include "../common/cc254x_map.h"
#include "../common/util.h"
#include "../common/pm_sleep.h"
#include "input.h"
#include "output.h"

#define OPEN_RETRIES 50
//3 weeks in hours
#define MAX_LOCKMINS 0x7620

int16 input_count(const uint8 led)
{
    int16 ret = 0;
    uint16 last_pressed = 0;
    uint16 i = 0;
    led_on(led);

    for (i = 0; last_pressed < 30; last_pressed++) {
        if (button_pressed()) {
            blink(led, 1, 0);
            led_on(led);
            ret++;
            last_pressed = 0;
        }

        pm_sleep(1, 100);
    }

    led_off(led);
    return ret;
}


void sleep_minutes(const uint32 minutes, const uint8 do_blink)
{
    uint32 i = 0;
    unsigned int n;

    for (i = 0; i < minutes; i++) {
        for (n = 0; n < 6; n++) {
            pm_sleep(0, 9910);
        }

        if (do_blink) {
            blink(MINUTE_LED, 1, 0);
        }
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

int safe_to_lock(uint32 minutes)
{
    return ((MAX_LOCKMINS * battery_soc() ) / 100) >= minutes;
}

int main(void)
{
    uint32 minutes = 0;
    uint32 hours = 0;
    uint32 days = 0;
    int is_random = 0;
    //slow down our device
    slow_clockspeed();
    open_lock();

    while (1) {
        if (button_pressed()) {
            if (shackle_closed()) {
                open_lock();
                pm_sleep(0, 2000);
                blink(MINUTE_LED, minutes, 1);
                blink(HOUR_LED, hours, 1);
                blink(DAY_LED, days, 1);
                pm_sleep(0, 4000);

                if (is_random) {
                    flash_leds();
                }

                if (shackle_closed()) {
                    minutes *= 10;
                    minutes += hours * 60;
                    minutes += days * 60 * 24;

                    if ( safe_to_lock(minutes) ) {
                        if (is_random) {
                            minutes = rnd32() % ( minutes + 1 );
                        }

                        close_lock();
                        sleep_minutes(minutes, minutes < 120 ? 1 : 0);
                        days = 0;
                        hours = 0;
                        minutes = 0;
                        is_random = 0;
                        safe_open_lock();
                    }
                }
            } else {
                is_random = flash_leds();
                pm_sleep(0, 3000);
                minutes = input_count(MINUTE_LED);
                pm_sleep(0, 1000);
                hours = input_count(HOUR_LED);
                pm_sleep(0, 1000);
                days = input_count(DAY_LED);
            }
        }

        pm_sleep(0, 1000);
        rnd();//seed RNG with tick count
    }
}

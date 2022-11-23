#include "../common/cc254x_types.h"
#include "../common/cc254x_map.h"
#include "../common/util.h"
#include "../common/pm_sleep.h"
#include "output.h"
#include "input.h"

void open_lock()
{
    pwm_motor(1,2,15);
}
//register 1 bit 0x02 is the actuator to close the lock
void close_lock()
{
    pwm_motor(2,2,15);
}

void led_on(const uint16 led)
{
    P1DIR |= led;
    P1 |= led;
}

void led_off(const uint16 led)
{
    P1 &= ~led;
}

void blink(const uint16 led, const uint16 count, const bool longblink)
{
    uint16 i = 0;

    for (i = 0; i < count; i++) {
        P1DIR |= led;
        P1 &= ~led;
        pm_sleep(1, longblink ? 300 : 200);
        P1 |= led;
        pm_sleep(1, longblink ? 1000 : 300);
        P1 &= ~led;
    }
}

int flash_leds()
{
    uint16 i = 0;
    int ret = button_pressed();

    for (i = 0; i < 70; i++) {
        ret &= button_pressed();

        switch (i % 6) {
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

        case 4:
            led_on(DAY_LED);
            break;

        case 5:
            led_off(DAY_LED);
            break;
        }

        pm_sleep(1, 100);
    }

    led_off(DAY_LED);
    led_off(HOUR_LED);
    led_off(MINUTE_LED);
    return ret;
}

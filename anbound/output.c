#include "../common/cc254x_types.h"
#include "../common/cc254x_map.h"
#include "../common/util.h"
#include "../common/pm_sleep.h"
#include "output.h"
#include "input.h"

//REGISTER 1  bit 0x1 is the actuator to open the lock.
void open_lock()
{
    P1DIR |= 1;
    P1 |= 1;
    pm_sleep(1, 50);
    P1 &= ~1;
    P1DIR &= ~1;
}

//register 1 bit 0x02 is the actuator to close the lock
void close_lock()
{
    P1DIR |= 2;
    P1 |= 2;
    pm_sleep(1, 50);
    P1 &= ~2;
    P1DIR &= ~2;
}

void led_on(const uint16 led)
{
    P0DIR |= led;
    P0 &= ~led;
}

void led_off(const uint16 led)
{
    P0 |= led;
    P0DIR &= ~led;
}

void blink(const uint16 led, const uint16 count)
{
    uint16 i = 0;

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

int flash_leds()
{
    uint16 i = 0;
    int ret = button_pressed();

    for (i = 0; i < 70; i++) {
        ret &= button_pressed();

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
    return ret;
}


/* sample code to wiggle P1.2 at 1 Hz */

#include "cc254x_types.h"

#include "cc254x_map.h"
#include "pm_sleep.h"
#include "util.h"
#include "time.h"
#define NOOP __asm nop __endasm;


//Don't leave anything dangling after using output or input pins. This wastes power.
//set pin as output/input before actuating it too.
//REGISTER 0 PIN 0X1 contains the lock's shackle state. It's pull-down, so when the shackle is closed the pin is low.
int shackle_closed()
{
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
int button_pressed()
{
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

void blink_blue()
{
    P0DIR |= 0x20;
    P0 |= 0x20;
    pm_sleep(1, 200);
    P0 &= ~0x20;
    pm_sleep(1, 300);
    P0 |= 0x20;
    P0DIR &= ~0X20;
}

void blink_red()
{
    P0DIR |= 0x10;
    P0 |= 0x10;
    pm_sleep(1, 200);
    P0 &= ~0x10;
    pm_sleep(1, 300);
    P0 |= 0x10;
    P0DIR &= ~0x10;
}

void red_on()
{
    P0DIR |= 0x10;
    P0 &= ~0x10;
}

void blue_on()
{
    P0DIR |= 0x20;
    P0 &= ~0x20;
}

void red_off()
{
    P0 |= 0x10;
    P0DIR &= ~0x10;
}

void blue_off()
{
    P0 |= 0x20;
    P0DIR &= ~0x20;
}

int main(void)
{
    int i = 0;
    int n = 0;
    unsigned int minutes = 0;
    unsigned int hours = 0;
    //slow down our device
    slow_clockspeed();
    open_lock();
    blink_blue();
    open_lock();

    while (1)
    {
        if (button_pressed())
        {
            if (shackle_closed())
            {
                open_lock();
                pm_sleep(0, 1000);

                for (i = 0; i < hours; i++)
                    blink_red();

                for (i = 0; i < minutes; i++)
                    blink_blue();

                pm_sleep(0, 2000);

                for (i = 0; i < 70; i++)
                {
                    switch (i % 5)
                    {
                        case 0:
                            red_on();
                            break;

                        case 1:
                            blue_on();
                            break;

                        case 2:
                            red_off();
                            break;

                        case 3:
                            blue_off();
                            break;
                    }

                    pm_sleep(1, 30);
                }

                blue_off();
                red_off();

                if (shackle_closed())
                {
                    close_lock();

                    for (i = 0; i < (minutes * 10); i++)
                    {
                        pm_sleep(0, 59000);
                        blink_blue();
                    }

                    for (n = 0; n < hours; n++)
                    {
                        for (i = 0; i < 60; i++)
                        {
                            pm_sleep(0, 59000);
                            blink_blue();
                        }
                    }

                    minutes = 0;
                    hours = 0;

                    for(i = 0; i < 10; i++){
                      open_lock();
                      pm_sleep(0,10000);
                    }
                }
            }
            else
            {
                red_on();
                blue_on();
                pm_sleep(1, 2000);
                red_off();
                hours = 0;
                minutes = 0;

                for (i = 0; i < 90; i++)
                {
                    if (button_pressed())
                    {
                        blink_blue();
                        blue_on();
                        minutes++;
                    }

                    pm_sleep(1, 100);
                }

                blue_off();
                red_on();

                for (i = 0; i < 90; i++)
                {
                    if (button_pressed())
                    {
                        blink_red();
                        red_on();
                        hours++;
                    }

                    pm_sleep(1, 100);
                }

                red_off();
            }
        }

        pm_sleep(0, 1000);
    }
}

#include "cc254x_types.h"
#include "cc254x_map.h"
#include "input.h"

#define NOOP __asm nop __endasm;
#define POLL_INPUT(REG,REGINP,PIN,OUT)   \
    REGINP |= PIN;\
    REG |= PIN;\
    NOOP;\
    NOOP;\
    NOOP;\
    NOOP;\
    OUT = !(REG & PIN);\
    REG &= ~PIN;\
    REGINP &= ~PIN;\

//Don't leave anything dangling after using output or input pins. This wastes power.
//set pin as output/input before actuating it too.
//REGISTER 0 PIN 0X1 contains the lock's shackle state. It's pull-down, so when the shackle is closed the pin is low.
int shackle_closed()
{
    int ret = 0;
    POLL_INPUT(P0, P0INP, 0x1, ret);
    return ret;
}

//REGISTER 1 BIT 0X10 contains the lock's button state. It's pull-down, so when the button is pressed the pin is low.
int button_pressed()
{
    int ret = 0;
    POLL_INPUT(P1, P1INP, 0x10, ret);
    return ret;
}

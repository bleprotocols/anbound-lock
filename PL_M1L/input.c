#include "../common/cc254x_types.h"
#include "../common/cc254x_map.h"
#include "../common/util.h"

#include "input.h"

#define POLL_INPUT(REG,REGINP,PIN,OUT)   \
    REGINP |= PIN;\
    REG |= PIN;\
    NOOP;\
    OUT = (REG & PIN);\
    REG &= ~PIN;\
    REGINP &= ~PIN;\

//Don't leave anything dangling after using output or input pins. This wastes power.
//set pin as output/input before actuating it too.
//REGISTER 0 PIN 0X1 contains the lock's shackle state. It's pull-down, so when the shackle is closed the pin is low.
int shackle_closed()
{
    int ret = 0;
    POLL_INPUT(P1, P1INP, 0x40, ret);
    return ret == 0;
}

//REGISTER 1 BIT 0X10 contains the lock's button state. It's pull-down, so when the button is pressed the pin is low.
//button 2 is up,
//button 4 is right
//button 8 is left
//button 16 is center
//button 32 is down
int button_pressed()
{
    int ret = 0;
    POLL_INPUT(P0, P0INP, 0x10, ret);
    return ret == 0;
}

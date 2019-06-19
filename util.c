#include "cc254x_types.h"
#include "cc254x_map.h"
#include "util.h"

/* empty interrupt 17 to make sure the interrupt table generated
 * by SDCC is long enough */
void isr_wdt() __interrupt(17) {}


void slow_clockspeed()
{
    OSC32K = 1; //use the internal 32 bit internal clock when in 32khz mode
    OSC = 1; //use the internal 16mhz oscillator NOW
    CLKSPD_0 = 1; //250khz clock
    CLKSPD_1 = 1;
    CLKSPD_2 = 1;
}

uint16 rnd()
{
    RNGCON_0 = 0; //ENABLE rng operation
    RNGCON_1 = 0;
    ADCCON1 |= 0x04; //increment RNG once
    return (((uint16)RNDH) << 8) | RNDL;
}



uint16 read_vdd()
{
    ADCCON3 = 0x3F; //First 4 bits means the reference source is VDD. other two means full 512 steps resolution ( 12 bits )

    while (!(ADCCON1 & 0x80)); //check if last bit contains 1 - if it is 1 then the read is complete

    return ADCL | (((uint16)ADCH) << 8);   //return the read value
}


uint16 read_temperature()
{
    uint16 ret = 0;
    uint8 tr0;
    uint8 atest;
    tr0 = TR0; //save registers
    atest = ATEST;
    ATEST = 1; //enable the temperature sensor
    TR0 = 1; //connect temperature sensor to ADC
    ADCCON3 = 0x3E; //First 3 bits means the reference source is TEMP

    while (!(ADCCON1 & 0x80)); //check if last bit contains 1 - if it is 1 then the read is complete

    ret = ADCL | (((uint16)ADCH) << 8);
    ATEST = atest;
    TR0 = tr0;
    return ret;
}

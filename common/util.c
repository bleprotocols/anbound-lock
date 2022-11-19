#include "cc254x_types.h"
#include "cc254x_map.h"
#include "util.h"

/* empty interrupt 17 to make sure the interrupt table generated
 * by SDCC is long enough */
void isr_wdt() __interrupt(17) {}


void slow_clockspeed()
{
    unsigned long int i = 0;
    //switch to 32mhz crystal to stabilize 32khz xosc
    CLKCON = (CLKCON & 0x80) ;

    while ( (CLKCONSTA & ~0x80) != 0 );

    //switch to 16mhz clock with divider to 250khz
    CLKCON = (CLKCON & 0x80) | 0x49;

    while ((CLKCONSTA & ~0x80) != 0x49 );
}

uint16 rnd()
{
    RNGCON_0 = 0; //ENABLE rng operation
    RNGCON_1 = 0;
    ADCCON1 |= 0x04; //increment RNG once
    return (((uint16)RNDH) << 8) | RNDL;
}


uint32 rnd32()
{
    return ((uint32)rnd()) << 16 | rnd();
}


//retrieve the battery voltage *100
uint16 read_vdd()
{
    uint16 ret = 0;
    ADCCON2 = 0x3F;  //First 4 bits means the reference source is VDD. other two means full 512 steps resolution ( 12 bits )
    ADCCON1 = 0x73;

    while (!(ADCCON1 & 0x80)); //check if last bit contains 1 - if it is 1 then the read is complete

    ret =  ADCL | (((uint16)ADCH) << 8) ;
    ret = (ret * 0.0183195 * 10) / 16;
    return ret;
}

uint16 battery_soc()
{
    uint16 voltage;
    const uint16 volts[] = {300, 290, 280, 270, 260, 250, 240, 200};
    // actual battery capacity  const uint16 caps[]={100,80,60,40,30,20,10,0};
    uint16 caps[] = {100, 78, 59, 37, 26, 13, 0, 0}; //battery capacity if we want our lock motor to work
    int i = 0;
    voltage = read_vdd();

    for (i = 0; i < 8; i++) {
        if ( voltage > volts[i]) {
            if (i == 0) {
                return 100;
            }

            return caps[i] + (  (caps[i - 1] - caps[i] ) * ( voltage - volts [i] ) / (volts[i - 1] - volts[i] ) ) ;
        }
    }

    return 0;
}


uint16 read_temperature()
{
    uint16 ret = 0;
    uint8 tr0 = TR0;//save registers
    uint8 atest = ATEST;
    uint16 adccon3 = ADCCON3;
    ATEST = 1; //enable the temperature sensor
    TR0 = 1; //connect temperature sensor to ADC
    ADCCON3 = 0x3E; //First 3 bits means the reference source is TEMP

    while (!(ADCCON1 & 0x80)); //check if last bit contains 1 - if it is 1 then the read is complete

    ret = ADCL | (((uint16)ADCH) << 8);
    ATEST = atest;
    TR0 = tr0;
    ADCCON3 = adccon3;
    return ret;
}

void pwm_motor(const uint8 pin)
{
    const uint8 pwm_on = 7;
    const uint8 pwm_off = 15;
    const uint32 iter = 100000;
    uint32 i = 0;
    uint8 n = 0;
    //make our clock fast enough for pwm use
    CLKCON = (CLKCON & 0x80) ;

    while ( (CLKCONSTA & ~0x80) != 0 );

    P1DIR |= pin;

    for (i = 0; i < iter; i++) {
        P1 |= pin;

        for (n = 0; n < pwm_on; n++) {
            NOOP;
        }

        P1 &= ~pin;

        for (n = 0; n < pwm_off; n++) {
            NOOP;
        }
    }

    //slow our clock to 250khz again
    CLKCON = (CLKCON & 0x80) | 0x49;

    while ((CLKCONSTA & ~0x80) != 0x49 );
}

#include "cc254x_types.h"
#include "cc254x_map.h"
#include "pm_sleep.h"

#define SLEEP_PM0          0 //PM0, Clock oscillators on, voltage regulator on
#define SLEEP_PM1          1 //PM1, 32.768 kHz oscillators on, voltage regulator on 
#define SLEEP_PM2          2 //PM2, 32.768 kHz oscillators on, voltage regulator off
#define SLEEP_PM3          3 //PM3, All clock oscillators off, voltage regulator off

#define STIE_BV             0x20 //Sleep timer interrupt mask


static void set_pcon()
{
    PCON = 1;
    __asm
    nop
    __endasm;
}

static void set_sleeptimer(uint32 sleepticks)
{
    uint32 current_ticks = 0;
    ((uint8 *) &current_ticks)[0] = ST0;
    ((uint8 *) &current_ticks)[1] = ST1;
    ((uint8 *) &current_ticks)[2] = ST2;
    // set sleep time offset
    current_ticks += sleepticks;
    ST2 = ((uint8 *) &current_ticks)[2];
    ST1 = ((uint8 *) &current_ticks)[1];
    ST0 = ((uint8 *) &current_ticks)[0];
}


void pm_sleep(int drive_pins, uint32 milliseconds)
{
    uint8 ien0, ien1, ien2;
    uint32 sleepticks;
    uint8 p0dir, p1dir, p2dir;
    uint8 p0inp, p1inp, p2inp;
    uint8 p0sel, p1sel, p2sel;
    uint8 p0, p1, p2;
    uint8 clkon;
    
    sleepticks = (milliseconds << 12) /140; //small fudge factor due to our boards xosc being weird.  / 125 is appropriate if it's 32mhz;
    SLEEP &= ~3;         /* clear mode bits */
    SLEEP |= drive_pins ? SLEEP_PM1 : SLEEP_PM2; /* set mode bits   */
    SLEEP |= 0x80; // disable calibration of 32khz timer
    
    while (!(STLOAD & 1));

    set_sleeptimer(sleepticks);
    /* backup interrupt enable registers before sleep */
    ien0  = IEN0;    /* backup IEN0 register */
    ien1  = IEN1;    /* backup IEN1 register */
    ien2  = IEN2;    /* backup IEN2 register */
    clkon = CLKCON; //backup clock state

    if (!drive_pins)
    {
        /* clear I/O registers before sleep */
        p0dir = P0DIR;
        p1dir = P1DIR;
        p2dir = P2DIR;
        p0inp = P0INP;
        p1inp = P1INP;
        p2inp = P2INP;
        p0 = P0;
        p1 = P1;
        p2 = P2;
        p0sel = P0SEL;
        p1sel = P1SEL;
        p2sel = P2SEL;
        P0DIR = 0;
        P1DIR = 0;
        P2DIR = 0;
        P0INP = 0xFF; //set all IO pins to 3-state
        P1INP = 0xFF;
        P2INP = 0xF;
        P0 = 0;
        P1 = 0;
        P2 = 0;
        P0SEL = 0;
        P1SEL = 0;
        P2SEL = 0;
    }

    /* sleep timer interrupt control */
    /* clear sleep interrupt flag */
    STIF = 0;
    /* enable sleep timer interrupt */
    IEN0 = STIE_BV; /* disable IEN0 except STIE */
    IEN1 = 0; /* disable IEN1 except P0IE */
    IEN2 = 0; /* disable IEN2 except P1IE, P2IE */
    EA = 1; //enable interrupts
    
    set_pcon();//go to sleep now
    
    EA = 0; //disable interrupts
    IEN0 = ien0;        /* restore IEN0 register */
    IEN1 = ien1;        /* restore IEN1 register */
    IEN2 = ien2;        /* restore IEN2 register */
    IEN0 &= ~STIE_BV;   /* disable sleep int */
    CLKCON = clkon;

    if (!drive_pins)
    {
        //restore registers
        P0DIR = p0dir;
        P1DIR = p1dir;
        P2DIR = p2dir;
        P0INP = p0inp;
        P1INP = p1inp;
        P2INP = p2inp;
        P0 = p0;
        P1 = p1;
        P2 = p2;
        P0SEL = p0sel;
        P1SEL = p1sel;
        P2SEL = p2sel;
    }

    EA = 1; //enable interrupts
}



ISR(ST, 0)
{
    STIF = 0;
}


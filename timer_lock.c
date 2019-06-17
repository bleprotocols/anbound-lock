/* sample code to wiggle P1.2 at 1 Hz */

#include "cc254x_types.h"

#include "cc254x_map.h"

#include "util.h"

//REGISTER 0 PIN 0X1 contains the lock's shackle state. It's pull-down, so when the shackle is closed the pin is low.
int shackle_closed() {
  P0INP ^= 0X01;
  P0 |= 1;
  delay_ms(5);
  return !(P0 & 1);
}

//REGISTER 1 BIT 0X10 contains the lock's button state. It's pull-down, so when the button is pressed the pin is low.
int button_pressed() {
  P1INP ^= 0X10;
  P1 |= 0X10;
  delay_ms(5);

  return !(P1 & 0x10);
}

//REGISTER 1  bit 0x1 is the actuator to close the lock.
void close_lock() {
  P1 &= ~0x01;
  delay_ms(50);
  P1 |= 0x01;
}

//register 1 bit 0x02 is the actuator to open the lock
void open_lock() {
  P1 &= ~0x02;
  delay_ms(50);
  P1 |= 0x02;
}

void blink_blue() {
  P0 |= 0x20;
  delay_ms(400);
  P0 &= ~0x20;
  delay_ms(600);
  P0 |= 0x20;
}

void blink_red() {
  P0 |= 0x10;
  delay_ms(400);
  P0 &= ~0x10;
  delay_ms(600);
  P0 |= 0x10;
}

void red_on() {
  P0 &= ~0x10;
}

void blue_on() {
  P0 &= ~0x20;
}

void red_off() {
  P0 |= 0x10;
}

void blue_off() {
  P0 |= 0x20;
}

void sleep(unsigned int duration) {
  delay_ms(duration);
}

int main(void) {
  int i=0;
  int n=0;
  unsigned int minutes = 0;
  unsigned int hours = 0;

  init_clock();

  //Enable our led/motor pins as outputs
  P0DIR = 0x20 | 0x10;
  P1DIR = 0x01 | 0x02;

  open_lock();

  while (1) {
    //button pressed
    if (button_pressed()) {
      if (shackle_closed()) {
          
        sleep(2000);
        
        for (i = 0; i < hours; i++) {
          blink_red();
        }

        for (i = 0; i < minutes; i++) {
          blink_blue();
        }

        sleep(2000);

        for (i = 0; i < 50; i++) {
          if (i % 5 == 0) {
            red_on();
          }

          if (i % 5 == 1) {
            blue_on();
          }

          if (i % 5 == 2) {
            red_off();
          }

          if (i % 5 == 3) {
            blue_off();
          }

          sleep(100);
        }

        blue_off();
        red_off();
        if (shackle_closed()) {
          close_lock();
          for (i = 0; i < (minutes * 1); i++) {
            sleep(59000);
            blink_blue();
          }

          for (n = 0; n < hours; n++) {
            for (i = 0; i < 60; i++) {
              sleep(59000);
              blink_blue();
            }
          }

          open_lock();

        }
      } else {
        red_on();
        blue_on();
        sleep(2000);
        red_off();
        hours = 0;
        minutes = 0;
        for (i = 0; i < 50; i++) {
          if (button_pressed()) {
            blink_blue();
            blue_on();
            minutes++;
          }
          sleep(100);
        }
        blue_off();
        red_on();
        for (i = 0; i < 50; i++) {
          if (button_pressed()) {
            blink_red();
            red_on();
            hours++;
          }
          sleep(100);
        }

        red_off();
      }

    }

    delay_ms(200);
  }
}

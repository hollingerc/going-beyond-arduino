/*
 * project: Blink
 * file: blink.c
 *
 * Created: 2016-08-05
 * Author : Craig Hollinger
 *
 * Source code for blinking the LED connected to Pin 13 (PORTB5) found on most
 * Arduino boards.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
*/ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB = 0b00100000;
    /* Replace with your application code */
    while (1) 
    {
      PORTB = 0b00100000;
      _delay_ms(250);
      PORTB = 0b00000000;
      _delay_ms(250);
      
    }
}


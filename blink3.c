/*
 * project: Blink3
 * file: blink3.c
 *
 * Created: 2016-08-14
 * Author : Craig Hollinger
 *
 * Source code for blinking six LEDs connected to IO pins PORTD2, PORTD3 and
 * PORTD4.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
*/ 

#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY 500

int main(void){

  DDRD |= _BV(PORTD2) | _BV(PORTD3) | _BV(PORTD4);
  PORTD &= ~_BV(PORTD2) & ~_BV(PORTD3) & ~_BV(PORTD4);
  
  while (1){
    DDRD &= ~_BV(PORTD2);
    DDRD |= _BV(PORTD3);
    PORTD &= ~_BV(PORTD3);
    PORTD |= _BV(PORTD4);
    
    _delay_ms(BLINK_DELAY);
    
    PORTD |= _BV(PORTD3);
    PORTD &= ~_BV(PORTD4);
    
    _delay_ms(BLINK_DELAY);

    DDRD &= ~_BV(PORTD4);
    DDRD |= _BV(PORTD2);
    PORTD &= ~_BV(PORTD2);
    PORTD |= _BV(PORTD3);
    
    _delay_ms(BLINK_DELAY);

    PORTD |= _BV(PORTD2);
    PORTD &= ~_BV(PORTD3);
    
    _delay_ms(BLINK_DELAY);

    DDRD &= ~_BV(PORTD3);
    DDRD |= _BV(PORTD4);
    PORTD &= ~_BV(PORTD2);
    PORTD |= _BV(PORTD4);
    
    _delay_ms(BLINK_DELAY);
    
    PORTD &= ~_BV(PORTD4);
    PORTD |= _BV(PORTD2);
    
    _delay_ms(BLINK_DELAY);
    
    PORTD &= ~_BV(PORTD2);

    _delay_ms(BLINK_DELAY);
  }
}


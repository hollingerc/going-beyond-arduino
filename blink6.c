/*
 * project: Blink6
 * file: blink6.c
 *
 * Created: 2016-08-14
 * Author : Craig Hollinger
 *
 * Source code for blinking six LEDs connected to IO pins PORTD2, PORTD3 and
 * PORTD4.  Refer to the schematic "Blink6 schematic.pdf".
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
*/ 

#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY 500 /* delay (ms) between changes in LED pattern */

int main(void){

  /* make LED pins output by writing 1s to Data Direction Register */
  DDRD |= _BV(PORTD2) | _BV(PORTD3) | _BV(PORTD4);
  /* ensure all LED pins are low */
  PORTD &= ~_BV(PORTD2) & ~_BV(PORTD3) & ~_BV(PORTD4);

/* The LED numbers refer to the schematic "Blink6 schematic.pdf" on my GitHub
   site. */
  while (1){
    /* turn on LED D2 */
    DDRD &= ~_BV(PORTD2); /* pin PD2 input (high impedance) */
    DDRD |= _BV(PORTD3); /* pin PD3 output */
    PORTD &= ~_BV(PORTD3); /* drive PD3 low */
    PORTD |= _BV(PORTD4); /* drive PD4 high */
    
    _delay_ms(BLINK_DELAY);
    
    /* turn off LED D2, and turn on LED D1 */
    PORTD |= _BV(PORTD3); /* drive PD3 high */
    PORTD &= ~_BV(PORTD4); /* drive PD4 low */
    
    _delay_ms(BLINK_DELAY);

    /* turn off LED D1, and turn on LED D6 */
    DDRD &= ~_BV(PORTD4); /* pin PD4 input (high impedance) */
    DDRD |= _BV(PORTD2); /* pin PD2 output */
    PORTD &= ~_BV(PORTD2); /* drive PD2 low */
    PORTD |= _BV(PORTD3); /* drive PD3 high */
    
    _delay_ms(BLINK_DELAY);

    /* turn off LED D6, and turn on LED D5 */
    PORTD |= _BV(PORTD2); /* drive PD2 high */
    PORTD &= ~_BV(PORTD3); /* drive PD3 low */
    
    _delay_ms(BLINK_DELAY);

    /* turn off LED D5, and turn on LED D4 */
    DDRD &= ~_BV(PORTD3); /* pin PD3 input (high impedance) */
    DDRD |= _BV(PORTD4); /* pin PD4 output */
    PORTD &= ~_BV(PORTD2); /* drive PD2 low */
    PORTD |= _BV(PORTD4); /* drive PD4 high */
    
    _delay_ms(BLINK_DELAY);
    
    /* turn off LED D4, and turn on LED D3 */
    PORTD &= ~_BV(PORTD4); /* drive PD4 low */
    PORTD |= _BV(PORTD2); /* drive PD2 high */
    
    _delay_ms(BLINK_DELAY);
    
    /* turn off LED D3, all LEDs are off */
    PORTD &= ~_BV(PORTD2); /* drive PD2 low */

    _delay_ms(BLINK_DELAY);
  }
}


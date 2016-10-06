/*
 * FastPWM.c
 *
 * Created: 2016-10-04
 * Author : Craig Hollinger
 *
 * Source code for producing a PWM signal.  We'll use the Fast PWM Mode of
 * Timer/Counter 0.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/* This is the number of 10ms delays to give us a slowly changing PWM. 
 * 50 was used for the oscilloscope display, but this was too slow for
 * visualizing the LEDs so 5 was used in this case. */
//#define PWM_CHANGE_TIME 50
#define PWM_CHANGE_TIME 5

/* This variable counts the number of 10ms delays. */
uint8_t pwmDelay = PWM_CHANGE_TIME;

/* A flag that is set every time Timer 2 times out (10ms).  Make it volatile
   so the compiler won't optimize it out and it will be visible in the main()
   function. */
volatile uint8_t t210msFlag;

/* This is the Timer 2 Compare A interrupt service routine.  Runs every 10ms
   when the timer times out.  All it does is sets a flag.
   - Important to keep it simple! */
ISR(TIMER2_COMPA_vect)
{
  t210msFlag = 1;
}

/* This is where it all happens. */
int main(void)
{
  /* Set up the IO port registers for the IO pins connected to the PWM output
   * pin OC0A (PD6) and OC0B (PD5). */
  DDRD |= (_BV(PORTD6) | _BV(PORTD5));
  PORTD &= (~_BV(PORTD6) & ~_BV(PORTD5));

  /* Set up Timer 2 to generate a 10ms interrupt:
     - clocked by F_CPU / 1024
     - generate interrupt when OCR2A matches TCNT2
     - load OCR2A with the delay count to get 10ms delay
   */
  TCCR2A = _BV(WGM21); /* = 0b00000010, TC2 mode 2, CTC - clear timer on match A */
  TCCR2B = _BV(CS22) | _BV(CS20); /* = 0b00000101, clock by F_CPU / 1024 */
  OCR2A = (F_CPU / 1024 / 100 - 1); /* = 155 */
  TIMSK2 = _BV(OCIE2A); /* = 0b00000010, enable OCIE2A, match A interrupt */

  /* Set up Timer 0 to generate a Fast PWM signal:
     - clocked by F_CPU (fastest PWM frequency)
     - non-inverted output
   */
//  TCCR0A = (_BV(WGM00)| _BV(COM0A1) | _BV(COM0B1)); /* = 0b10100001, TC0 Mode 1, Phase Correct PWM */
  TCCR0A = (_BV(WGM00) | _BV(WGM01) | _BV(COM0A1) | _BV(COM0B1)); /* = 0b10100011, TC0 Mode 3, Fast PWM */
  TCCR0B = _BV(CS00); /* TC0 clocked by F_CPU, no prescale */
  OCR0A = 0; /* start with duty cycle = 0% */
  OCR0B = 255; /* start with duty cycle = 100% */

  /* enable the interrupt system */
  sei();

  /* run around this loop for ever */
  while (1) 
  {
    if(t210msFlag == 1) /* test the 10ms flag, is it set? */
    {
      t210msFlag = 0; /* reset the 10ms flag */

      if(--pwmDelay == 0) /* decrement and test the delay timer*/
      {
        pwmDelay = PWM_CHANGE_TIME; /* reset the delay timer */
        ++OCR0A; /* change the duty cycle */
        --OCR0B; /* change the duty cycle */

      }/* end if(--pwmDelay == 0) */

    }/* end if(t210msFlag == 1) */

  }/* end while(1) */

}/* end main() */


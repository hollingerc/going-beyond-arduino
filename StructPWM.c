/*
 * StructPWM.c
 *
 * Created: 2016-10-14
 * Author : Craig Hollinger
 *
 * Source code for producing a PWM signal.  We'll use the Fast PWM Mode of
 * Timer/Counter 0.
 *
 * This program uses the struct, union and typedef code for an alternate way of
 * accessing the control registers of Timer 0 and Timer 2.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer/tc0.h>
#include <timer/tc2.h>

/* This is the number of 10ms delays to give us a slowly changing PWM. 
 * 50 was used for the oscilloscope display, but this was too slow for
 * visualizing the LEDs so 1 was used in this case. */
//#define PWM_CHANGE_TIME 50
#define PWM_CHANGE_TIME 1

/* This variable counts the number of 10ms delays. */
uint8_t pwmDelay = PWM_CHANGE_TIME;

/* A flag that is set every time Timer 2 times out (10ms).  Make it volatile
   so the compiler won't optimize it out and it will be visible in the main()
   function. */
volatile uint8_t t210msFlag;

/* These are temporary data structures to hold the information for programming
   Timer 0 and Timer 2. */
Timer_Counter0 timer0;
Timer_Counter2 timer2;

/* This is the Timer 2 Compare A interrupt service routine.  Runs every 10ms
   when the timer times out.  All it does is sets a flag.
   - Important to keep it short and simple! */
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
  tc2_get_config(timer2);
  timer2.tccr2a.wgm2l = TC2_TCCR2A_M2_CTC; /* TC2 Mode 2, Clear timer on match */
  timer2.tccr2a.com2a = TC2_TCCR2A_OC2A_MODE0; /* OC0A pin disabled */
  timer2.tccr2a.com2b = TC2_TCCR2A_OC2B_MODE0; /* OC0B pin disabled */
  timer2.tccr2b.cs2 = TC2_TCCR2B_CLK_PRSC1024; /* TC0 clocked by F_CPU/1024 */
  timer2.tccr2b.wgm2h = 0; /* upper bit of TC0 Mode */
  timer2.ocr2a = (F_CPU / 1024 / 100 - 1);
  timer2.timsk2.ocie2a = 1; /* enable OCIE2A, match A interrupt */
  tc2_set_config(timer2);

  /* Set up Timer 0 to generate a Fast PWM signal:
     - clocked by F_CPU (fastest PWM frequency)
     - non-inverted output
   */
  tc0_get_config(timer0);
//  timer0.tccr0a.wgm0l = TC0_TCCR0A_M1_PWM_FF; /* TC0 Mode 1, Phase Correct PWM */
  timer0.tccr0a.wgm0l = TC0_TCCR0A_M3_FASTPWM_FF; /* TC0 Mode 3, Fast PWM */
  timer0.tccr0a.com0a = TC0_TCCR0A_OC0A_MODE2; /* clear OC0A on match */
  timer0.tccr0a.com0b = TC0_TCCR0A_OC0B_MODE2; /* clear OC0B on match */
  timer0.tccr0b.cs0 = TC0_TCCR0B_CLK_PRSC1; /* TC0 clocked by F_CPU, no prescale */
  timer0.tccr0b.wgm0h = 0; /* upper bit of TC0 Mode */
  timer0.ocr0a = 0; /* start with duty cycle = 0% */
  timer0.ocr0b = 255; /* start with duty cycle = 100% */
  tc0_set_config(timer0);

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


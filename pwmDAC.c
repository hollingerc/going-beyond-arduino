/*
 * pwmDAC.c
 *
 * Created: 2016-11-12
 * Author : Craig Hollinger
 *
 * Source code for producing a PWM generated sine wave signal.  We'll use the 
 * Fast PWM Mode of Timer/Counter 0.  
 *
 * Note:
 * This is a VERY simple way of generating an analog, repetitive signal (in this
 * case a sine wave, but could be any arbitrary waveform).  The main limitation
 * is the frequency of the signal cannot be changed.
 *
 * The frequency of the produced sine wave is fixed at about 244Hz which is
 * 62500, the PWM frequency divided by 256, the number of points in the sine
 * table.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer/tc0.h>

/* This is the sine table.  Each value represents a voltage level on a
   sinusoidal waveform.  A spreadsheet program such as Excel can be used to
   generate these numbers.

   This uses up a lot of RAM because this is where it is stored.  Could be moved
   to FLASH, but would require more overhead to get the data out. */
uint8_t SINE_TABLE[]=
{
  128, 131, 134, 137, 140, 143, 146, 149, 152, 156, 159, 162, 165, 168, 171, 174,
  176, 179, 182, 185, 188, 191, 193, 196, 199, 201, 204, 206, 209, 211, 213, 216,
  218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 237, 239, 240, 242, 243, 245,
  246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 254, 254, 253, 252, 252, 251, 250, 249, 248, 247,
  246, 245, 243, 242, 240, 239, 237, 236, 234, 232, 230, 228, 226, 224, 222, 220,
  218, 216, 213, 211, 209, 206, 204, 201, 199, 196, 193, 191, 188, 185, 182, 179,
  176, 174, 171, 168, 165, 162, 159, 156, 152, 149, 146, 143, 140, 137, 134, 131,
  127, 124, 121, 118, 115, 112, 109, 106, 103, 99, 96, 93, 90, 87, 84, 81,
  79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39,
  37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10,
  9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8,
  9, 10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35,
  37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76,
  79, 81, 84, 87, 90, 93, 96, 99, 103, 106, 109, 112, 115, 118, 121, 124
};

/* This is the Timer 0 Compare A interrupt service routine.  Runs every time
   the counter (TCNT0) matches the output compare register A (OCR0A).  Here a
   new value is read from SINE_TABLE and written the output compare register. */
ISR(TIMER0_COMPA_vect)
{
  /* This variable points to a value in the SINE_TABLE.  Made static so it is
     remembered between interrupts. */
  static uint8_t i;

  /* update the output compare register */
  OCR0A = SINE_TABLE[i++];

}/* end ISR(TIMER0_COMPA_vect) */

/* This is where it all happens. */
int main(void)
{
  /* This data structure holds the information for programming Timer 0. */
  Timer_Counter0 timer0;

  /* Set up the IO port registers for the IO pin connected to the PWM output
     pin OC0A (PD6). */
  DDRD |= (_BV(PORTD6));
  PORTD &= (~_BV(PORTD6));

  /* Set up Timer 0 to generate a Fast PWM signal:
     - clocked by F_CPU (fastest PWM frequency)
     - non-inverted output */
  tc0_get_config(timer0);
  timer0.tccr0a.wgm0l = TC0_TCCR0A_M3_FASTPWM_FF; /* TC0 Mode 3, Fast PWM */
  timer0.tccr0a.com0a = TC0_TCCR0A_OC0A_MODE2; /* clear OC0A on match */
  timer0.tccr0a.com0b = TC0_TCCR0A_OC0B_MODE0; /* OC0B disabled */
  timer0.tccr0b.cs0 = TC0_TCCR0B_CLK_PRSC1; /* TC0 clocked by F_CPU, no prescale */
  timer0.tccr0b.wgm0h = 0; /* upper bit of TC0 Mode */
  timer0.ocr0a = 0; /* start with duty cycle = 0% */
  timer0.timsk0.ocie0a = 1; /* enable OCIE0A, match A interrupt */
   tc0_set_config(timer0);

  /* enable the interrupt system */
  sei();

  /* run around this loop for ever */
  while (1) 
  {
    /* Nothing happens here, all the work is done in the ISR */
  }/* end while(1) */

}/* end main() */


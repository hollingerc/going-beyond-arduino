/*
 * BlinkWithTC0.c
 *
 * Created: 2016-09-05
 * Author : Craig Hollinger
 *
 * Source code for blinking six LEDs connected to IO pins PORTD2, PORTD3 and
 * PORTD4.
 *
 * - implement a 10ms delay with Timer 0 to use as the blinking delay
 * - use an interrupt service function to deal with the timer
 * - implement a state machine to keep track of which LED to turn on
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/* This is the number of 10ms delays to give us the desired LED blink time. */
#define LED_BLINK_TIME 50

/* These constants are the different states of the blinking LED state machine.
 */
enum
{
  ledState0,
  ledState1,
  ledState2,
  ledState3,
  ledState4,
  ledState5,
  ledState6,
  maxLedState /* this state is used to check for the end of the state machine */
};

/* This variable holds the state of the blinking LEDs state machine. */
uint8_t ledState,
/* This variable counts the number of 10ms delays */
        ledDelay = LED_BLINK_TIME;

/* A flag that is set every time Timer 0 times out (10ms).  Make it volatile
   so the compiler won't optimize it out and it will be visible in the main()
   function. */
volatile uint8_t t010msFlag;

/* This is the Timer 0 Compare A interrupt service routine.  Runs every 10ms
   when the timer times out.  All it does is sets a flag.
   - Important to keep it simple! */
ISR(TIMER0_COMPA_vect)
{
  t010msFlag = 1;
}

/* This is where it all happens. */
int main(void)
{
  /* Set up the IO port registers for the IO pins connected to the LEDs. */
  DDRD |= _BV(PORTD2) | _BV(PORTD3) | _BV(PORTD4); /* all are outputs */
  PORTD &= ~_BV(PORTD2) & ~_BV(PORTD3) & ~_BV(PORTD4); /* all are low */

  /* Set up Timer 0:
     - clocked by F_CPU / 1024
     - generate interrupt when OCR0A matches TCNT0
     - load OCR0A with the delay count to get 10ms delay
   */
  TCCR0A = 0b00000010; /* TC0 mode 2, CTC - clear timer on match A */
  TCCR0B = 0b00000101; /* clock by F_CPU / 1024 */
  OCR0A = 155; /* = F_CPU / 1024 / 100 - 1 */
  TIMSK0 = 0b00000010; /* enable OCIE0, match A interrupt */

  /* enable the interrupt system */
  sei();

  /* run around this loop for ever */
  while (1)
  {
    if(t010msFlag == 1) /* test the 10ms flag, is it set? */
    {
      t010msFlag = 0; /* reset the flag */
      
      if(ledDelay-- == 0) /* count another 10ms, time up yet? */
      {
        ledDelay = LED_BLINK_TIME; /* reset the delay counter */
        
        if(++ledState == maxLedState) /* advance to the next state */
        {
          ledState = ledState0; /* reset the state to the first state */
        }
        
        /* test ledState and do what we need to do */
        switch(ledState)
        {
          case ledState0:
            DDRD &= ~_BV(PORTD2);
            DDRD |= _BV(PORTD3);
            PORTD &= ~_BV(PORTD3);
            PORTD |= _BV(PORTD4);
            break;

          case ledState1:
            PORTD |= _BV(PORTD3);
            PORTD &= ~_BV(PORTD4);
            break;

          case ledState2:
            DDRD &= ~_BV(PORTD4);
            DDRD |= _BV(PORTD2);
            PORTD &= ~_BV(PORTD2);
            PORTD |= _BV(PORTD3);
            break;        

          case ledState3:
            PORTD |= _BV(PORTD2);
            PORTD &= ~_BV(PORTD3);
            break;

          case ledState4:
            DDRD &= ~_BV(PORTD3);
            DDRD |= _BV(PORTD4);
            PORTD &= ~_BV(PORTD2);
            PORTD |= _BV(PORTD4);
            break;

          case ledState5:        
            PORTD &= ~_BV(PORTD4);
            PORTD |= _BV(PORTD2);
            break;

          case ledState6:        
            PORTD &= ~_BV(PORTD2);
            break;

          default:
            break;

        }/* end switch(ledState) */
        
      }/* end if(ledDelay-- == 0) */
      
    }/* end if(t010msFlag == 1) */
    
  }/* end while(1) */
  
}/* end main() */


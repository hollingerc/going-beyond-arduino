/*
 * variables.c
 *
 * Created: 2018-04-02
 * Author : Craig Hollinger
 *
 * Source code for the video 'Going Beyond Arduino #15: Variables'.
 *
 * This code demonstrates the declaration and use of variables.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */ 

#include <avr/io.h>
#include <stdlib.h> /* for ltoa() and sizeof() */
#include "uart/uart.h"

/* Function prototypes */
void initialize(void);
void lengths(void);
void print_int(int arg);

/* Variables */
char tempStr[10];/* string to hold variables converted to ASCII */
int global_a = 765, global_b = 4357;
char c;
short d;
long e;
float f;
double g;

/* add()
 *
 * This function adds the two arguments and returns the result.
 *
 * - a, b: the variables to add
 */
int add(int x, int y)
{
/* a local variable to temporarily store the result */
  static int result;

/* do the addition */
  result = x + y;

/* exit the function and return the result */
  return(result);

}/* end add() */

/* Start of code */
int main(void)
{
  int result;
  int local_a = 24, local_b = 125;

  initialize();
  lengths();

/* literals as arguments */
  result = add(24, 125);
  print_int(result);

/* initialized local variables as arguments */
  result = add(local_a, local_b);
  print_int(result);

/* re-assign values to local variables, then use as arguments */
  local_a = 62;
  local_b = 12;
  result = add(local_a, local_b);
  print_int(result);

/* initialized global variables as arguments */
  result = add(global_a, global_b);
  print_int(result);

/* re-assign values to global variables, then use as arguments */
  global_a = 325;
  global_b = 1234;
  result = add(global_a, global_b);
  print_int(result);

/* wait here forever */
  while (1) 
  {
  }/* end while() */

}/* end main() */

/* initialize()
 *
 * This function initializes the system.
 */
void initialize(void)
{
  uart_init(115200, USART_CHAR_SZ_EIGHT, USART_PARITY_NONE, USART_STOP_BIT_ONE);

}/* end initialize() */

/* lengths()
 *
 * This function displays the length in bytes of various variables.
 */
void lengths(void)
{
  ltoa(sizeof(c), tempStr, 10);
  uart_putstr("Length of char: ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

  ltoa(sizeof(global_b), tempStr, 10);
  uart_putstr("Length of int: ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

  ltoa(sizeof(d), tempStr, 10);
  uart_putstr("Length of short: ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

  ltoa(sizeof(e), tempStr, 10);
  uart_putstr("Length of long: ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

  ltoa(sizeof(f), tempStr, 10);
  uart_putstr("Length of float: ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

  ltoa(sizeof(g), tempStr, 10);
  uart_putstr("Length of double: ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

}/* end lengths() */

/* print_int()
 *
 * This function converts the int argument to an ASCII string and sends the
 * string to the serial port.
 */
void print_int(int arg)
{
  char str[10];

  ltoa(arg, str, 10);
  uart_putstr(str);
  uart_putstr("\r\n");

}/* end print_int() */

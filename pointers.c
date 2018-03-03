/*
 * pointers.c
 *
 * Created: 2018-02-28
 * Author : Craig Hollinger
 *
 * Source code for the video 'Going Beyond Arduino #14: Pointers'.
 *
 * This code demonstrates the declaration and use of pointers.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */ 

#include <avr/io.h>
#include <stdlib.h>/* for itoa() */
#include "uart/uart.h"

/* Function prototypes */
void swap(int *first, int *second);

/* Variables */
char tempStr[10];/* string to hold variables converted to ASCII */
int variable_1, variable_2;/* two integer variables declared in the normal way */
int * variable_P;/* this is a pointer (points to integer variables) */

/* Start of code */
int main(void)
{
/* initialize the UART so we can use it to see what's going on */
  uart_init(115200, USART_CHAR_SZ_EIGHT, USART_PARITY_NONE, USART_STOP_BIT_ONE);

/* display the RAM addresses of the two variables, in HEX */
  uart_putstr("Address of variable_1: 0x");
  itoa((uint16_t)&variable_1, tempStr, 16);
  uart_putstr(tempStr);
  uart_putstr("\r\n");
  uart_putstr("Address of variable_2: 0x");
  itoa((uint16_t)&variable_2, tempStr, 16);
  uart_putstr(tempStr);
  uart_putstr("\r\n");
  
/* display the address of the pointer in HEX (where it is located in RAM not
   what it points to) */
  uart_putstr("Address of *variable_P: 0x");
  itoa((uint16_t)&variable_P, tempStr, 16);
  uart_putstr(tempStr);
  uart_putstr("\r\n");

/* assign some values to the variables */
  variable_1 = 1234;
  variable_2 = 5678;

/* display the contents of the two variables before swap() */
  uart_putstr("Before swap():\r\n");
  itoa(variable_1, tempStr, 10);
  uart_putstr("variable_1 = ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");
  itoa(variable_2, tempStr, 10);
  uart_putstr("variable_2 = ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

/* perform the swap */
  swap(&variable_1, &variable_2);

/* display the contents of the two variables after swap() */
  uart_putstr("After swap():\r\n");
  itoa(variable_1, tempStr, 10);
  uart_putstr("variable_1 = ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");
  itoa(variable_2, tempStr, 10);
  uart_putstr("variable_2 = ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

/* assign the address of variable_1 to the pointer variable_P */
  variable_P = &variable_1;

/* display the contents of the pointer, and the contents of the variable it
   points to */
  uart_putstr("Contents of *variable_P (address of variable_1): 0x");
  itoa((uint16_t)variable_P, tempStr, 16);
  uart_putstr(tempStr);
  uart_putstr("\r\n");
  uart_putstr("Contents of location *variable_P points to (variable_1): ");
  itoa(*variable_P, tempStr, 10);
  uart_putstr(tempStr);
  uart_putstr("\r\n");

/* perform the swap using the pointer in place of variable_1 */
  swap(variable_P, &variable_2);
  
/* display the contents of the variables after second swap() */
  uart_putstr("After second swap():\r\n");
  itoa(variable_1, tempStr, 10);
  uart_putstr("variable_1 = ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");
  itoa(variable_2, tempStr, 10);
  uart_putstr("variable_2 = ");
  uart_putstr(tempStr);
  uart_putstr("\r\n");

/* wait here forever */
  while (1) 
  {
  }/* end while() */

}/* end main() */

/* swap()
 *
 * This function swaps the two variables.
 *
 * - first, second: addresses of the variables to swap
 */
void swap(int *first, int *second)
{
/* a local variable to temporarily store one of the inputs */
  int temp;

/* temporarily store the first number */
  temp = *first;
/* put the second number into the first variable */
  *first = *second;
/* move the stored first number into the second variable */
  *second = temp;

}/* end swap() */



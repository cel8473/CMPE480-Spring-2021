/*
 * File:        uart.h
 * Purpose:     Provide UART  definitions
 *
 * Notes:		
 *
 */

 #ifndef UART_H
 #define UART_H
 
 #include "MK64F12.h"
 
 //void uart_init(void);
 void uart0_init(void);
 void uart3_init(void);
 
 
 uint8_t uart0_getchar(void);
 void uart0_putchar(char ch);
 void uart0_put(char *ptr_str);
 
 uint8_t uart3_getchar(void);
 void uart3_putchar(char ch);
 void uart3_put(char *ptr_str);
 
 #endif
 
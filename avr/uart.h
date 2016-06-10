/*
 *    uart.h - AVR serial library
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

#ifndef UART_H
#define UART_H

void uart_init(void);
char uart_getchar(void);
void uart_putchar(char c);
void uart_puts(char *str);
char *itoa(int32_t);

#endif /* UART_H */

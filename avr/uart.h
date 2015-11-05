/*
 *    uart.h - AVR serial library
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

void uart_init(void);
void uart_send(int64_t num);
void uart_lb(void);

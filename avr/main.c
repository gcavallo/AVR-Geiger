/*
 *    AVR-Geiger
 *    https://github.com/gcavallo/AVR-Geiger
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

#include <avr/io.h>           // pin name definitions
#include <avr/interrupt.h>    // interrupts

#include "main.h"
#include "time.h"
#include "uart.h"

ISR(INT0_vect) {
	if (cpm < UINT16_MAX)
		cpm++;
}

int main(void) {
	uart_init();        // initialize USART
	cli();              // disable global interrupts
	timer1_init();      // initialize TIMER1
	DDRD &= ~_BV(0);    // reset INT0 to input mode
	PORTD |= _BV(0);    // set INT0 to high
	sei();              // enable global interrupts
	cpm = 0;            // reset cpm counter

	while(1) {
		if (seconds >= 60) {
			cli();                   // disable global interrupts
			uart_puts(itoa(cpm));    // print cpm number
			cpm = 0;                 // reset cpm counter
			seconds = 0;             // reset seconds counter
			sei();                   // enable global interrupts
		}
	}
}

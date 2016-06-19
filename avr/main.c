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
#include "serial.h"

ISR(INT0_vect) {
	if (cpm < UINT16_MAX)
		cpm++;
}

int main(void) {
	cli();                   // disable global interrupts
	uart_init();             // initialize USART
	timer1_init();           // initialize TIMER1
	DDRD &= ~_BV(DDD2);      // clear INT0 pin to input mode
	PORTD |= _BV(PORTD2);    // set INT0 pin to high
	EICRA |= _BV(ISC01);     // set INT0 to falling edge
	EIMSK |= _BV(INT0);      // enable external interupt on INT0
	cpm = 0;                 // reset cpm counter
	seconds = 0;             // reset seconds counter
	sei();                   // enable global interrupts

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

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
#include "uart.h"

uint16_t seconds = 0;

void timer1_init(void) {
	OCR1A = 0x3D08;                     // (clock / prescaler) * seconds) - 1
	TCCR1B |= _BV(WGM12);               // set TIMER1 to CTC mode
	TIMSK1 |= _BV(OCIE1A);              // set interrupt on CTC match
	TCCR1B |= _BV(CS12) | _BV(CS10);    // start TIMER1 with 1024 prescaler
}

ISR (TIMER1_COMPA_vect) {
	seconds++;
}

volatile uint16_t cpm = 0;

ISR(INT0_vect) {
	cpm++;
}

int main(void) {
	uart_init();        // initialize USART
	cli();              // disable global interrupts
	timer1_init();      // initialize TIMER1
	DDRD &= ~_BV(0);    // reset INT0 to input mode
	PORTD |= _BV(0);    // set INT0 to high
	sei();              // enable global interrupts

	while(1) {
		if (seconds >= 60) {
			cli();
			uart_send(cpm);    // print cpm number
			uart_lb();         // print linebreak
			cpm = 0;           // reset cpm counter
			seconds = 0;       // reset seconds counter
			sei();
		}
	}
}

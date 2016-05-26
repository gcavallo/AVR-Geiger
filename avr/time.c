/*
 *    time.c - AVR timer library
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "time.h"

void timer1_init(void) {
	OCR1A = 0x3D08;                     // 16 MHz / 1024 prescaler * 1 sec - 1
	TCCR1B |= _BV(WGM12);               // set TIMER1 to CTC mode
	TIMSK1 |= _BV(OCIE1A);              // set interrupt on CTC match
	TCCR1B |= _BV(CS12) | _BV(CS10);    // start TIMER1 with 1024 prescaler
	seconds = 0;                        // reset TIMER1 to 0
}

ISR(TIMER1_COMPA_vect) {    // define TIMER1 interrupt
	seconds++;
}

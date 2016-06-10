/*
 *    time.h - AVR timer library
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

#ifndef TIME_H
#define TIME_H

volatile uint16_t seconds;

void timer1_init(void);

#endif /* TIME_H */

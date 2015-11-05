/*
 *    AVR-Geiger
 *    https://github.com/gcavallo/AVR-Geiger
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

#define F_CPU 16000000UL
#define BAUD 9600

volatile uint16_t cpm;

int main(void);

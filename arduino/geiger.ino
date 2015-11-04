/*
 *    AVR-Geiger
 *    https://github.com/gcavallo/AVR-Geiger
 *
 *    Copyright (c) 2015, Gabriel Cavallo
 *    GPLv3 License https://gnu.org/licenses/gpl.txt
 */

#define TIME   60000
#define BAUD   9600
#define PIN    2
#define INT    0

volatile unsigned long cpm;
unsigned long past;
unsigned long now;

void pulse() {
	cpm++;
}

void setup() {
	Serial.begin(BAUD);
	pinMode(PIN, INPUT_PULLUP);
	attachInterrupt(INT, pulse, FALLING);
	past = millis();
	cpm = 0;
}

void loop() {
	now = millis();
	if (now - past >= TIME) {
		Serial.println(cpm);
		cpm = 0;
		past = now;
	}
}

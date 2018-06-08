#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"
#include "utils.h"


ISR(USART_RX_vect) {
	char byte = UDR0;
	if(byte == '1') {
		PORTB |= (1<<PB0);
	}
	else if(byte == '2') {
		PORTB &= ~(1<<PB0);
	}
}

static inline void initTimer(void) {
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	TCCR1B |= (1<<CS11);
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1);

	ICR1 = 2500;
}

int main(void) {
	DDRB = 0xff;
	initUSART();
	initTimer();
	sei();
	printString("Welcome!\r\n");
	int value1 = 80;
	int direction1 = 1;

	int value2 = 340;
	int direction2 = -1;
	while(1) {
		OCR1A = value1;
		OCR1B = value2;
		_delay_ms(2);
		value1 += direction1;
		value2 += direction2;
		if(value1 == 80) {
			direction1 = 1;
		}
		if(value1 == 340) {
			direction1 = -1;
		}

		if(value2 == 80) {
			direction2 = 1;
		}
		if(value2 == 340) {
			direction2 = -1;
		}
	}
	return 0;
}


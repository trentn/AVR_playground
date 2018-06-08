#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NUM_STATES 3

volatile int state;

int debounce(void) {
	if(bit_is_clear(PIND, PD2)) {
		_delay_ms(1);
		if(bit_is_clear(PIND, PD2)) {
			return 0;
		}
	}
	return 1;
}

ISR(INT0_vect){
	state = (state+1)%NUM_STATES;
}

void initInterrupt0(void) {
	EIMSK |= (1 << INT0);
	EICRA |= (2 << ISC00);
	sei();
}

void initInterruptPC18(void) {
	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT18);
	sei();
}


void fun(void) {
	for(int i = 0; i < 8; i++) {
		PORTB = (1<<i);
		_delay_ms(100);
	}
	for(int i = 6; i > 0; i--) {
		PORTB = (1<<i);
		_delay_ms(100);
	}
}

int main(void) {
	DDRB |= 0xff;
	PORTD |= (1<<PD2);
	initInterrupt0();

	state = 0;
	
	while(1) {
		switch(state) {
			case 0: PORTB = 0;
				break;
			case 1: PORTB = 0xff;
				break;
			case 2: fun();
				break;
			default:
				break;
		}
	}
	return 0;
}

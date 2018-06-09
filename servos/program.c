#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"
#include "servo.h"

char bytes[10] = {0};
int bytes_index = 0;

int pwm_counter = 0;
char servo_select = 0;

ISR(USART_RX_vect) {
	char byte = UDR0;
	transmitByte(byte);
	if (byte == '\r') {
		transmitByte('\n');
		bytes[bytes_index] = 0;
		if(bytes[0] == 's') {
			servo_select = atoi(bytes+1);
		} else {
			setServoAngle(servo_select, atoi(bytes));
		}
		bytes_index = 0;
	} else {
		bytes[bytes_index] = byte;
		bytes_index = (bytes_index + 1)%10;
	}
}


ISR(TIMER1_COMPA_vect) {
	pwm_counter = (pwm_counter + 1)%250;

	pinControl(pwm_counter);
}

void clear_clkpr (void) {
	CLKPR = 0x80;
	CLKPR = 0;
}

int main(void) {
	
	clear_clkpr(); //set clock to 8MHz
	initUSART(8);
	
	registerServo(0);
	registerServo(1);
	registerServo(7);
	
	initServoTimer(100);

	printString("Welcome!\r\n");

	while(1) {
	}
	return 0;
}


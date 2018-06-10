#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"
#include "servo.h"

char bytes[10] = {0};
int bytes_index = 0;

int pwm_counter = 0;
char servo_select = 0;

volatile int run = 0;

ISR(USART_RX_vect) {
	char byte = UDR0;
	transmitByte(byte);
	if (byte == '\r') {
		transmitByte('\n');
		bytes[bytes_index] = 0;
		if(bytes[0] == 's') {
			servo_select = atoi(bytes+1);
		} else if (bytes[0] == 'r') {
			run = 1;
		} else if (bytes[0] == 'p') {
			run = 0;
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
	registerServo(2);
	registerServo(3);
	registerServo(4);
	registerServo(5);
	registerServo(6);
	registerServo(7);
	
	initServoTimer(100);

	printString("Welcome!\r\n");

	while(1) {
		if(run == 1) {
			setServoAngle(0, 22);
			setServoAngle(1, 14);

			setServoAngle(2, 12);
			setServoAngle(3, 20);

			setServoAngle(4, 22);
			setServoAngle(5, 24);

			setServoAngle(6, 12);
			setServoAngle(7, 10);
			_delay_ms(2000);


			setServoAngle(0, 17);
			setServoAngle(1, 24);

			setServoAngle(2, 17);
			setServoAngle(3, 10);

			setServoAngle(4, 17);
			setServoAngle(5, 14);

			setServoAngle(6, 17);
			setServoAngle(7, 20);
			_delay_ms(2000);
		}
	}
	return 0;
}


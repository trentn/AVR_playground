#include <avr/io.h>
#include "USART.h"

int get_value(void) {
	char value[10] = {0};
	for(int i; i < 9; i++) {
		char byte = receiveByte();
		if(byte == '\r' || byte == '\n') {
			break;
		}
		value[i] = byte;
	}
	return atoi(value);
}

/*
 * secondAtmega.c
 *
 * Created: 22-Feb-23 8:11:52 PM
 * Author : User
 */ 

#include <avr/io.h>

#define F_CPU 1000000

#include <util/delay.h>


int main(void)
{
	DDRB = 0b00000000;
	DDRA = 0b11111111;
	PORTB = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		uint8_t b = PINB;		
		++b;
		
		PORTA = b;
		
		_delay_ms(50);
    }
}


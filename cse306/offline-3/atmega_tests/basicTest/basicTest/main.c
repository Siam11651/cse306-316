/*
 * basicTest.c
 *
 * Created: 18-Feb-23 12:15:30 PM
 * Author : User
 */ 

#include <avr/io.h>


int main(void)
{
	DDRD = 0xff;
	DDRA = 0xff;
	DDRB = 0xff;//output
    /* Replace with your application code */
    while (1) 
    {
		PORTD = 0xff;
		PORTA = 0xff;
		PORTB = 0xff;
    }
}


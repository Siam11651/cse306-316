/*
 * atmegaASclock.c
 *
 * Created: 23-Feb-23 7:10:22 PM
 * Author : User
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>


int main(void)
{
    /* Replace with your application code */
	DDRB = 0xff;//output
	int i = 0;
    while (1) 
    {
		if(++i >= 4) {
			break;
		}
		PORTB = 0xff;//all on
		PORTB = 0x00;//all off
		_delay_ms(3000); // wait till 3 second for next pulse
		
    }
}


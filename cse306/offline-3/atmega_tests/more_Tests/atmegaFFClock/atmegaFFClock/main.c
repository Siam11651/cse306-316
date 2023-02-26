#include <avr/io.h>
#include <avr/interrupt.h> //STEP1
#define F_CPU 1000000
#include<util/delay.h>

ISR(INT1_vect){	//step 2
	PORTB = 0xff;	//turn on all wait 0.5 s, turn off
	_delay_ms(1000);
	GIFR = GIFR | ( 1 << INTF1);
	PORTB = 0x00;
}
int main(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	GICR = (1 << INT1); //STEP3 INT1 at D.3 active
	MCUCR = MCUCR | (1 << ISC11);//STEP4 MCUCR ISC11-10 = 11 means rising edge of INT1
	MCUCR = MCUCR | (1 << ISC10); //STEP4
	sei();//STEP5
while(1); }
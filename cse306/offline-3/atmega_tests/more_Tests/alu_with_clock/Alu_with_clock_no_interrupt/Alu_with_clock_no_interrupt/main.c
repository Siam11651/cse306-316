/*
 * ALU.c
 *
 * Created: 18-Feb-23 7:10:41 PM
 * Author : User
 */

#include <avr/io.h>
#define F_CPU 1000000 // Clock Frequency
#include <util/delay.h>

int main(void)
{
	DDRD = 0b10000111;	//0b10000111			//D[3] interrrupt3 D[7] - Zero Flag,
	// setting output and input modes
	DDRA = 0b00000000;	//0b00000000			// in1[3:0], in2[7:4]
	DDRB = 0b01111000;	//0b01111000			// ALUOp B[2:0], B[7] interrupt output, B[6:3] output of operation
	
	uint8_t control_bits;
	uint8_t in1, in2, out;
	while (1)
	{
		control_bits = PINB & 0b00000111;	// ALUOp B[2:0] 

		in1 = PINA & 0b00001111;	// A[3:0]
		in2 = (PINA & 0b11110000) >> 4;	// A[7:4]
		out = 0;

		if(control_bits == 0) {
			out = in1 + in2;
		} else if(control_bits == 1){
			out = in1 - in2;
		} else if(control_bits == 2) {
			out = in1 & in2;
		} else if(control_bits == 3) {
			out = in1 | in2;
		} else if(control_bits == 4) {
			out = ~(in1 | in2);
		} else if(control_bits == 5) {
			out = in1 << in2;
		} else if(control_bits == 6) {
			out = in1 >> in2;
		}

		out = out & 0b00001111;	
		if(!out) {	// out == 0b00000000 so 0 flag on
			out = out | 0b10000000;
		}
		if (PIND & 0b00001000) {// if D.3 on, turn on B.7 
			PINB = PINB | (0b10000000);
			_delay_ms(50);
			PINB = PINB & (0b01111111);//turn off B.7
		}
// out = 0bxxxxVVVV, we have to shift this value by 3 times and output in PORTB[6:3]
		PORTB = (out << 3);
		
	}
}
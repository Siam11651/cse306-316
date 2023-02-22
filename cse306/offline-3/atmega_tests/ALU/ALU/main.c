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
	// setting output and input modes
	DDRA = 0x00;	//0b00000000			// A[3:0], B[7:4]
	DDRB = 0xF8;	//0b11111000			// ALUOp B[2:0]
	DDRD = 0x8f;	//0b10001111			// D[7] - Zero Flag, D[3:0] - Output

	uint8_t control_bits;
	uint8_t in1, in2, out;

	while (1)
	{
		control_bits = PINB & 0b00000111;	// ALUOp B[2:0] 

		in1 = PINA & 0b00001111;	// A[3:0]
		in2 = (PINA & 0b11110000) >> 4;	// B[7:4]
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

		PORTD = out;
	}
}



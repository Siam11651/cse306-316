/*
 * InstructionMemory.c
 *
 * Created: 18-Feb-23 7:46:25 PM
 * Author : User
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

uint16_t INSTRUCTIONS[12] = {
	0x306f,
	0x3013,
	0xc011,
	0x6112,
	0x0213,
	0x7075,
	0xe774,
	0x2714,
	0xb123,
	0x4442,
	0xa441,
	0x5132
};

uint8_t pc = -1;
uint16_t instruction;

int main(void)
{
	 
	// setting up input output mode
    DDRA = 0b00000000;	// input pc
	PORTA = 0x00;
	
	// Instruction : D[7:0]B[7:0]
	DDRB = 0b11111111;	// lower 8-bits of instruction
	DDRD = 0b11111111;	// upper 8-bits of instruction
	
	while (1) 
    {
		if (PINA != pc) {	// checking if pc is same is previous, would be a mistake
			pc = PINA;
			instruction = INSTRUCTIONS[pc];
			PORTB = (uint8_t) instruction;
			PORTD = (uint8_t) (instruction >> 8); 
			_delay_ms(50);
		}
    }
}



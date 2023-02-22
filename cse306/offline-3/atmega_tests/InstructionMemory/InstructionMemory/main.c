/*
 * InstructionMemory.c
 *
 * Created: 18-Feb-23 7:46:25 PM
 * Author : User
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

uint16_t INSTRUCTIONS[1<<8] = {
	0x206f, 0x2013, 0x702e, 0xe127, 0x6123, 0xb724, 0x5213, 0xf221, 0x8321, 0xa1c0, 0x1331, 0xd224, 0x5610, 0x7661, 0x5620, 0x7661, 0xa1a0, 0x4727, 0x9421, 0x3115, 0x2661, 0xd620, 0x0241, 0x2661, 0xd610, 0xa1c0, 0x8271, 0xa110
};

int main(void)
{
	
	// setting up input output mode
    DDRA = 0b00000000;	// input pc
	
	// Instruction : D[7:0]B[7:0]
	DDRB = 0b11111111;	// lower 8-bits of instruction
	DDRD = 0b11111111;	// upper 8-bits of instruction
	
	uint8_t pc = -1;
	uint16_t instruction;
	
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



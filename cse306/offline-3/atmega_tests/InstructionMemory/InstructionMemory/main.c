/*
 * InstructionMemory.c
 *
 * Created: 18-Feb-23 7:46:25 PM
 * Author : User
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

uint16_t INSTRUCTIONS[1<<8] =
{
	306f,
	3012,
	3023,
	6123
};

uint16_t instructionCount = 4;

int main(void)
{
	
	// setting up input output mode
    DDRA = 0b00000000;	// instruction count write
	
	// Instruction : D[7:0]B[7:0]
	DDRB = 0b11111111;	// lower 8-bits of instruction
	DDRD = 0b11111111;	// upper 8-bits of instruction
	DDRC = 0b11111111; // instruction count read
	
	// uint8_t pc = -1;
	uint16_t instruction;

	for(uint16_t pc = 0; pc < instructionCount; ++pc)
	{
		PORTC = pc;
		instruction = INSTRUCTIONS[pc];
		PORTB = (uint8_t) instruction;
		PORTD = (uint8_t) (instruction >> 8); 
		pc = PINA;
		_delay_ms(50);
	}

	while(1); // so it does not die
	
	// while (1) 
    // {
	// 	if (PINA != pc) {	// checking if pc is same is previous, would be a mistake
	// 		pc = PINA;
	// 		instruction = INSTRUCTIONS[pc];
	// 		PORTB = (uint8_t) instruction;
	// 		PORTD = (uint8_t) (instruction >> 8); 
	// 		_delay_ms(50);
	// 	}
    // }
}


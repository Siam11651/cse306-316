// /*
//  * InstructionMemory.c
//  *
//  * Created: 18-Feb-23 7:46:25 PM
//  * Author : User
//  */ 
// 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// 
// uint16_t INSTRUCTIONS[1<<8] =
// {
// 	0x306f,
// 	0x3012,
// 	0x3023,
// 	0x6123
// };
// 
// uint16_t instructionCount = 4;
// 
// int main(void)
// {
// 	MCUCSR = (1<<JTD);  MCUCSR = (1<<JTD);
// 	
// 	// setting up input output mode
//     DDRA = 0b00000000;	// instruction count write
// 	PORTA = 0;
// 	
// 	// Instruction : D[7:0]B[7:0]
// 	DDRB = 0b11111111;	// lower 8-bits of instruction
// 	DDRD = 0b11111111;	// upper 8-bits of instruction
// 	DDRC = 0b11111111; // instruction count read
// 	
// 	uint8_t pc = 0;
// 	uint16_t instruction;
// 
// 	while(1)
// 	{
// 		if(pc >= instructionCount)
// 		{
// 			break;
// 		}
// 		
// 		PORTC = pc;
// 		instruction = INSTRUCTIONS[pc];
// 		PORTB = (uint8_t) instruction;
// 		PORTD = (uint8_t) (instruction >> 8); 
// 		
// 		_delay_ms(2000);
// 		
// 		pc = PINA;
// 	}
// 
// 	while(1); // so it does not die
// 	
// 	// while (1) 
//     // {
// 	// 	if (PINA != pc) {	// checking if pc is same is previous, would be a mistake
// 	// 		pc = PINA;
// 	// 		instruction = INSTRUCTIONS[pc];
// 	// 		PORTB = (uint8_t) instruction;
// 	// 		PORTD = (uint8_t) (instruction >> 8); 
// 	// 		_delay_ms(50);
// 	// 	}
//     // }
// }
// 
// 
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
	0x306f,
	 0x3012,
	 0x3023,
	 0x6123
};



int main(void)
{
	GICR = 1 << INT1;
	MCUCSR = (1<<JTD);  MCUCSR = (1<<JTD);
	MCUCR = 1 << ISC11;
	MCUCR = 1 << ISC10;
	// setting up input output mode
    DDRA = 0b00000000;	// input pc
	
	// Instruction : D[7:0]B[7:0]
	DDRB = 0b11111111;	// lower 8-bits of instruction
	DDRC = 0b11111111;	// upper 8-bits of instruction
	
	
	uint8_t pc = -1;
	uint16_t instruction;
	
	while (1) 
    {
		if (PINA != pc) {	// checking if pc is same is previous, would be a mistake
			pc = PINA;
			instruction = INSTRUCTIONS[pc];
			PORTB = (uint8_t) instruction;
			PORTC = (uint8_t) (instruction >> 8); 
			_delay_ms(50);
		}
    }
}



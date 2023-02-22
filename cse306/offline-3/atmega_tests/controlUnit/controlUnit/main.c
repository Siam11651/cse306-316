/*
 * controlUnit.c
 *
 * Created: 18-Feb-23 7:29:55 PM
 * Author : User
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

uint16_t CONTROL_ROM[1<<4] = {0x901, 0x011, 0x903, 0x500, 0x505, 0x904, 0x900, 0x503, 
							  0x008, 0x640, 0x506, 0x902, 0x501, 0x780, 0x502, 0x021};

int main(void)
{
	
	// Setting output input modes
    DDRA = 0xF0; // 0b11110000	// A[3:0] - Opcode Input
	
	// Control Unit Bits : D[3:0]B[7:0]
	DDRB = 0xFF;	// B[mem_read[1], mem_write[1], branch[1], branch_neq[1], jump[1], alu_op[3]]
	
	DDRD = 0xFF;	// D[reg_dst[1], alu_src[1], mem_to_reg[1], reg_write[1]]
	
	uint8_t opcode = ~0;
	
    while (1) 
    {
		if (opcode != (0b00001111 & PINA)) {	// checking if new opcode is same as previous, no need to calculate if it is
			opcode = (0b00001111 & PINA);
			PORTB = CONTROL_ROM[opcode];
			PORTD = (CONTROL_ROM[opcode] >> 8);	// shifting to get highest 4 bit of ROM output
			_delay_ms(50);
		}
    }
}

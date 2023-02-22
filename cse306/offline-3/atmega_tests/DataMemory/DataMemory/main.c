/*
 * DataMemory.c
 *
 * Created: 18-Feb-23 7:59:55 PM
 * Author : User
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define CLK_		0x04
#define RESET_		0x08
#define MEM_READ_	0x02
#define MEM_WRITE_	0x01

uint8_t MEMORY[1<<4];

void memory_reset() {
	for (int i=0; i<(1<<4); i++)
		MEMORY[i] = 0;
}

int main(void)
{
	
    DDRA = 0b11110000;	// A[0] - Memory Write, A[1] - Memory Read, A[2] - Clock, A[3] - Reset
	DDRB = 0b00000000;	// Address B[3:0], Write Data B[7:4]
	
	DDRD = 0b11111111;	// D[3:0] - Data Read
	
	memory_reset();
	
	uint8_t last_control_input = 0b00000000, current_control_input;
	uint8_t last_write_data_address = 0b00000000, current_write_data_address;
	
    while (1) 
    {
		current_control_input = PINA & 0b00001111;
		current_write_data_address = PINB;
		if (current_control_input & RESET_) {
			memory_reset();
			_delay_ms(100);
		} 
		// checking if last and current clock bits are different
		else if ((last_control_input ^ current_control_input) & CLK_) {		
			// so previous clk was 1 and current clk is 0, a negative edge
			if (last_control_input & CLK_) {						
				if (last_control_input & MEM_WRITE_) {
					MEMORY[last_write_data_address & 0b00001111] = (last_write_data_address >> 4);
				}
			}
			_delay_ms(100);
		} 
		if (current_control_input & MEM_READ_) {
			PORTD = MEMORY[current_write_data_address & 0b00001111];
		}
			
		last_control_input = current_control_input;
		last_write_data_address = current_write_data_address;
	}
}


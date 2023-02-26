/*
 * registers.c
 *
 * Created: 18-Feb-23 8:23:28 PM
 * Author : User
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define WRITE_REG_	0b00000100
#define CLK_		0b00001000
#define RESET_		0b00010000
#define OUTPUT_		0b00001000

uint8_t REGISTER[8] = {0};
void uart_init()
{
	UCSRA = 0b00000010;
	UCSRB = 0b00011000;
	UCSRC = 0b10000110;
	UBRRH = 0;
	UBRRL = 12;
}unsigned char uart_receive(void){
	while ((UCSRA & (1<<RXC)) == 0x00);
	return UDR;
}void uart_send(unsigned char data){
	while ((UCSRA & (1<<UDRE)) == 0x00);
	UDR = data;
}

int main(void)
{
	uart_init();
	_delay_ms(1000);
	
	// Some pins of PORTC can not be used for I/O directly.
	// turn of JTAG to use them/,
	// o write a 1 to the JTD bit twice consecutively to turn it off
	MCUCSR = (1<<JTD);  MCUCSR = (1<<JTD);
	
	// setting up input and output modes
    DDRA = 0x00;	// A[3:0] - SrcReg1, A[7:4] - SrcReg2
	DDRB = 0x00;	// B[3:0] - DstReg, B[7:4] - Write Data
	
	DDRC = 0xFF;	// C[3:0] - $1, C[7:4] - $2
	DDRD = 0b11100000;	// D[2] - WriteReg, D[3] - CLK, D[4] - RESET
	
	uint8_t src, dst_data, last_control = 0, current_control;
	
    while (1) 
    {
		src = PINA;
		dst_data = PINB;
		current_control = PIND;
		// src[3:0] is srcReg1, src[7:4] is srcReg2, so first part of OR is srcReg2, second part is srcReg1
		PORTC = ( REGISTER [(src>>4) & 0b00000111] << 4 ) | ( REGISTER[src & 0b00000111 ] );
		
		if (current_control&RESET_) {
			for (uint8_t i=0; i<8; i++) REGISTER[i] = 0;
			_delay_ms(100);
			continue;
		}
		
		// checking if clk is different in current and last control
		if ((current_control^last_control)&CLK_) {					
			
			//so last control's clk was 1, a negative edge, with write instruction on
			if ((last_control&CLK_) && (last_control&WRITE_REG_)) {	
				REGISTER[dst_data&0b00000111] = (dst_data>>4);
				// PORTD = dst_data & 0b11110000;
			}
			_delay_ms(100);
			
			//now we send over uart to arduino, the values of register array
			for (int i = 0; i < 8; i++) {
				uart_send((unsigned char)REGISTER[i] + 50);
				uart_send(' ');
			}
			
			uart_send('\r');
			uart_send('\n');
		}
		
		last_control = current_control;
    }
}

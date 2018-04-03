/*
 * Microweek2.c
 *
 * Created: 2/8/2018 3:44:30 PM
 * Author : Gijs
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void opdracht4();

const unsigned char Numbers [16] =
{
	// dPgfe dcba
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // 10
	0b01111100, // 11
	0b00111001, // 12
	0b01011110, // 13
	0b01111001, // 14
	0b01110001, // 15
};

const unsigned char patern [8] =
{
	// dPgfe dcba
	0b00100000, // 0
	0b00000001, // 1
	0b00000010, // 2
	0b01000000, // 3
	0b00010000, // 4
	0b00001000, // 5
	0b00000100, // 6
	0b01000000, // 7
};

/******************************************************************/
void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				until/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}
int x = 0;


/******************************************************************/
ISR( INT0_vect )
/* 
short:			ISR INT0
inputs:			
outputs:	
notes:			Set PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
{
	if(x<=8){
		PORTD |= (1<<x);
		x++;
	}	
	//if(x==15){
		//x=0;
	//}
	//PORTC = Numbers[x];
	//x++;
		wait(1000);
}

/******************************************************************/
ISR( INT1_vect )
/* 
short:			ISR INT1
inputs:			
outputs:	
notes:			Clear PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
{
	if(x>=4){
		PORTD &= ~(1<<x);
		x--;
	}
	//PORTC = Numbers[x];
	//x--;
	//if(x==0){
		//x=15;
	//}
	wait(1000);
				
}

/******************************************************************/
int main( void )
/* 
short:			main() loop, entry point of executable
inputs:			
outputs:	
notes:			Slow background task after init ISR
Version :    	DMK, Initial code
*******************************************************************/
{
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input	
	DDRC = 0xFF;

	// Init Interrupt hardware
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x03;			// Enable INT1 & INT0
	
	//opdracht4();
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();			

	while (1)
	{
		//PORTD ^= (1<<7);	// Toggle PORTD.7
		//wait( 500 );								
	}

	return 1;
}

void opdracht4(){
	int index = 0;
	while(index<sizeof(patern)){
		PORTC = patern[index];
		wait(1000);
		index++;
	}
}


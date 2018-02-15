/*
 * Microweek2.c
 *
 * Created: 2/8/2018 3:44:30 PM
 * Author : Gijs
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


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
    PORTD |= (1<<5);		
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
    PORTD &= ~(1<<5);		
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

	// Init Interrupt hardware
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x03;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();				

	while (1)
	{
		PORTD ^= (1<<7);	// Toggle PORTD.7
		wait( 500 );								
	}

	return 1;
}


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_E 	3
#define LCD_RS	2

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void lcd_write_at(int, char *str);

/******************************************************************/
void lcd_strobe_lcd_e(void)

/*
short:			Strobe LCD module E pin --__
inputs:
outputs:
notes:			According datasheet HD44780
Version :    	DMK, Initial code
*******************************************************************/
{
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);			// nodig
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			// nodig?
}

/******************************************************************/
void init_4bits_mode(void)
/*
short:			Init LCD module in 4 bits mode.
inputs:
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// PORTC output mode and all low (also E and RS pin)
	//DDRC = 0xFF;
	//PORTC = 0x00;
	//
	//PORTC = 0x01;
	//PORTC |= (1<<LCD_RS);
	//lcd_strobe_lcd_e();
	//PORTC &= ~(1<<LCD_RS);
	//lcd_strobe_lcd_e();
//
	//
	//// Step 2 (table 12)
	//PORTC = 0x20;	// function set
	//lcd_strobe_lcd_e();
//
	 ////Step 3 (table 12)
	//PORTC = 0x20;   // function set
	//lcd_strobe_lcd_e();
	//PORTC = 0x80;
	//lcd_strobe_lcd_e();
//
	 ////Step 4 (table 12)
	//PORTC = 0x00;   // Display on/off control
	//lcd_strobe_lcd_e();
	//PORTC = 0xF0;
	//lcd_strobe_lcd_e();
//
	 ////Step 4 (table 12)
	//PORTC = 0x00;   // Entry mode set
	//lcd_strobe_lcd_e();
	//PORTC = 0x60;
	//lcd_strobe_lcd_e();
	
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	lcd_write_command(0x01);
	lcd_strobe_lcd_e();
	
	lcd_write_command(0x02);
	lcd_strobe_lcd_e();

	// Step 2 (table 12)
	lcd_write_command(0x20);	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	lcd_write_command(0x20);   // function set
	lcd_strobe_lcd_e();
	lcd_write_command(0x80);
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	lcd_write_command(0x00);   // Display on/off control
	lcd_strobe_lcd_e();
	lcd_write_command(0xF0);
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	lcd_write_command(0x00);   // Entry mode set
	lcd_strobe_lcd_e();
	lcd_write_command(0x60);
	lcd_strobe_lcd_e();
}

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

int tenthValue = 0;
int presetValue = -10;
/******************************************************************/
void lcd_write_string(char *str)
/*
short:			Writes string to LCD at cursor position
inputs:
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// Het kan met een while:

	// while(*str) {
	// 	lcd_write_data(*str++);
	// }


	//lcd_write_command(0x80);
	lcd_write_command(0x02);
	lcd_strobe_lcd_e();
	// of met een for
	for(;*str; str++){
		lcd_write_data(*str);
	}
	//PORTC = 0x08;
	//lcd_strobe_lcd_e();
	//PORTC = 0x0C;
	//lcd_strobe_lcd_e();
}


/******************************************************************/
void lcd_write_data(unsigned char byte)
/*
short:			Writes 8 bits DATA to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

/******************************************************************/
void lcd_write_command(unsigned char byte)
/*
short:			Writes 8 bits COMMAND to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void timer2Init( void )
{
	TCNT2 = -10; // Compare value of counter 2
	TIMSK |= 0x02; // T2 compare match interrupt enable
	SREG |= 0x01; // turn_on intr all
	TCCR2 = 0b00000111;
}

ISR( TIMER2_OVF_vect )
{
	TCNT2 = -10;
	tenthValue++; // Increment counter
	lcd_write_string("HALLO");
	
}

void lcd_write_at(int pos, char *str){
	int i;
	lcd_write_command(0x02);
	lcd_strobe_lcd_e();
	
	for(i = 0; i < pos; i++){
		lcd_write_command(0x14);
		lcd_strobe_lcd_e();
	}
	lcd_write_string(*str);
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
	
	DDRD &= 0x01; // set PORTD.7 for input
	DDRB = 0xFF; // PORTB is output
	DDRA = 0xFF;
	
	init_4bits_mode();
	
	timer2Init();
	
	// Init I/O
	//DDRD = 0xFF;			// PORTD(7) output, PORTD(6:0) input

	// Init LCD

	// Write sample string
	
	//lcd_write_at(2, "1234567");

	// Loop forever
	while (1)
	{	
		PORTA = TCNT2; // show value counter 2
		PORTB = tenthValue; // show value tenth counter
		wait(10);
	}

	return 1;
}
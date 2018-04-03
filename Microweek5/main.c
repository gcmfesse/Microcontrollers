/*
* Project name : Demo5_7a : spi - 7 segments display
* Author : Avans-TI, WvdE, JW
* Revision History : 20110228: - initial release; 20120307: - this version
* Description : This program sends data to a 4-digit display with spi
* Test configuration:
 MCU: ATmega128
 Dev.Board: BIGAVR6
 Oscillator: External Clock 08.0000 MHz
 Ext. Modules: Serial 7-seg display
 SW: AVR-GCC
* NOTES : Turn ON switch 15, PB1/PB2/PB3 to MISO/MOSI/SCK
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define BIT(x) ( 1<<x )
#define DDR_SPI DDRB // spi Data direction register
#define PORT_SPI PORTB // spi Output register
#define SPI_SCK 1 // PB1: spi Pin System Clock
#define SPI_MOSI 2 // PB2: spi Pin MOSI
#define SPI_MISO 3 // PB3: spi Pin MISO
#define SPI_SS 0 // PB0: spi Pin Slave Select
// wait(): busy waiting for 'ms' millisecond - used library: util/delay.h
void timer1Init(void);
void toggleSeconds(void);

int stateHours = 1;
int stateMinutes = 1;

const unsigned char NumbersDotted[10] =
{
	0b10000000,		//0    
	0b10000001,		//1
	0b10000010,		//2    
	0b10000011,		//3
	0b10000100,		//4   
	0b10000101,		//5   
	0b10000110,		//6   
	0b10000111,		//7    
	0b10001000,		//8    
	0b10001001		//9     
};


void wait(int ms)
{
for (int i=0; i<ms; i++)
_delay_ms(1);
}

void spi_masterInit(void)
{
DDR_SPI = 0xff; // All pins output: MOSI, SCK, SS, SS_display
DDR_SPI &= ~BIT(SPI_MISO); // except: MISO input
PORT_SPI |= BIT(SPI_SS); // SS_ADC == 1: deselect slave
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); // or: SPCR = 0b11010010;
// Enable spi, MasterMode, Clock rate fck/64
// bitrate=125kHz, Mode = 0: CPOL=0, CPPH=0
}

// Write a byte from master to slave
void spi_write( unsigned char data )
{
SPDR = data; // Load byte --> starts transmission
while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
}

// Write a byte from master to slave and read a byte from slave - not used here
char spi_writeRead( unsigned char data )
{
SPDR = data; // Load byte --> starts transmission
while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
data = SPDR; // New received data (eventually, MISO) in SPDR
return data; // Return received byte
}

// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
PORTB &= ~BIT(chipNumber);
}

// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{
PORTB |= BIT(chipNumber);
}

// Initialize the driver chip (type MAX 7219)
void displayDriverInit()
{
spi_slaveSelect(0); // Select display chip (MAX7219)
 spi_write(0x09); // Register 09: Decode Mode
 spi_write(0xFF); // -> 1's = BCD mode for all digits
 spi_slaveDeSelect(0); // Deselect display chip
 spi_slaveSelect(0); // Select dispaly chip
 spi_write(0x0A); // Register 0A: Intensity
 spi_write(0x04); // -> Level 4 (in range [1..F])				
 spi_slaveDeSelect(0); // Deselect display chip
 spi_slaveSelect(0); // Select display chip
 spi_write(0x0B); // Register 0B: Scan-limit
 spi_write(0x03); // -> 1 = Display digits 0..1						
 spi_slaveDeSelect(0); // Deselect display chip
 spi_slaveSelect(0); // Select display chip
 spi_write(0x0C); // Register 0B: Shutdown register
 spi_write(0x01); // -> 1 = Normal operation
 spi_slaveDeSelect(0); // Deselect display chip
}

// Set display on ('normal operation')
void displayOn()
{
 spi_slaveSelect(0); // Select display chip
 spi_write(0x0C); // Register 0B: Shutdown register
 spi_write(0x01); // -> 1 = Normal operation
 spi_slaveDeSelect(0); // Deselect display chip
}

// Set display off ('shut down')
void displayOff()
{
 spi_slaveSelect(0); // Select display chip
 spi_write(0x0C); // Register 0B: Shutdown register
 spi_write(0x00); // -> 1 = Normal operation
 spi_slaveDeSelect(0); // Deselect display chip
}

int main()
{
DDRB=0x01; // Set PB0 pin as output for display select
DDRC = 0xFF;
DDRD = 0xFF;
DDRD &= ~(1 << PIND1);
DDRD &= ~(1 << PIND0);

EICRA = 0x0B;
EIMSK = 0x03;

timer1Init();
spi_masterInit(); // Initialize spi module
displayDriverInit(); // Initialize display chip
// clear display (all zero's)
for (char i =1; i<=4; i++)
{
	spi_slaveSelect(0); // Select display chip
	spi_write(i); // digit adress: (digit place)
	spi_write(0); // digit value: 0
	spi_slaveDeSelect(0); // Deselect display chip
}
// set first dot for hours
spi_slaveSelect(0); // Select display chip
spi_write(3); // digit adress: (digit place)
spi_write(NumbersDotted[0]); // digit value: 0
spi_slaveDeSelect(0); // Deselect display chip

while(1){
	wait(10);
}

 return (1);
}

void spi_writeWord(unsigned char adress, unsigned char data){
	spi_slaveSelect(0); // Select display chip
	spi_write(adress); // digit adress: (digit place)
	spi_write(data); // digit value: i (= digit place)
	spi_slaveDeSelect(0); // Deselect display chip
	wait(10);
}

void writeLedDisplay(int value, int startpos){
	int i = 1;
	int pos = startpos;
	if(value == 0){
		spi_writeWord(pos, value);
	}
	while (value > 0) {
		int digit = value % 10;
		spi_writeWord(pos, digit);
		value /= 10;
		i++;
		pos++;
	}
	
	
}

void timer1Init(void){
	OCR1A = 31500; // 16-bits compare value of counter 1
	TIMSK |= BIT(4); // T1 compare match A interrupt enable
	TCCR1A = 0b00000000; // Initialize T1: timer, prescaler=256,
	TCCR1B = 0b00001100; // compare output disconnected, CTC, RUN
	sei();
}

int sCount = 0;
int minutes = 0;
int hours = 0;

ISR(TIMER1_COMPA_vect){
	sCount++; // Increment s counter
	toggleSeconds();
	PORTC ^= BIT(0); // Toggle bit 0 van PORTC
	if ( sCount == 60 ) // Every 1 minute:
	{ //
		minutes++; // Increment minutes counter
		//writeLedDisplay(minutes,1);
		sCount = 0; // Reset s-counter
		if ( minutes >= 60 ) // Every hour:
		{ //
			minutes = 0; // Reset min-counter
			writeLedDisplay(0,2);
			hours++; // Increment hours counter
			writeLedDisplay(hours,3);
			spi_writeWord(3, NumbersDotted[hours%10]);
			if ( hours >= 24 ){ // Every day:
				hours = 0; // reset hours counter
				writeLedDisplay(0,4);
			}
		}
	}
}

ISR(INT0_vect){
	if(stateMinutes == 1){
		stateMinutes = 0;
		minutes++;
		if(minutes >=60){
			minutes = 0;
			writeLedDisplay(0,2);
		}
		writeLedDisplay(minutes,1);
		spi_writeWord(1, NumbersDotted[minutes%10]);
		
		stateMinutes = 1;
	}
}


ISR( INT1_vect )
{
	if(stateHours == 1){
		stateHours = 0;
		hours++;
		if(hours >=24){
			hours = 0;
			writeLedDisplay(0,4);
		}
		writeLedDisplay(hours,3);
		spi_writeWord(3, NumbersDotted[hours%10]);
		stateHours = 1;
		
	}
}

void toggleSeconds(void){
	if(sCount%2 == 0){
		writeLedDisplay(minutes,1);
	}else{
		spi_writeWord(1, NumbersDotted[minutes%10]);
	}
}
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
void opdracht2();
void opdracht3();
void opdracht4();
void opdracht5();
void opdracht6();
void opdracht7();
void checkState2();

/******************************************************************/
void wait( int ms )

{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

/******************************************************************/
int waittimes[] = {500,100};
	int stateNum = 0;

int main( void )
{
	 //DDRD = 0b10000000;
	 //DDRC = 0b11111110;
	 
	 DDRD = 0b11111111;
	 DDRC = 0b00000000;
	while (1)
	{
		checkState2(stateNum);
		opdracht7()	;		
	}

	return 1;
}

/*	
	Description: Lets the light blink every 500ms
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
void opdracht2(){
		PORTD = 0x80;
		wait(500);
		PORTD = 0x00;
		wait(500);
}

/*	
	Description: When you hold the button the light blinks every 500ms
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
void opdracht3(int *state){
		if(PINC == 0b00000001 || *state == 1) {
			PORTD = 0x80;
			wait(500);
			PORTD = 0x00;
			wait(500);
			}
}

/*	
	Description: Light pattern using bit shifting
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
void opdracht4(int *state){
	int x = 0x80;
	PORTD = x;
	wait(100);
		while(x != 0x01){
		x=x>>1;	
		PORTD = x;
		wait(100);
		}
		while(x != 0x80){
		x=x<<1;	
		PORTD = x;	
		wait(100);
		}
}
	
/*	
	Description: Cool light pattern using an array
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/	
void opdracht5(int *state){
	int ledpins[] = {0x80,0xC0,0xE0,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x03,0x01,0x00,
		0x80,0xC0,0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00,
		0x01,0x03,0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,
		0x81,0xC3,0x66,0x3C,0x66,0xC3,0x81,0x00,
		0xFF,0x00,0xFF,0x00};
		int index = 0;
		while(index<sizeof(ledpins)){
			PORTD = ledpins[index];
			wait(300);
			index++;
		}
}

/*	
	Description: The light blinks every 500ms, but when you press the button it blinks every 100ms
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
void opdracht6(int acc){	
		PORTD = 0x80;
		wait(waittimes[acc]);
		PORTD = 0x00;
		wait(waittimes[acc]);
}

/*	
	Description: Changes the state when you press the button
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
int checkstate(int *state){
	if(PINC == 0b00000001){
		return !state;
	}else{return state;}
}

/*	
	Description: Changes the state when you press a specific button.
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
void opdracht7(){
	if(stateNum == 0){
		PORTD = 0x01;
		}else if(stateNum == 1){
		PORTD = 0x02;
		}else if(stateNum == 2){
		PORTD = 0x04;
		}else if(stateNum == 3){
		PORTD = 0x08;
		}else if(stateNum == 4){
		PORTD = 0x10;
	}
	checkState2();
}

/*	
	Description: lights a led depending on the state.
	Author: Gijs van Fessem & Frank Veraart
	Created: week 1
*/
void checkState2(){
	if((PINC == 0b01000000 && stateNum == 0 )||(PINC == 0b01000000 && stateNum == 2)){
		stateNum = 1;
		}else if((PINC == 0b10000000 && stateNum == 1 )|| (PINC == 0b10000000 && stateNum == 4) || (PINC == 0b10000000 && stateNum == 3) || (PINC == 0b10000000 && stateNum == 2)){
		stateNum = 0;
		}else if((PINC == 0b00100000 && stateNum == 0 )|| (PINC == 0b00100000 && stateNum == 1)){
		stateNum = 2;
		}else if(PINC == 0b00100000 && stateNum == 2){
		stateNum = 3;
		}else if((PINC == 0b00100000 && stateNum == 3 )|| (PINC == 0b01000000 && stateNum == 3)){
		stateNum = 4;
	}
}




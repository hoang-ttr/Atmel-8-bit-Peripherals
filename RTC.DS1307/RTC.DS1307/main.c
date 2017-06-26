/*
 * RTC.DS1307.c
 *
 * Created: 02/23/17 05:32:23 PM
 * Author : Hoang Thien Tran
 */ 

#define F_CPU 16000000UL

#include "src/lcd4bit.h"
#include "src/DS1307.h"

int main(void)
{
	PORTC |= (1<<7);	//pull-up for buttons
	
	Lcd4_Ini();
	Lcd4_Clear();
	RTCIni();
	
	Lcd4_SetString("RTC in running");
	_delay_ms(500);
	
	TCCR0 |= (1<<CS02)|(1<<CS00);	//Use for timer interrupt to update LCD
	TIMSK |= (1<<TOIE0);
	sei();
	
	timevalue[0]=Dec2BCD(Sec);
	timevalue[1]=Dec2BCD(Min);
	if(Mode!=0) timevalue[2]=Dec2BCD(Hour)|(Mode<<6)|(AmPm<<5);
	else timevalue[2]=Dec2BCD(Hour);
	timevalue[3]=Dec2BCD(Day);
	timevalue[4]=Dec2BCD(Date);
	timevalue[5]=Dec2BCD(Month);
	timevalue[6]=Dec2BCD(Year);
	
	storedvalue[0]=timevalue[0];
	storedvalue[1]=timevalue[1];
	storedvalue[2]=timevalue[2];
	storedvalue[3]=timevalue[3];
	storedvalue[4]=timevalue[4];
	storedvalue[5]=timevalue[5];
	storedvalue[6]=timevalue[6];

	RTCIni();
	RTCRead(0x00,7);
	Display();
    while (1)
	{
		if (bit_is_clear(PINC,7))
		{
			RTCWrite(0x00,storedvalue,7);
			_delay_ms(300);
		}
	}
}
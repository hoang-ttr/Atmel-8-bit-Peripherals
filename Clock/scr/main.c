/*
 * Clock.c
 *
 * Created: 09/21/16 10:57:45 AM
 * Author : Hoang Thien Tran
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd4bit.h"
#include "Counter.h"
#include "ButtonConfig.h"

int main(void)
{
	DDRA = 0xFF; //LEDs
	DDRB = 0x00; //Buttons
	PORTB= 0xFF; //Pull-up resistors
	DDRC = 0xFF; //LEDs
	DDRD = 0xFF; //LCD
	
	CLK_Ini();
	Lcd4_Ini();
	Lcd4_Clear();
	
	Lcd4_Cursor(1,0);
	Lcd4_SetString("Time");
	Lcd4_Cursor(1,4);
	Lcd4_SetString(":");
	Lcd4_Cursor(1,7);
	Lcd4_SetString(":");
	Lcd4_Cursor(1,10);
	Lcd4_SetString(":");
	Lcd4_Cursor(1,14);
	Lcd4_SetString("PM");
    while (1) 
	{
		changevalue();
		PORTC = timevalue[0];
		PORTA = ~timevalue[1];

		dtostrf(timevalue[2],2,0,timedisplay);
		Lcd4_Cursor(1,5);
		Lcd4_SetString(timedisplay);
		
		dtostrf(timevalue[1],2,0,timedisplay);
		Lcd4_Cursor(1,8);
		Lcd4_SetString(timedisplay);
	
		dtostrf(timevalue[0],2,0,timedisplay);
		Lcd4_Cursor(1,11);
		Lcd4_SetString(timedisplay);
    }
}
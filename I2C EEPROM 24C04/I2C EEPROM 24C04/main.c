/*
 * I2C EEPROM 24C04.c
 *
 * Created: 02/13/17 11:38:14 AM
 * Author : Hoang Thien Tran
 */ 

#define F_CPU 16000000UL

#include <stdlib.h>
#include "src/lcd4bit.h"
#include "src/I2C24C04.h"

int main(void)
{
   uint8_t i, sentdata[8];
   uint8_t n=0,m;	//For testing
   m=n+8;
   char TWIstr[10];

	Lcd4_Ini();		//Ini. LCD
	Lcd4_Clear();
	EEIni();		//Ini. EEPROM
	
	Lcd4_Cursor(1,2);
	Lcd4_SetString("External EEPROM");
	_delay_ms(1000);

	Lcd4_Clear();
	Lcd4_SetString("Writing...");

	for(i=n;i<=(n+7);i++)	sentdata[i-n]=i;
	if(EEWriteByte(n,sentdata))
	{	
		Lcd4_Clear();
		Lcd4_SetString("Written 1st times");
	}
	_delay_ms(1000);

	for(i=m;i<=(m+7);i++)	sentdata[i-m]=i;
	if(EEWriteByte(m,sentdata))
	{
		Lcd4_Clear();
		Lcd4_SetString("Written 2nd times");
	}
	_delay_ms(1000);
	
	Lcd4_Clear();
	Lcd4_SetString("Verifying ...");
	_delay_ms(1000);
	
	Lcd4_Clear();
	Lcd4_SetString("Data Retrieved");
	Lcd4_Cursor(2,1);
	for(i=0;i<=12;i++)
	{
		itoa(EEReadByte(i),TWIstr,10);
		Lcd4_SetString(TWIstr);
	}
	
	while(1){;}
}
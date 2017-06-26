/*
 * MMC.SDwithSPI.c
 *
 * Created: 02/20/17 09:54:33 AM
 * Author : Hoang Thien Tran
 */

#define F_CPU 16000000UL

#include "src/lcd4bit.h"
#include "src/MMC_SD.h"
#include "src/UART.h"

static char buffing[Block_Length];

int main(void)
{
	char Result;
    DDRC=0xFF;		//For 7segs to check for stages in simulation
	PORTC=0x00;

	Lcd4_Ini();
	Lcd4_Clear();
	Lcd4_SetString("Init. MMC...");
	UARTIni(0,1);
	Result=Card_Ini();
	PORTC|=(1<<0);
	if(Result!=0)	goto Finish;
	_delay_ms(1000);
	//----------WRITING INTO THE CARD----------//
	Lcd4_Clear();
	Lcd4_SetString("Writing 1st...");
	char *testStr = "This is a demostration for the use of MMC.SD Card\n";
	Result=Card_Write(1,testStr);
	PORTC|=(1<<1);
	if(Result!=0) goto Finish;
	_delay_ms(1000);

	Lcd4_Clear();
	Lcd4_SetString("Writing 2nd...");
	testStr = "Welcome to EE\n";
	Result=Card_Write(2,testStr);
	PORTC|=(1<<2);
	if(Result!=0) goto Finish;
	_delay_ms(1000);

	Lcd4_Clear();
	Lcd4_SetString("Writing 3rd...");
	testStr = "Demo for MMC.SD";
	Result=Card_Write(3,testStr);
	PORTC|=(1<<3);
	if(Result!=0) goto Finish;
	_delay_ms(1000);

	//----------READIND INTO THE CARD----------//
	Lcd4_Clear();
	Lcd4_SetString("Reading 1st...");
	Result=Card_Read(1,buffing);
	PORTC|=(1<<4);
	UART_SendString(buffing);
	_delay_ms(1000);

	Lcd4_Clear();
	Result=Card_Read(2,buffing);
	PORTC|=(1<<5);
	UART_SendString(buffing);
	Lcd4_SetString(buffing);
	_delay_ms(1000);

	Result=Card_Read(3,buffing);
	PORTC|=(1<<6);
	UART_SendString(buffing);
	Lcd4_Cursor(2,1);
	Lcd4_SetString(buffing);
	_delay_ms(1000);
	goto Succes;
Finish:
	Lcd4_Clear();
	Lcd4_SetString("Failed!");
	//PORTC |= (Result<<4);*/
Succes:
	Lcd4_Clear();
	Lcd4_SetString("Successful!");
    while(1){;}
}
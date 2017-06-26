/*
 * UART.c
 *
 * Created: 02/08/17 04:34:21 PM
 * Author : Hoang Thien Tran
 */ 

#define F_CPU 16000000UL

#include "src/UART.h"
#include "src/lcd4bit.h"

int main(void)
{
	Lcd4_Ini();
	Lcd4_Clear();
	UARTIni(0,1);
	
	Lcd4_Cursor(1,1);
	Lcd4_SetString("Data Tx: ");
	Lcd4_Cursor(2,1);
	Lcd4_SetString("UART Demo");
	
	UART_SendString("UART Demo\n");
	UART_SendString("This is running properly\n");
    while(1)
    {
    }
}
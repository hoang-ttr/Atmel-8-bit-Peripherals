/*
 * Slave.c
 *
 * Created: 02/18/17 10:54:20 PM
 * Author : TRAN THIEN HOANG
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "SPI_Slave.h"
#include "lcd4bit.h"


int main(void)
{
	char strSPI[10];
	
	sei();	//Global Interrupt
    SPISlave_Ini();	//Ini. SPI as Slave
	Lcd4_Ini();		//Ini. LCD
	Lcd4_Clear();
    while (1) 
    {
		Lcd4_SetString("Data Rx:");
		itoa(data_rx,strSPI,10);
		Lcd4_Cursor(2,1);
		Lcd4_SetString(strSPI);
	}
}


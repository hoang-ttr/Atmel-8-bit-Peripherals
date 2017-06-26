/*
 * Master.c
 *
 * Created: 02/16/17 10:58:27 PM
 * Author : TRAN THIEN HOANG
 */ 

#define F_CPU 16000000UL

#include <stdio.h>
#include "src/SPI_Master.h"
#include "src/lcd4bit.h"

uint8_t spi_data[3]={10, 20, 30};		

int main(void)
{
	uint8_t i;
	Lcd4_Ini();
	Lcd4_Clear();
	SPIMaster_Ini();	//Ini. SPI as Master
	_delay_ms(1);		//Wait for slaves to initiate
	for (i=0;i<3;i++)
	{
		SPI_Send_9bit(SS,0b010011001);
		_delay_ms(1000);
	}
	/*for (i=0;i<3;i++)
	{
		SPI_Send_9bit(SS,spi_data[i]);
		_delay_ms(1000);
	}*/	
	Lcd4_SetString("Process done!");
    while (1){;}
}


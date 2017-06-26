/*
 * LED.c
 *
 * Created: 7/5/2011 0:04:45
 *  Author: MMM
 */

#include <avr/io.h>
#include <util/delay.h>
#include "LED.h"

void vLEDInit(void)
{
// Set LED1 as output pin
	LED1_DDR |=	0xFF;
}
void vLEDFreeRun(void)
{
	uint8_t i;

	for (i=0;i<7;i++)
	{
		LED1^= 0xFF;
		_delay_ms(100);
	}
	LED1=0;
	for (i=0;i<7;i++)
	{
		LED1 = 0xAA;
		_delay_ms(100);
		LED1 = 0;
		_delay_ms(100);
		LED1 = 0x55;
		_delay_ms(100);
	}
	LED1=0;
	for (i=0;i<7;i++)
	{
		LED1=(1<<i);
		_delay_ms(100);
	}
	LED1=0;
	for (i=0;i<7;i++)
	{
		LED1=(0x80>>i);
		_delay_ms(100);
	}
	LED1=0;
	for (i=0;i<7;i++)
	{
		LED1|=(1<<i);
		_delay_ms(100);
	}
	LED1=0;
	for (i=0;i<7;i++)
	{
		LED1|=(0X80>>i);
		_delay_ms(100);
	}
	LED1=0;
}

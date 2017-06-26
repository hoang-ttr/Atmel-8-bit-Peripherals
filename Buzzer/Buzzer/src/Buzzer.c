/*
 * buzzer.c
 *
 * Created: 04/16/17 09:53:53 PM
 *  Author: Hoang Thien Tran
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "Buzzer.h"

void Buzzer_Init(void)
{
	Buzzer_DDR |=(1<<Buzzer_Pin); //Buzzer pin0 as OUTPUT
	Buzzer = 1; //Buzzer active LOW
}
void BuzzerRun(void)
{
	uint8_t i;
	double period;

	period = 1000 / (2*freq);	// 1000ms, ON duty is 50%
	for(i=0; i<freq; i++)
	{
		Buzzer ^= (1<<Buzzer_Pin);
		_delay_ms(period);
	}
	Buzzer |= (1<<Buzzer_Pin);
}
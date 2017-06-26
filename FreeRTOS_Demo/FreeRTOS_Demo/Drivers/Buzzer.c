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
	Buzzer_DDR |= (1<<Buzzer_Pin); //Buzzer pin0 as OUTPUT
	Buzzer |= (1<<Buzzer_Pin); //Buzzer active LOW
}
/*
 * Buzzer.c
 *
 * Created: 02/07/17 02:13:46 PM
 * Author : Hoang Thien Tran
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "src/Buzzer.h"
#include "src/Button.h"

int main(void)
{
	Buzzer_Init();
	DDRB &= ~(1<<7);
	PORTB |= (1<<7);

	while (1)
    {
		if (ButtonPressed(PINB, 7))
		BuzzerRun();
	}
}


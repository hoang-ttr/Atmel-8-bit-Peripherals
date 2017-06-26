/*
 * Button.h
 *
 * Created: 03/13/17 03:42:46 PM
 *  Author: Hoang Thien Tran
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

#define ButtonP 7
#define ButtonPin PINB
#define ButtonPullup PORTB
#define Button_DDR DDRB

volatile uint8_t button_down;

void ButtonInit(void);

static inline void debounce(void)
{
	static uint8_t count = 0;	// Counter for number of equal states
	static uint8_t button_state = 0;	// Keeps track of current (debounced)state
	uint8_t current_state = (~ButtonPin & (1<<ButtonP));	// Check if button is high or low for the moment

	if (current_state != button_state)
	{
		count++;		// Button state is about to be changed, increase counter
		if (count >= 5)
		{
			button_state = current_state;	// The button have not bounced for four checks, change state
			if (current_state != 0)			// If the button was pressed (not released), tell main so
			{
				button_down = 1;
			}
			count = 0;
		}
	}
	else
	{
		count = 0;		// Reset counter
	}
}

#endif
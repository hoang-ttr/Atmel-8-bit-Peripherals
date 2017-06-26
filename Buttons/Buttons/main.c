/*
 * Buttons.c
 *
 * Created: 09/22/16 02:38:16 PM
 * Author : Hoang Thien Tran
 */

#define F_CPU 16000000UL

#include "src/Button.h"

#define LED PORTC
#define LED_DDR DDRC

int main(void)
{
	ButtonInit();
	LED_DDR = 0xFF;		//LED as output

	while (1)
	{
		debounce();
		if (button_down)
		{
			button_down =  0;
			LED ^= (1 << 0);
		}
	}
}
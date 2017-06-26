/*
 * Button.h
 *
 * Created: 03/13/17 03:42:46 PM
 *  Author: Hoang Thien Tran
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

#define numberOfButtons 2
#define holdtime 100

uint8_t ButtonPressed(uint8_t pinOfButton, uint8_t portBit)
{
	static uint8_t	pressed=0,
					Pressed_Confidence_Level,		//Measure button press confidence
					Released_Confidence_Level;		//Measure button release confidence

	if (bit_is_clear(pinOfButton, portBit))
	{
		Pressed_Confidence_Level++;					//Increase Pressed Confidence
		Released_Confidence_Level = 0;				//Reset released button confidence since there is a button press
		if (Pressed_Confidence_Level > holdtime)	//Indicator of good button press
		{
			if (pressed == 0)
			{
				pressed = 1;
				return 1;
			}
			Pressed_Confidence_Level = 0;			//Zero it so a new pressed condition can be evaluated
		}
	}
	else
	{
		Released_Confidence_Level++;				//This works just like the pressed
		Pressed_Confidence_Level = 0;				//Reset pressed button confidence since the button is released
		if (Released_Confidence_Level > holdtime)
		{
			if (pressed == 1)
			{
				pressed = 0;
				Released_Confidence_Level = 0;
			}
		}
	}
	return 0;
}
#endif
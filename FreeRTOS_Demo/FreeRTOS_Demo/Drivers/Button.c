/*
 * Button.c
 *
 * Created: 04/17/17 11:08:49 PM
 *  Author: Hoang Thien Tran
 */

#include "Button.h"

void ButtonInit(void)
{
	Button_DDR &= ~(1<<ButtonP);	//Buttons as INPUT
	ButtonPullup |= (1<<ButtonP);	//Enable pull-up
}

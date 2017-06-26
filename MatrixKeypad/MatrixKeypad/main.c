/*
 * MatrixKeypad.c
 *
 * Created: 02/07/17 04:43:46 PM
 * Author : Hoang Thien Tran
 */ 

#define F_CPU 16000000UL

#include "src/lcd4bit.h"
#include "src/Keypad.h"

int main(void)
{
	char keypad;
	Lcd4_Ini();
	Lcd4_Clear();
	Keypad_Ini();
	//
	Lcd4_Cursor(1,1);
	Lcd4_SetString("Key pressed:");
	Lcd4_Cursor(2,1);
	while (1)
	{
		keypad=pressKEY();
		if(pressed)
		{
			Lcd4_SetChar(keypad);
			pressed = 0;
		}
		
	}
}
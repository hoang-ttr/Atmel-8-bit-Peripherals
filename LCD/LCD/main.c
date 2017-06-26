/*
 * LCD.c
 *
 * Created: 02/07/17 02:51:47 PM
 * Author : Hoang Thien Tran
 */ 

#include "src/lcd4bit.h" //Choose between 8bit interface or 4bit interface

int main(void)
{
	Lcd4_Ini();
	Lcd4_Clear();
	Lcd4_Cursor(1,2);
	Lcd4_SetString("This is a test");
	Lcd4_Cursor(2,1);
	Lcd4_SetNum(12);
	while(1){;}
}

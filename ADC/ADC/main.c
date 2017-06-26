/*
 * ADC.c
 *
 * Created: 02/07/17 04:27:54 PM
 * Author : Hoang Thien Tran
 */

#include "src/ADCIni.h"
#include "src/lcd4bit.h"

int main(void)
{
	ADCIni();
	Lcd4_Ini();
	Lcd4_Clear();

	Lcd4_Cursor(1,1);
	Lcd4_SetString("ADC val:");
	while (1)
	{
		readAD(0);
		Lcd4_Cursor(2,1);
		Lcd4_SetNum(ADCresult);
		readAD(1);
		Lcd4_Cursor(2,11);
		Lcd4_SetNum(ADCresult);
		_delay_ms(1);
	}
}

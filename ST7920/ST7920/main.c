/*
 * ST7920.c
 *
 * Created: 02/28/17 10:09:59 AM
 * Author : Hoang Thien Tran
 */ 
#define F_CPU 16000000UL

#include "src/ST7920.h"

int main(void)
{
	int i, j, k;
	char string1[16] = "-----ST7920-----";
	char string3[16] = "---Character----";
	char string2[16] = "------Mode------";
	char string4[16] = "---TTHoang-2017-";

	GLCD_Char_Ini();    // initialize as character display
	GLCD_Clear();    // clear character screen and reset address to 0

	for (i = 0; i < 16; ++i)	GLCD_Data(string1[i]);
	for (i = 0; i < 16; ++i)	GLCD_Data(string2[i]);
	for (i = 0; i < 16; ++i)	GLCD_Data(string3[i]);
	for (i = 0; i < 16; ++i)	GLCD_Data(string4[i]);

	_delay_ms(1000);
	GLCD_Clear();

	GLCD_Graphic_Ini();
	GLCD_Graphic_Fill(0);
	
	_delay_ms(1000);

	// draw a graph with x and y axis
	write_char_xy(7, 59, 0);
	write_gnumber_xy(113, 59, 100);
	write_char_xy(0, 0, 9);
	write_char_xy(0, 53, 0);
	GLCD_Graph_Yaxis(6,57,121);
	GLCD_Graph_Xaxis(6, 0, 57);

	// imitate a plot
	k=32;
	for (i=7; i<128; i++)
	{
		j=rand() % 4;
		GLCD_Graph_Pixel(i, k);
		if (j == 1)	k=k-1;
		if (j > 1)	k=k+1;
	}
}
/*
 * lcd4bit.c
 *
 * Created: 03/16/17 01:58:39 PM
 *  Author: Hoang Thien Tran
 */

#include "stdlib.h"
#include "lcd4bit.h"

//----------Local functions prototypes----------//
static void Lcd4_Write_Instruction();
static void Lcd4_Write_Data();
static void Lcd4_Execute(uint8_t data);				//Load data to the LCD
static void Lcd4_Cmd(uint8_t cmd);						//Send a command

//----------Main functions for use----------//
void Lcd4_Clear()								//Clear screen
{
	Lcd4_Cmd(1);
	_delay_ms(2);
}
void Lcd4_Cursor(uint8_t row, uint8_t column)	//Set DDRAM(cursor) address --- 0b1(ADD6:0)
{
	char temp;
	if(row == 1)
	{
		temp = 0b10000000 + (column-1);
		Lcd4_Cmd(temp);
	}
	else if(row == 2)
	{
		temp = 0b11000000 + (column-1);
		Lcd4_Cmd(temp);
	}
}
void Lcd4_Init(void)								//Initialize LCD
{
	LCD_DDR = 0xFF;
	//----------Reset process from data sheet----------//
	_delay_ms(20);	//Wait for more than 15ms after VCC=4.5V
	Lcd4_Cmd(0x03);
	_delay_ms(5);	//Wait for more than 4.1ms
	Lcd4_Cmd(0x03);
	_delay_ms(1);	//Wait for more than 100us
	Lcd4_Cmd(0x03);
	//--------------------//
	Lcd4_Cmd(0x02);	//Function set: 4bit
	Lcd4_Cmd(0x28);	//Function set: 4bit, 2 lines, 5x8 font		---	0b001DNF** (D:data length,N:no. of line,F:font)
	Lcd4_Cmd(0x0C); //Display control: display ON	--- 0b00001DCB (D:display,C:cursor,B:blinking)
	Lcd4_Cmd(0x06); //Entry mode set: increment, no accompany	--- 0b000001IS (I:increment/decrement,S:acc. display shift)
}
void Lcd4_SetChar(char character)				//Send a single character
{
	Lcd4_Write_Data();

	Lcd4_Execute(character>>4); //Data transfer
	Lcd4_Execute(character&0x0F);
}
void Lcd4_SetString(char *pStr)					//Send a string
{
	while(*pStr>0) //Whenever pointer gets to the end, *pStr = 0
	Lcd4_SetChar(*pStr++); //Send individual char and increase pointer address
}
void Lcd4_SetNum(uint16_t integer)				//Send a number as string (max 4 digits)
{
	char intStr[5];

	itoa(integer, intStr, 10);
	if (integer<10)		{Lcd4_SetString("000");}
	else if (integer<100)	{Lcd4_SetString("00");}
	else if (integer<1000)	{Lcd4_SetString("0");}
	Lcd4_SetString(intStr);
}
//----------Local functions----------//
static void Lcd4_Write_Instruction()
{
	LCD_DATA &= ~RS;
	_delay_us(delay_time);
	LCD_DATA &= ~RW;
	_delay_us(delay_time);
}
static void Lcd4_Write_Data()
{
	LCD_DATA |= RS;
	_delay_us(delay_time);
	LCD_DATA &= ~RW;
	_delay_us(delay_time);
}
static void Lcd4_Execute(uint8_t data)					//Load data to the LCD
{
	if(data & 1) LCD_DATA|=(1<<4);	//1 = 0b00000001
	else LCD_DATA&=~(1<<4);
	if(data & 2) LCD_DATA|=(1<<5);	//1 = 0b00000010
	else LCD_DATA&=~(1<<5);
	if(data & 4) LCD_DATA|=(1<<6);	//1 = 0b00000100
	else LCD_DATA&=~(1<<6);
	if(data & 8) LCD_DATA|=(1<<7);	//1 = 0b00001000
	else LCD_DATA&=~(1<<7);

	LCD_DATA |= E;	//Execute instruction
	_delay_us(delay_time);
	LCD_DATA &= ~E;
	_delay_us(delay_time);

}
static void Lcd4_Cmd(uint8_t cmd)						//Send a command
{
	Lcd4_Write_Instruction();
	Lcd4_Execute(cmd>>4);
	Lcd4_Execute(cmd&0x0F);
}
/*
 * lcd8bit.c
 *
 * Created: 03/16/17 02:00:53 PM
 *  Author: Hoang Thien Tran
 */

#include "lcd8bit.h"

//----------Local functions prototypes----------//
static void Lcd8_Write_Instruction();
static void Lcd8_Write_Data();
static void Lcd8_Execute(uint8_t data);			//Load data to the LCD
static void Lcd8_Cmd(uint8_t cmd);					//Send cmd to the LCD

//----------Main functions for use----------//
void Lcd8_Clear()
{
	Lcd8_Cmd(1);
}
void Lcd8_Cursor(char row, char column) //Set DDRAM(cursor) address --- 0b1(ADD6:0)
{
	if(row == 1)
	Lcd8_Cmd(0b10000000 + column);
	else if(row == 2)
	Lcd8_Cmd(0x11000000 + column);
}
void Lcd8_Init() //Initialize LCD
{
	LCD_DATA_DDR=0xFF;
	LCD_CTL_DDR=0x07;
	///////////// Reset process from data sheet /////////
	_delay_ms(20);
	Lcd8_Cmd(0x30);
	_delay_ms(5);
	Lcd8_Cmd(0x30);
	_delay_ms(1);
	Lcd8_Cmd(0x30);
	/////////////////////////////////////////////////////
	Lcd8_Cmd(0x38);	//Function set: 8bit, 2 lines, 5x8 font		---	0b001DNF** (D:data length,N:no. of line,F:font)
	Lcd8_Cmd(0x0C); //Display control: display ON	--- 0b00001DCB (D:display,C:cursor,B:blinking)
	Lcd8_Cmd(0x06); //Entry mode set: increment, no accompany	--- 0b000001IS (I:increment/decrement,S:acc. display shift)
}
void Lcd8_SetChar(char character)
{
	Lcd8_Write_Data();
	Lcd8_Execute(character); //Data transfer
}
void Lcd8_SetString(char *pStr)//Send a string
{
	while(*pStr>0) //Whenever pointer gets to the end, *pStr = 0
	Lcd8_SetChar(*pStr++); //Send individual char and increase pointer address
}

//----------Local functions----------//
static void Lcd8_Write_Instruction()
{
	LCD_CTL &= ~RS;
	_delay_us(delay_time);
	LCD_CTL &= ~RW;
	_delay_us(delay_time);
}
static void Lcd8_Write_Data()
{
	LCD_CTL |= RS;
	_delay_us(delay_time);
	LCD_CTL &= ~RW;
	_delay_us(delay_time);
}
static void Lcd8_Execute(uint8_t data)					//Load data to the LCD
{
	if(data & 1) LCD_DATA|=(1<<0);		//0b00000001
	else LCD_DATA&=~(1<<0);
	if(data & 2) LCD_DATA|=(1<<1);		//0b00000010
	else LCD_DATA&=~(1<<1);
	if(data & 4) LCD_DATA|=(1<<2);		//0b00000100
	else LCD_DATA&=~(1<<2);
	if(data & 8) LCD_DATA|=(1<<3);		//0b00001000
	else LCD_DATA&=~(1<<3);
	if(data & 16) LCD_DATA|=(1<<4);		//0b00010000
	else LCD_DATA&=~(1<<4);
	if(data & 32) LCD_DATA|=(1<<5);		//0b00100000
	else LCD_DATA&=~(1<<5);
	if(data & 64) LCD_DATA|=(1<<6);		//0b01000000
	else LCD_DATA&=~(1<<6);
	if(data & 128) LCD_DATA|=(1<<7);	//0b10000000
	else LCD_DATA&=~(1<<7);

	LCD_CTL |= E;	//Execute instruction
	_delay_us(delay_time);
	LCD_CTL &= ~E;
	_delay_us(delay_time);

}
static void Lcd8_Cmd(uint8_t cmd)						//Send cmd to the LCD
{
	Lcd8_Write_Instruction();
	Lcd8_Execute(cmd);
}
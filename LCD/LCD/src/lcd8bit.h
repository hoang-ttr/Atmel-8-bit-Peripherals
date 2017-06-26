/*
 * lcd8bit.h
 *
 * Created: 02/07/17 02:51:47 PM
 * Author : Hoang Thien Tran
 */

#ifndef LCD8BIT_H_
#define LCD8BIT_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DATA		PORTD	//Choose the port that suits your needs
#define LCD_CTL			PORTC
#define LCD_DATA_DDR	DDRD
#define LCD_CTL_DDR		DDRC
#define RS	1					//PB0  binary equivalent of pin number
#define RW	2					//PB1
#define E	4					//PB2

#define delay_time 80			//Default delay time for most instructions


//----------Main functions for use----------//
void Lcd8_Clear();
void Lcd8_Cursor(char row, char column);	//Set DDRAM(cursor) address --- 0b1(ADD6:0)
void Lcd8_Init();							//Initialize LCD
void Lcd8_SetChar(char character);			//Send a character
void Lcd8_SetString(char *pStr);			//Send a string

#endif
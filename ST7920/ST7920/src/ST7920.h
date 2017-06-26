
/*
 * st7920.h
 *
 *  Created on: Apr 19, 2013
 *      Author: Peter Barnes
 *
 *      Use and distribute freely for non-commercial uses, but please
 *      include this file and my name if you distribute or post it. Enjoy!
 *
 */

#ifndef ST7920_H_
#define ST7920_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define DATA_OUT		PORTB
#define DATA_IN			PINB
#define COMMAND_OUT		PORTC
#define DATA_DDR		DDRB
#define COMMAND_DDR		DDRC
#define RS	1					//PC0  binary equivalent of pin number
#define RW	2					//PC1
#define E	4					//PC2

#define delay_time	80			// default settling time in uS after a command is sent

uint16_t packed_number[4];
uint8_t numbers[12][5];

void GLCD_Write_Instruction(void);
void GLCD_Read_Intruction(void);
void GLCD_Write_Data(void);
void GLCD_Read_Data(void);
void GLCD_Excecute(uint8_t data);

//-------------------Character Mode--------------------//
void GLCD_Char_Ini(void);								//Initialize in character mode
void GLCD_Clear(void);								//Clear the whole GLCD
uint16_t GLCD_Read(uint8_t instruct);				//Read data byte(instruct=1) or command register(instruct=0)
void GLCD_Data(uint8_t data);						//Write a character at the current address			
void GLCD_WordData(uint16_t data);					//Write a word at the current address
void GLCD_Goto(uint8_t x, uint8_t y);				//x=0-7, y=0-63 (for 128 x 64 display)
void GLCD_Write_Word(int x, int y, uint16_t data);	//x=0-7 (8 words per line), y=0-63, starting from top left
void GLCD_CGRAM(uint8_t x, uint8_t y);

//-------------------Graphic Mode--------------------//
void GLCD_Graphic_Ini(void);						//Initialize in graphic mode
void GLCD_Graphic_Fill(uint16_t value);				//Fill with a 16-bit value. Zero clears the screen.
void GLCD_Graph_Pixel(uint8_t x, uint8_t y);
void GLCD_Graph_Pixel_Clr(uint8_t x, uint8_t y);
void write_char(int x, int y, uint8_t num_0, uint8_t num_1, uint8_t num_2, uint8_t num_3);	//Write 4-digit number on word boundary
void write_char_xy(uint8_t x, uint8_t y, uint8_t number);//Write a digit anywhere using set_pixel. Slower, but flexible for placement on the screen
void write_number_xy(uint8_t x, uint8_t y, uint8_t number);		//Write single digit on word boundary (x=0-7)
void write_lnumber_xy(uint8_t x, uint8_t y, uint16_t number);	//Write a (1->4)digit number on a word boundary (0-7)
void write_gnumber_xy(uint8_t x, uint8_t y, uint16_t number);	//Write a 1-4 digit number on any x-y pixel
void GLCD_Graph_Yaxis(int x, int y, int length);
void GLCD_Graph_Xaxis(int x, int y, int length);

#endif
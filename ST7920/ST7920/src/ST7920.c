/*
 * ST7920.c
 *
 * Created: 03/16/17 02:17:21 PM
 *  Author: Hoang Thien Tran
 */ 

#include "ST7920.h"

uint8_t numbers[12][5]=	{								// a minimal 3x5 numeric font
						{0x0e, 0x0a, 0x0a, 0x0a, 0x0e},	// 0
						{0x04, 0x04, 0x04, 0x04, 0x04},	// 1
						{0x0e, 0x02, 0x0e, 0x08, 0x0e},	// 2
						{0x0e, 0x02, 0x0e, 0x02, 0x0e},	// 3
						{0x0a, 0x0a, 0x0e, 0x02, 0x02},	// 4
						{0x0e, 0x08, 0x0e, 0x02, 0x0e},	// 5
						{0x0e, 0x08, 0x0e, 0x0a, 0x0e}, // 6
						{0x0e, 0x02, 0x02, 0x02, 0x02},	// 7
						{0x0e, 0x0a, 0x0e, 0x0a, 0x0e},	// 8
						{0x0e, 0x0a, 0x0e, 0x02, 0x02},	// 9
						{0x00, 0x00, 0x0e, 0x00, 0x00},	// -
						{0x00, 0x00, 0x00, 0x00, 0x00}	// (space)
														};

void GLCD_Write_Instruction(void)
{
	COMMAND_OUT &= ~RS;
	_delay_us(delay_time);

	COMMAND_OUT &= ~RW;
	_delay_us(delay_time);
}
void GLCD_Read_Intruction(void)
{
	COMMAND_OUT &= ~RS;
	_delay_us(delay_time);

	COMMAND_OUT |= RW;
	_delay_us(delay_time);
}
void GLCD_Write_Data(void)
{
	COMMAND_OUT |= RS;
	_delay_us(delay_time);

	COMMAND_OUT &= ~RW;
	_delay_us(delay_time);
}
void GLCD_Read_Data(void)
{
	COMMAND_OUT |= RS;
	_delay_us(delay_time);

	COMMAND_OUT |= RW;
	_delay_us(delay_time);
}
void GLCD_Excecute(uint8_t data)
{
	DATA_OUT = data;    // set 8-bit interface
	
	COMMAND_OUT |= E;
	_delay_us(delay_time);
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
}

//-------------------Character Mode--------------------//
void GLCD_Char_Ini(void)								//Initialize in character mode
{
	DATA_DDR = 0xFF;		//Set data as OUTPUT
	COMMAND_DDR = 0x07;		//Set command as OUTPUT
	
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	GLCD_Write_Instruction();
	
	GLCD_Excecute(0x30);    //Set 8-bit interface
	_delay_ms(1);
	GLCD_Excecute(0x30);    //Set 8-bit interface, basic instruction set
	GLCD_Excecute(0x0C);    //Display ON, cursor ON, blink OFF
	GLCD_Excecute(0x01);    //Clear display, reset address
	GLCD_Excecute(0x06);    //Entry mode: Increment
}
void GLCD_Clear(void)								//Clear the whole GLCD
{
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);

	GLCD_Write_Instruction();

	GLCD_Excecute(0x01);    // command to clear screen and reset address counter
}
uint16_t GLCD_Read(uint8_t instruct)				//Read data byte(instruct=1) or command register(instruct=0)
{
	uint8_t byteH, byteL;
	
	DATA_DDR = 0x00;	//Set data as OUTPUT
	DATA_OUT = 0x00;	//Reset pull-up
	
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	if (instruct == 0)
	{
		GLCD_Read_Intruction();
	}
	else
	{
		GLCD_Read_Data();
	}
	
	COMMAND_OUT |= E;				//Dummy Read
	_delay_us(delay_time);
	byteH = DATA_IN;
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	COMMAND_OUT |= E;				//1st byte
	_delay_us(delay_time);
	byteH = DATA_IN;
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	_delay_ms(1);
	
	COMMAND_OUT |= E;				//2nd Byte
	_delay_us(delay_time);
	byteL = DATA_IN;
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);

	DATA_DDR = 0xFF;		//Set data as OUTPUT again

	return((byteH<<8)|byteL);    // return byte read
}
void GLCD_Data(uint8_t data)						//Write a character at the current address
{
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	GLCD_Write_Data();
	
	GLCD_Excecute(data);	//Write the byte to display at the current address
}
void GLCD_WordData(uint16_t data)					//Write a word at the current address
{
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	GLCD_Write_Data();

	GLCD_Excecute(data>>8);   // write the byte to display at current address
	GLCD_Excecute(data);
}
void GLCD_Goto(uint8_t x, uint8_t y)				//x=0-7, y=0-63 (for 128 x 64 display)
{
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	GLCD_Write_Instruction();
	
	if (y > 31)		//Convert coordinates to weirdly-arranged 128x64 screen (the ST7920 is mapped for 256x32 displays)
	{
		y -= 32;	//Because there are only 31 addressable lines in the ST7920
		x += 8;		//So we overflow x (7 visible bytes per line) to reach the bottom half
	}
	
	//Bit 7 signals that this is a data address write
	GLCD_Excecute((y|=0x80));	//Set vertical DDRAM address
	GLCD_Excecute((x|=0x80));	//Set horizontal DDRAM address
}
void GLCD_Write_Word(int x, int y, uint16_t data)	//x=0-7 (8 words per line), y=0-63, starting from top left
{
	GLCD_Goto(x,y);					//First set the address
	
	COMMAND_OUT |= RS;				//Change to write data
	_delay_us(delay_time);
	
	GLCD_Excecute(data>>8);			//Load 1st DDRAM data
	GLCD_Excecute(data);			//Load 2nd DDRAM data
}
void GLCD_CGRAM(uint8_t x, uint8_t y)
{
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);

	GLCD_Write_Instruction();
	
	GLCD_Excecute(0x40|x);
	GLCD_Excecute(0x40|y);
}

//-------------------Graphic Mode--------------------//
void GLCD_Graphic_Ini(void)								//Initialize in graphic mode
{
	DATA_DDR = 0xFF;		//Set data as OUTPUT
	COMMAND_DDR = 0x07;		//Set command as OUTPUT
	
	COMMAND_OUT &= ~E;
	_delay_us(delay_time);
	
	GLCD_Write_Instruction();
	
	GLCD_Excecute(0x30);    //Set 8-bit interface
	_delay_ms(1);
	GLCD_Excecute(0x36);    //Set 8-bit interface, extended instruction set, graphic mode on
	GLCD_Excecute(0x0C);    //Display ON, cursor ON, blink OFF
	GLCD_Excecute(0x01);    //Clear display, reset address
	GLCD_Excecute(0x06);    //Entry mode: Increment
}
void GLCD_Graphic_Fill(uint16_t value)				//Fill with a 16-bit value. Zero clears the screen.
{
	uint8_t y, x;
	for (y=0;y<64;y++)
	for (x=0;x<8;x++)
	GLCD_Write_Word(x, y, value);
}

void GLCD_Graph_Pixel(uint8_t x, uint8_t y)
{
	uint8_t x_wd, x_pixel;
	uint16_t temp;
	uint16_t dot = 0x8000;    // this will be rotated into it's correct position in the word
	
	x_wd = x / 16;        // find address of word with our pixel (x; 0-7)
	x_pixel = x - (x_wd * 16);        // get the modulo remainder; that's our pixel's position in the word
	
	GLCD_Goto(x_wd, y);
	
	temp = GLCD_Read(1);    // read word from screen at that position
	temp = (temp | (dot >> x_pixel));    // convert x_pixel into a bit position, 0-16
	GLCD_Write_Word(x_wd, y, temp);
}
void GLCD_Graph_Pixel_Clr(uint8_t x, uint8_t y)
{
	uint8_t x_wd, x_pixel;
	uint16_t temp;
	uint16_t dot = 0x7fff;    // this will be rotated into it's correct position in the word
	
	x_wd = x / 16;        // find address of word with our pixel (x; 0-7)
	x_pixel = x - (x_wd * 16);        // get the modulo remainder; that's our pixel's position in the word
	
	GLCD_Goto(x_wd, y);
	
	temp = GLCD_Read(1);    // read word from screen at that position
	temp = (temp & (dot >> x_pixel));    // convert x_pixel into a bit position, 0-16
	GLCD_Write_Word(x_wd, y, temp);
}
void write_char(int x, int y, uint8_t num_0, uint8_t num_1, uint8_t num_2, uint8_t num_3)	//Write 4-digit number on word boundary
{
	int j;
	uint16_t temp=0, packed_number[5];
	
	for (j = 0; j < 5; j++)
	{
		temp = (numbers[num_0][j]);
		temp = temp << 4;
		temp |= (numbers[num_1][j]);
		temp = temp << 4;
		temp |= (numbers[num_2][j]);
		temp = temp << 4;
		temp |= (numbers[num_3][j]);
		packed_number[j] = temp;
	}
	
	for (j = 0; j < 5; j++)
	GLCD_Write_Word (x,y+j, packed_number[j]);
}
void write_char_xy(uint8_t x, uint8_t y, uint8_t number)//Write a digit anywhere using set_pixel. Slower, but flexible for placement on the screen
{
	uint8_t h=0, i, j, l, m = x, k = 0x08;
	for (i=0; i<5; i++)
	{
		l = (numbers[number][h]);
		for (j = 0; j < 3; j++)
		{
			if (l & k)
			{
				GLCD_Graph_Pixel(m, y);
			}
			m++;
			k /=2;
		}
		h++;
		y++;
		m=x;
		k=0x08;
	}
}
void write_number_xy(uint8_t x, uint8_t y, uint8_t number)		//Write single digit on word boundary (x=0-7)
{
	uint8_t i=0;
	uint16_t temp=0;
	
	for (i = 0; i < 5; i++)
	{
		temp = numbers[number][i];
		temp = temp << 12;
		GLCD_Write_Word(x, (y + i), temp);
	}
}
void write_lnumber_xy(uint8_t x, uint8_t y, uint16_t number)	//Write a (1->4)digit number on a word boundary (0-7)
{
	uint8_t j[4];    // the individual digits, from left to right
	
	if (number > 999)
	{
		j[0] = number / 1000;
		number = number - (1000 * j[0]);
	}
	else
	{
		j[0] = 0x0b;	// blank first zero if present
	}

	if (number > 99)
	{
		j[1] = number / 100;
		number = number - (100 * j[1]);
	}
	else
	{
		j[1] = 0;
	}

	if (number > 9)
	{
		j[2] = number / 10;
		number = number - (10 * j[2]);
	}
	else
	{
		j[2] = 0;
	}
	
	j[3] = number;        // at this point j[] has 4 separate decimal digits

	write_char(x, y, j[0], j[1], j[2], j[3]);
}
void write_gnumber_xy(uint8_t x, uint8_t y, uint16_t number)	//Write a 1-4 digit number on any x-y pixel
{
	uint8_t j[4];    // the individual digits, from left to right
	
	if (number > 999)
	{
		j[0] = number / 1000;
		number = number - (1000 * j[0]);
	}
	else
	{
		j[0] = 0x0b;	// blank first zero if present
	}

	if (number > 99)
	{
		j[1] = number / 100;
		number = number - (100 * j[1]);
	}
	else
	{
		j[1] = 0;
	}

	if (number > 9)
	{
		j[2] = number / 10;
		number = number - (10 * j[2]);
	}
	else
	{
		j[2] = 0;
	}
	
	j[3] = number;        // at this point j[] has 4 separate decimal digits

	write_char_xy(x, y, j[0]);
	x=x+4;
	write_char_xy(x, y, j[1]);
	x=x+4;
	write_char_xy(x, y, j[2]);
	x=x+4;
	write_char_xy(x, y, j[3]);
}
void GLCD_Graph_Yaxis(int x, int y, int length)
{
	int i;
	
	for (i=0; i<length; i++)
	{
		GLCD_Graph_Pixel(x, y);
		x++;
	}
}
void GLCD_Graph_Xaxis(int x, int y, int length)
{
	int i;
	
	for (i=0; i<length; i++)
	{
		GLCD_Graph_Pixel(x, y);
		y++;
	}
}
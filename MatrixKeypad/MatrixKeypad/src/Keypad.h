/*
 * Keypad.h
 *
 * Created: 10/18/16 09:41:50 AM
 *  Author: Hoang Thien Tran
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define Keypad_In PINA
#define Keypad_Pullup PORTA
#define Keypad_DDR DDRA

uint8_t scanCODE[4] = {0x0E,0x0D,0x0B,0X07};
uint8_t pressed;

char ascii[4][4]={	{'7','8','9','/'},
					{'4','5','6','*'},
					{'1','2','3','-'},
					{'N','0','=','+'}	};

void Keypad_Ini(void)		//Initialization for keypad
{
	Keypad_DDR = 0xF0;		//For Keypad, C.ABCD=input, R.EFGH=output
	Keypad_Pullup = 0x0F;	//C.ABCD=pull-up, R.EFGH=0
}

char pressKEY(void)			//Polling for a key
{
	uint8_t column,row,val,key;
	if(Keypad_In!=0x0F) //Check if any key is pressed
	{
			val = Keypad_In & 0x0F; //Store the column
			for (row=4;row<=7;row++)
			{
				Keypad_Pullup = 0xFF - (1<<row); //Scan each row EFGH
				_delay_us(1);	//IMPORTANT: changing PORT and reading PIN need delay time
				key = Keypad_In & 0x0F; // Store scanned row EFGH
				if(key==val) //Check if the right key is pressed
					for (column=0;column<=3;column++)
						if(key==scanCODE[column])
						{
							Keypad_Pullup=0x0F;
							pressed = 1;
							_delay_ms(300);
							return ascii[row-4][column];
						}
			}
	}
	return 0;
}

#endif
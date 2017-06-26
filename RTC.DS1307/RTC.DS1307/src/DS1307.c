/*
 * DS1307.c
 *
 * Created: 03/16/17 02:31:37 PM
 *  Author: Hoang Thien Tran
 */ 

#include "DS1307.h"

volatile uint8_t Sec=0,Min=8,Hour=16,Day=1,Date=11,Month=3,Year=17,Mode=1,AmPm=0;
volatile uint8_t counter=0;

void RTCIni()	//Initialize TWI for DS1307
{
	TWBR = 103;							//Bit rate
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));	//Set pre-scaler=1
	}

char RTCWrite(uint8_t initial_address,volatile uint8_t *data, uint8_t length)	//Write to a desired blocks of bytes
{
	uint8_t i;
	do
	{
		TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Reset INT flag, toggle STArt Condition, TWI ENable
		while(!((TWCR>>TWINT)&1));					//Poll Till Done (till TWINT=1)
		if((TWSR & 0xF8) != 0x08)					//Check status if STArt Condition is sent
			return 0;

		TWDR=0b11010000;							//Write SLA+W DS1307 (Device address is 1101)
		
		TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer (Clear TWINT, TWI ENable)
		while(!((TWCR>>TWINT)&1));					//Poll Till Done
	}
	while((TWSR & 0xF8) != 0x18);					//Looping till receiving ACK from Slave successfully

	TWDR=initial_address;							//Write address for modification
	TWCR=(1<<TWINT)|(1<<TWEN);						//Initiate Transfer
	while(!((TWCR>>TWINT)&1));						//Poll Till Done
	if((TWSR & 0xF8) != 0x28)						//Check status if SLA is sent
		return 0;
	
	for(i=0;i<length;i++)
	{
		TWDR=data[i];								//Write data
		TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer
		while(!((TWCR>>TWINT)&1));					//Poll Till Done
		if((TWSR & 0xF8) != 0x28)					//Check status
			return 0;
	}

	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);			//Put STOp Condition on bus

	return 1;										//Return TRUE - Write successfully
}

char RTCRead(uint8_t initial_address, uint8_t length)	//---Random address bit---
{
	uint8_t i;

	do	//Master acts as Sender to send out a slave address
	{
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Reset INT flag, toggle STArt Condition, TWI ENable
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
		if((TWSR & 0xF8) != 0x08)				//Check status
			return 0;

		TWDR=0b11010000;						//Write SLA+W DS1307 (Device address is 1101)
		TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	}
	while((TWSR&0xF8) != 0x18);					//Looping till receiving ACK from Slave successfully

	TWDR=initial_address;						//Write address for modification
	TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer
	while(!((TWCR>>TWINT)&1));					//Poll Till Done
	if((TWSR & 0xF8) != 0x28)					//Check status if memory address is sent
		return 0;

	//*************************DUMMY WRITE SEQUENCE END **********************

	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);		//Put Re-Start Condition on TWI Bus
	while(!(TWCR & (1<<TWINT)));				//Poll Till Done
	if((TWSR & 0xF8) != 0x10)					//Check status
		return 0;

	TWDR=0b11010001;							//Write SLA+R DS1307 (Device address is 1101)
	TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer
	while(!(TWCR & (1<<TWINT)));				//Poll Till Done
	if((TWSR & 0xF8) != 0x40)					//Check status
		return 0;
	
	for(i=0;i<length-1;i++)
	{
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);	//Now enable Reception of data by clearing TWINT
		while(!(TWCR & (1<<TWINT)));			//Wait till done
		if((TWSR & 0xF8) != 0x50)				//Check status
			return 0;
		timevalue[i]=TWDR;						//Read the first length data and store in "encoded"
	}
	
	TWCR=(1<<TWINT)|(1<<TWEN);					//Now enable Reception of data by clearing TWINT
	while(!(TWCR & (1<<TWINT)));				//Wait till done
	if((TWSR & 0xF8) != 0x58)					//Check status
		return 0;
	timevalue[length-1]=TWDR;						//Read the last data and store in "encoded"

	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);		//Put Stop Condition on bus

	return 1;									//Successfully done
}

uint8_t BCD2Dec(uint8_t BCD)
{
	uint8_t Low,High;
	Low = BCD&0x0F;
	High = (BCD>>4)*10;
	return (High+Low);
}

uint8_t Dec2BCD(uint8_t Dec)
{
	uint8_t Low,High;
	Low = Dec%10;
	High = (Dec/10)<<4;
	return (High+Low);
}

void Display()
{
	Sec = BCD2Dec(timevalue[0]&0x7F);
	Min = BCD2Dec(timevalue[1]&0x7F);
	if(Mode!=0) Hour = BCD2Dec(timevalue[2]&0x1F);	//Mode 12h
	else		Hour = BCD2Dec(timevalue[2]&0x3F);	//mode 24h
	Date = BCD2Dec(timevalue[4]);
	Month = BCD2Dec(timevalue[5]);
	Year = BCD2Dec(timevalue[6]);
	
	Lcd4_Clear();
	
	Lcd4_SetString("Time: ");
	sprintf(displayLCD,"%i",Hour);
	Lcd4_Cursor(1,7);	if(Hour<10)	Lcd4_SetChar(48);	Lcd4_SetString(displayLCD);	Lcd4_Cursor(1,9);	Lcd4_SetChar(45);
	sprintf(displayLCD,"%i",Min);
	Lcd4_Cursor(1,10);	if(Min<10)	Lcd4_SetChar(48);	Lcd4_SetString(displayLCD);	Lcd4_Cursor(1,12);	Lcd4_SetChar(45);
	sprintf(displayLCD,"%i",Sec);
	Lcd4_Cursor(1,13);	if(Sec<10)	Lcd4_SetChar(48);	Lcd4_SetString(displayLCD);
	if (Mode!=0)
	{
		Lcd4_Cursor(1,15);
		if(bit_is_set(timevalue[2],5))	Lcd4_SetString("PM");	//"PM"
		else Lcd4_SetString("AM");	//"AM"
	}
	
	Lcd4_Cursor(2,1);
	Lcd4_SetString("Date: ");
	sprintf(displayLCD,"%i",Date);
	Lcd4_Cursor(2,7);	if(Date<10)	Lcd4_SetChar(48);	Lcd4_SetString(displayLCD);	Lcd4_Cursor(2,9);	Lcd4_SetChar(45);
	sprintf(displayLCD,"%i",Month);
	Lcd4_Cursor(2,10);	if(Month<10)	Lcd4_SetChar(48);	Lcd4_SetString(displayLCD);	Lcd4_Cursor(2,12);	Lcd4_SetChar(45);
	Lcd4_SetString("20");
	sprintf(displayLCD,"%i",Year);
	Lcd4_Cursor(2,15);	if(Year<10)	Lcd4_SetChar(48);	Lcd4_SetString(displayLCD);	
}

ISR(TIMER0_OVF_vect)		//update the display every second
{
	counter++;
	if (counter>=10)
	{
		RTCRead(0x00,7);
		if (BCD2Dec(timevalue[0])!=Sec)
		{
			Sec=BCD2Dec(timevalue[0]&0x7F);
			sprintf(displayLCD,"%i",Sec);
			if (Sec<10)
			{
				Lcd4_Cursor(1,13);	Lcd4_SetChar(48);
				Lcd4_Cursor(1,14);	Lcd4_SetChar(45);
				Lcd4_Cursor(1,14);	Lcd4_SetString(displayLCD);
			}
			else
			{
				Lcd4_Cursor(1,13);	Lcd4_SetChar(45);
				Lcd4_Cursor(1,13);	Lcd4_SetString(displayLCD);
			}
			if(Sec==0) Display();
		}
		counter=0;
	}
}
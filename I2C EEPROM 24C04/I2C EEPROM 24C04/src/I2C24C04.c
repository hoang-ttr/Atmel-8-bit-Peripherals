/*
 * I2C24C04.c
 *
 * Created: 03/16/17 02:29:08 PM
 *  Author: Hoang Thien Tran
 */ 

#include "I2C24C04.h"

void EEIni(void)	//Initialize TWI for 24C04
{
	TWBR = 103;	//Bit rate
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));	//Set pre-scaler=1
}

char EEWriteByte(uint8_t initial_address, uint8_t data[])	//With WP=VIL, Page-write is activated, Maximum bytes to sent is 8 
{
	uint8_t i;
	do
	{
		TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Reset INT flag, toggle STArt Condition, TWI ENable
		while(!((TWCR>>TWINT)&1));					//Poll Till Done (till TWINT=1)
		if((TWSR & 0xF8) != 0x08)					//Check status if STArt Condition is sent
			return 0;

		TWDR=0b10100000;						//Write SLA+W EEPROM (Device address is 1010, also a Block Select at bit 1 (1 block = 256 address)
		TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer (Clear TWINT, TWI ENable)
		while(!((TWCR>>TWINT)&1));				//Poll Till Done
	}
	while((TWSR & 0xF8) != 0x18);			//Looping till receiving ACK from Slave successfully

	TWDR=initial_address;					//Write memory address
	TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
	while(!((TWCR>>TWINT)&1));				//Poll Till Done
	if((TWSR & 0xF8) != 0x28)				//Check status if SLA is sent
		return 0;
	
	for(i=0;i<=7;i++)
	{
		TWDR=data[i];							//Write data
		TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
		while(!((TWCR>>TWINT)&1));				//Poll Till Done
		if((TWSR & 0xF8) != 0x28)				//Check status
			return 0;
	}

	if((TWSR & 0xF8) != 0x28)				//Check status if data is sent
		return 0;

	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	//Put STOp Condition on bus

	return 1;							//Return TRUE - Write successfully
}

uint8_t EEReadByte(uint8_t initial_address)	//---Random address bit---
{
	uint8_t data;

	do	//Master acts as Sender to send out a slave address
	{
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Reset INT flag, toggle STArt Condition, TWI ENable
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
		if((TWSR & 0xF8) != 0x08)				//Check status
			return 0;

		TWDR=0b10100000;	//Write SLA+W EEPROM (Device address is 1010, also a Block Select at bit 1 (1 block = 256 address)
		TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	}
	while((TWSR&0xF8) != 0x18);//Looping till receiving ACK from Slave successfully

	TWDR=initial_address;					//Write memory address
	TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
	while(!((TWCR>>TWINT)&1));				//Poll Till Done
	if((TWSR & 0xF8) != 0x28)				//Check status if memory address is sent
		return 0;

	//*************************DUMMY WRITE SEQUENCE END **********************

	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Put Re-Start Condition on TWI Bus
	while(!(TWCR & (1<<TWINT)));			//Poll Till Done
		if((TWSR & 0xF8) != 0x10)			//Check status
		return 0;

	TWDR=0b10100001;				//Write SLA+R EEPROM (Device address is 1010, also a Block Select at bit 1 (1 block = 256 address)
	TWCR=(1<<TWINT)|(1<<TWEN);		//Initiate Transfer
	while(!(TWCR & (1<<TWINT)));	//Poll Till Done
	if((TWSR & 0xF8) != 0x40)		//Check status
		return 0;
	
	TWCR=(1<<TWINT)|(1<<TWEN);		//Now enable Reception of data by clearing TWINT
	while(!(TWCR & (1<<TWINT)));	//Wait till done
	if((TWSR & 0xF8) != 0x58)		//Check status
		return 0;
	data=TWDR;						//Read the data

	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	//Put Stop Condition on bus

	return data;	//Return the data
}
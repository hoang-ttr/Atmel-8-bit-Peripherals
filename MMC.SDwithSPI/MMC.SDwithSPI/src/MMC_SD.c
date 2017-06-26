/*
 * MMC_SD.c
 *
 * Created: 03/16/17 02:40:47 PM
 *  Author: Hoang Thien Tran
 */
#include "lcd4bit.h"
#include "MMC_SD.h"
#include <stdlib.h>

char Card_Status=0; //Busy flag for MMC/SD

void SPI_MasterIni(void)								//Initialize SPI for Master AVR
{
	SPI_DDR |= (1<<SCK)|(1<<MOSI)|(1<<SS);	//SCK, MOSI, SS as OUTPUT
	SPI_DDR &= ~(1<<MISO);					//MISO as INPUT
	SPCR |= (1<<SPIE)|(1<<SPE)|(1<<MSTR);	//Interrupt enable, SPI enable, Master mode, MSB first, CPOL=0, CPHA=0
}

void SPI_SendByte(uint8_t data)						//Sent byte to slave
{
	SPDR=data;
	while(bit_is_clear(SPSR,SPIF)){;}	//Loop until SPIF is set - The process is complete
}

uint8_t SPI_ReadByte(void)								//Read byte from slave
{
	uint8_t data;

	SPDR=0xFF;							//dummy data
	while(bit_is_clear(SPSR,SPIF)){;}	//Loop until SPIF is set - The process is complete
	data=SPDR;
	return data;
}

uint8_t Card_Respond(uint8_t Respond)				//Acknowledgment from MMC/SD
{
	int timeout=0x0FFF;
	uint8_t Incomming;					//Respond from the card

	while((timeout--)>0)				//count down the time
	{
		Incomming=SPI_ReadByte();
		if(Incomming==Respond)	break;
	}
	if(timeout==0) return 0;			//If timeout
	else return 1;						//If the card responds
}

void Card_Cmd(uint8_t command, uint32_t argument, uint8_t crc)	//Send command to card
{
	SPI_PORT &= ~(1<<SS);					//Wake up the card

	SPI_SendByte(command|0b01000000);		//Start bit "1" + Transmission bit "0" + 6 Command bits

	SPI_SendByte(argument>>24);	//Sending 32-bit argument as 4 8-bit package
	SPI_SendByte(argument>>16);
	SPI_SendByte(argument>>8);
	SPI_SendByte(argument);

	SPI_SendByte(crc);						//CRC7 + Stop bit "1"
}

uint8_t Card_Ini(void)										//Initialize interface with MMC/SD
{
	uint16_t i;
	SPI_MasterIni();

	SPI_PORT |= (1<<SS);						//Put card in waiting
	for(i=0;i<10;i++)	SPI_SendByte(0xFF);		//Send 10*8=80 pulse to wake up card
	SPI_PORT &= ~(1<<SS);						//Activate card in SPI mode

	Card_Cmd(CMD0,0,0x95);						//Reset the card with CMD0
	if (Card_Respond(0x01)==0)					//If card won't respond
	{
		Card_Status=1;							//Set card status for when errors occur or timeout
		SPI_PORT |= (1<<SS);
		return Card_Status;
	}

	Card_Cmd(CMD1,0,0xFF);						//Request the card to enter SPI Mode
	if(Card_Respond(0x00)==0)
	{
		Card_Status=2;
		SPI_PORT |= (1<<SS);
		return Card_Status;
	}

	Card_Cmd(CMD16,Block_Length,0xFF);			//Set block length for the card

	SPI_PORT |= (1<<SS);						//Put the card in waiting
	return 0;
}

uint8_t Card_Write(uint32_t LBAddress, char *buff)		//Write to a sector in MMC/SD
{
	uint8_t i;
	uint32_t tempA;
	tempA=512*LBAddress;
	//char buffercount[4];

	SPI_PORT &= ~(1<<SS);								//Activate card in SPI mode

	Card_Cmd(CMD24,tempA,0x95);							//Request to start writing process
	if (Card_Respond(0x00)==0)
	{
		Card_Status=4;
		SPI_PORT |= (1<<SS);
		return Card_Status;
	}

	SPI_SendByte(0xFE);									//Send data token, ready to transfer data, token=0xFE

	for(i=0;i<Block_Length;i++)
	{
		SPI_SendByte(buff[i]);	//Send the whole block of predetermined bytes
		//itoa(i,buffercount,10);
		//Lcd4_Clear();
		//Lcd4_SetString(buffercount);
		//_delay_ms(50);
	}

	//Lcd4_Clear();
	//Lcd4_SetString("Checksum...");
	SPI_SendByte(0xFF);									//2 checksum bytes
	SPI_SendByte(0xFF);

	if((SPI_ReadByte()&0x0F)!=0x05)						//Check respond from the card, must be 5
	{
		Card_Status=5;
		SPI_PORT |= (1<<SS);
		return Card_Status;
	}

	SPI_PORT |= (1<<SS);								//Put the card in waiting
	return 0;
}

uint8_t Card_Read(uint32_t LBAddress, char *buff)		//Read to a sector in MMC/SD
{
	uint8_t i;
	uint32_t tempA;
	tempA=512*LBAddress;

	SPI_PORT &= ~(1<<SS);								//Activate card in SPI mode

	Card_Cmd(CMD17,tempA,0xFF);							//Request to start reading process
	if (Card_Respond(0x00)==0)
	{
		Card_Status=7;
		SPI_PORT |= (1<<SS);
		return Card_Status;
	}

	if (Card_Respond(0xFE)==0)							//Verify the token received
	{
		Card_Status=8;
		SPI_PORT |= (1<<SS);
		return Card_Status;
	}

	for(i=0;i<Block_Length;i++)	buff[i]=SPI_ReadByte();	//Read the whole block of predetermined bytes

	SPI_SendByte(0xFF);									//2 checksum bytes
	SPI_SendByte(0xFF);

	SPI_PORT |= (1<<SS);								//Put the card in waiting
	return 0;
}
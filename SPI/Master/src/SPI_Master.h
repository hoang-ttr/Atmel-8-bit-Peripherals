/*
 * SPI.h
 *
 * Created: 02/16/17 11:03:52 PM
 *  Author: TRAN THIEN HOANG
 */ 

#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

#define SPI_DDR DDRB		//Choose a port here
#define SPI_PORT PORTB		//Choose a port here
#define SS 4
#define MOSI 5
#define MISO 6
#define SCK 7
#define Slave(i) i		//If using many slaves

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint16_t data_rx;	//Storing received data

void SPIMaster_Ini(void)
{
	SPI_DDR |= (1<<SCK)|(1<<MOSI)|(1<<SS);	//SCK, MOSI as OUTPUT
	SPI_DDR &= ~(1<<MISO);			//MISO as INPUT
	
	SPCR |= (1<<SPIE)|(1<<SPE)|(1<<MSTR);						//INT enable, SPI enable,MSB first, Master mode, SPR1:0=00-prescaler=4
	SPI_PORT |= 1<<SS;		//SS active LOW
}

void SPI_Send(uint8_t ss, uint8_t data)
{
	SPI_PORT &= ~(1<<ss);	//Select a slave
	
	SPDR = data;						//Send the data
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	
	SPI_PORT |= (1<<ss);				//Return the slave to idle status
}

void SPI_Send_9bit(uint8_t ss, uint16_t data)
{
	uint8_t temp=((data>>8)&0x01);	//bit 8
	
	SPI_PORT &= ~(1<<ss);	//Select a slave
	
	//Sending the 9th bit using bit banging process
	{
		SPCR &= ~(1<<SPE);		//Disable SPI
	
			SPI_PORT &= ~(1<<SCK);	//Clock LOW
		
			if (temp)	SPI_PORT |= (1<<MOSI);	//Send the MSB, if it is 1
			else SPI_PORT &= ~(1<<MOSI);
	
			SPI_PORT |= (1<<SCK);	//Clock HIGH
			_delay_us(1);
			SPI_PORT &= ~(1<<SCK);	//Clock LOW
			_delay_us(1);
		SPCR |= (1<<SPE);		//Enable SPI
	}
	
	SPDR = data;						//Send the data
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
		
	SPI_PORT |= (1<<ss);				//Return the slave to idle status
}

ISR(SPI_STC_vect)
{
	data_rx=SPDR;
}

#endif
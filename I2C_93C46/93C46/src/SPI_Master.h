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
	
	SPCR |= (1<<SPE)|(1<<MSTR);						//SPI enable,MSB first, Master mode, SPR1:0=00-prescaler=4
	SPI_PORT |= 1<<SS;		//SS active LOW
}

void _93C46_WriteEnable(void)	//Enable writing, earasing process
{
	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
		SPCR &= ~(1<<SPE);		//Disable SPI
			SPI_PORT &= ~(1<<SCK);	//Clock LOW
			SPI_PORT |= (1<<MOSI);	//Send the start bit
			SPI_PORT |= (1<<SCK);	//Clock HIGH
			_delay_us(1);
			SPI_PORT &= ~(1<<SCK);	//Clock LOW
			_delay_us(1);
		SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = 0b00110000;						//OP Code:00, Address:11xxxx
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
		
	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);
}

void _93C46_WriteDisable(void)	//Disable writing, earasing process
{
	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
	SPCR &= ~(1<<SPE);		//Disable SPI
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	SPI_PORT |= (1<<MOSI);	//Send the start bit
	SPI_PORT |= (1<<SCK);	//Clock HIGH
	_delay_us(1);
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	_delay_us(1);
	SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = 0b00000000;						//OP Code:00, Address:00xxxx
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	
	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);

}

void _93C46_Write(uint8_t address, uint16_t data)			//writing a 16bit data to a 8bit address
{
	_93C46_WriteEnable();

	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
	SPCR &= ~(1<<SPE);		//Disable SPI
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	SPI_PORT |= (1<<MOSI);	//Send the start bit
	SPI_PORT |= (1<<SCK);	//Clock HIGH
	_delay_us(1);
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	_delay_us(1);
	SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = (0b01000000|address);						//OP Code:01, Address:A5:0
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	SPDR = (data>>8);						//First data byte
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	SPDR = (data&0xFF);						//Second data byte
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	
	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);
	
	_93C46_WriteDisable();
}

void _93C46_WriteAll(uint16_t data)			//writing a 16bit data to all address
{
	_93C46_WriteEnable();

	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
	SPCR &= ~(1<<SPE);		//Disable SPI
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	SPI_PORT |= (1<<MOSI);	//Send the start bit
	SPI_PORT |= (1<<SCK);	//Clock HIGH
	_delay_us(1);
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	_delay_us(1);
	SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = 0b00010000;						//OP Code:00, Address:A5:01xxxx
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	SPDR = (data>>8);						//First data byte
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	SPDR = (data&0xFF);						//Second data byte
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	
	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);
	
	_93C46_WriteDisable();
}

void _93C46_Erase(uint8_t address)			//Erase a 16bit data from a 8bit address
{
	_93C46_WriteEnable();

	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
	SPCR &= ~(1<<SPE);		//Disable SPI
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	SPI_PORT |= (1<<MOSI);	//Send the start bit
	SPI_PORT |= (1<<SCK);	//Clock HIGH
	_delay_us(1);
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	_delay_us(1);
	SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = (0b11000000|address);						//OP Code:11, Address:A5:0
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set

	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);
	
	_93C46_WriteDisable();
}

void _93C46_EraseAll(void)			//Erase all memory
{
	_93C46_WriteEnable();

	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
	SPCR &= ~(1<<SPE);		//Disable SPI
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	SPI_PORT |= (1<<MOSI);	//Send the start bit
	SPI_PORT |= (1<<SCK);	//Clock HIGH
	_delay_us(1);
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	_delay_us(1);
	SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = 0b00100000;						//OP Code:00, Address:10xxxx
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	
	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);
	
	_93C46_WriteDisable();
}

void _93C46_Read(uint8_t address)			//Read from a 8bit address
{
	SPI_PORT |= (1<<SS);	//Select a slave

	//Sending the start bit using bit banging process
	SPCR &= ~(1<<SPE);		//Disable SPI
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	SPI_PORT |= (1<<MOSI);	//Send the start bit
	SPI_PORT |= (1<<SCK);	//Clock HIGH
	_delay_us(1);
	SPI_PORT &= ~(1<<SCK);	//Clock LOW
	_delay_us(1);
	SPCR |= (1<<SPE);		//Enable SPI
	
	SPDR = (0b10000000|address);						//OP Code:11, Address:A5:0
	while(bit_is_clear(SPSR,SPIF)){;}	//Wait until SPIF is set
	

	SPI_PORT &= ~(1<<SS);				//Return the slave to idle status
	_delay_us(1);
}

#endif
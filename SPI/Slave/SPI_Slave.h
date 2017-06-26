/*
 * SPI_Slave.h
 *
 * Created: 02/18/17 10:56:33 PM
 *  Author: TRAN THIEN HOANG
 */ 


#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

#define SPI_DDR DDRB		//Choose a port here
#define SPI_PORT PORTB		//Choose a port here
#define SS 4
#define MOSI 5
#define MISO 6
#define SCK 7

#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t data_rx;

void SPISlave_Ini(void)
{
	SPI_DDR |= (1<<MISO);						//MISO as OUTPUT
	SPI_DDR &= ~((1<<SCK)|(1<<MOSI)|(1<<SS));	//MOSI, SCK, SS as INPUT
	
	SPCR |= (1<<SPIE)|(1<<SPE);		//INT enable, SPI enable,MSB first, Slave mode, SPR1:0=00-prescaler=4
}

ISR(SPI_STC_vect)
{
	data_rx=SPDR;
}
#endif
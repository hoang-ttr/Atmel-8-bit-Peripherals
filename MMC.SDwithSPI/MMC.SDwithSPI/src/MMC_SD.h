/*
 * MMC_SD.h
 *
 * Created: 02/20/17 01:34:47 PM
 *  Author: Hoang Thien Tran
 */


#ifndef MMC_SD_H_
#define MMC_SD_H_

#include "avr/io.h"

#define SPI_PORT PORTB		//Set up port for SPI
#define SPI_DDR DDRB
#define SCK 7
#define MISO 6
#define	MOSI 5
#define SS 4

#define Block_Length 255	//Length for each block of data in MMC/SD ---MAX is 255 (for simulation)

#define CMD0 0x00			//Go_Idle_State
#define CMD1 0x01			//Request_For_Condition
#define CMD16 0x10
#define CMD17 0x11
#define CMD24 0x18

char Card_Status; //Busy flag for MMC/SD

void SPI_MasterIni(void);										//Initialize SPI for Master AVR
void SPI_SendByte(uint8_t data);								//Sent byte to slave
uint8_t SPI_ReadByte(void);										//Read byte from slave
uint8_t Card_Respond(uint8_t Respond);							//Acknowledgment from MMC/SD
void Card_Cmd(uint8_t command, uint32_t argument, uint8_t crc);	//Send command to card
uint8_t Card_Ini(void);											//Initialize interface with MMC/SD
uint8_t Card_Write(uint32_t LBAddress, char *buff);				//Write to a sector in MMC/SD
uint8_t Card_Read(uint32_t LBAddress, char *buff);				//Read to a sector in MMC/SD

#endif
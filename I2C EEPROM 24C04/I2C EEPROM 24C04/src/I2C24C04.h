/*
 * I2C24C04.h
 *
 * Created: 02/13/17 12:25:45 PM
 *  Author: Hoang Thien Tran
 */ 


#ifndef I2C24C04_H_
#define I2C24C04_H_

#include <avr/io.h>

void EEIni(void);	//Initialize TWI for 24C04
char EEWriteByte(uint8_t initial_address, uint8_t data[]);	//With WP=VIL, Page-write is activated, Maximum bytes to sent is 8
uint8_t EEReadByte(uint8_t initial_address);	//---Random address bit---

#endif
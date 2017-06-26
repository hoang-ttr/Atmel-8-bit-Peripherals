/*
 * DS1307.h
 *
 * Created: 02/23/17 05:33:38 PM
 *  Author: Hoang Thien Tran
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

volatile uint8_t Sec,Min,Hour,Day,Date,Month,Year,Mode,AmPm;
volatile uint8_t timevalue[7],storedvalue[7];
char displayLCD[2];
volatile uint8_t counter;

void RTCIni();	//Initialize TWI for DS1307
char RTCWrite(uint8_t initial_address,volatile uint8_t *data, uint8_t length);	//Write to a desired blocks of bytes
char RTCRead(uint8_t initial_address, uint8_t length);	//---Random address bit---
uint8_t BCD2Dec(uint8_t BCD);
uint8_t Dec2BCD(uint8_t Dec);
void Display();

#endif
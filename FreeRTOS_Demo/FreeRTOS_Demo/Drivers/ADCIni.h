#ifndef _ADCINI_H
#define _ADCINI_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define AREF 0
#define AVCC (1<<REFS0)
#define INTERNAL (1<<REFS1)|(1<<REFS0)

volatile uint16_t ADCresult;

//----------Main Function----------//
void ADCInit(void);			//Initialize ADC module
void readAD(uint8_t pinADC); //Read out a ADC value at a pin

#endif
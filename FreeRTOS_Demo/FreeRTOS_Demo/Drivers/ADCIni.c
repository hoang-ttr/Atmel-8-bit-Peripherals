/*
 * CFile1.c
 *
 * Created: 03/13/17 09:04:18 AM
 *  Author: Hoang Thien Tran
 */

#include "ADCIni.h"

void ADCInit(void)
{
	DDRA = 0x00; //ADC as INPUT
	sei();
	ADCSRA |= (1<<ADEN); //Enable ADC //Always do this FIRST
	ADMUX |= AVCC; //Choose reference voltage
	ADCSRA |= (1<<ADIE); //Enable AD Interrupt
	ADCSRA |= (1<<ADPS2)|(1<<ADPS0); //Division factor: 32
}
void readAD(uint8_t pinADC) //Read out a ADC value at a pin
{
	ADMUX = AVCC|pinADC; //Enable single output at pin
	ADCSRA |= (1<<ADSC); //Start ADC
}

ISR(ADC_vect)
{
	uint8_t lowAD = ADCL;
	uint8_t hiAD = ADCH;
	ADCresult = hiAD<<8|lowAD;
}
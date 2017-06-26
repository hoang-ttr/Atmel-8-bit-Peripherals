/*
 * UART.c
 *
 * Created: 03/16/17 02:07:34 PM
 *  Author: Hoang Thien Tran
 */ 

#include "UART.h"

volatile uint8_t i=0;

void UARTIni(char rx, char tx)
{
	UBRRH=0;									//f=16MHz, baud rate=9614bps
	UBRRL=103;
	
	UCSRA=0x00;									//U2X=0:Normal speed, MPCM=0:single processor
	
	if(rx==1)	//Enable RX and its interrupt
	{
		sei();
		UCSRB|= (1<<RXEN)|(1<<RXCIE);
	}
	else		UCSRB&= ~((1<<RXEN)|(1<<RXCIE));
	if(tx==1)	UCSRB|= (1<<TXEN);				//Enable TX
	else		UCSRB&= ~(1<<TXEN);
	
	UCSRC|= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);	//Choose UCSRC, Asynchronous mode, no Parity, 1 Stop bit, UCSZ2:0=011: 8 bit data,
}

void UART_SendChar(char uartchar)
{
	if(uartchar=='\n')	UART_SendChar('\r');
	while(bit_is_clear(UCSRA,UDRE)){;}
	UDR=uartchar;
}

void UART_SendString(char *pStr)	//Send a string
{
	while(*pStr>0)					//Whenever pointer gets to the end, *pStr = 0
	UART_SendChar(*pStr++);			//Send individual char and increase pointer address
}

ISR(USART_RXC_vect) //UART RECIEVE INTERRUPT
{
	uart_data_rx=UDR;
	if(i==100)	i=0;	//reset the storage after 100 write
	data_rx_storage[i++]=uart_data_rx;	//store data for further use
}
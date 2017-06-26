/*
 * UART.h
 *
 * Created: 02/08/17 05:19:24 PM
 *  Author: Hoang Thien Tran
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

volatile char uart_data_rx, data_rx_storage[100];	//Storage if many pack are received
volatile extern uint8_t i;							//Counter for data storage

void UARTIni(char rx, char tx);
void UART_SendChar(char uartchar);
void UART_SendString(char *pStr);	//Send a string

#endif
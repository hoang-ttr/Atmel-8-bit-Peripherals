/*
 * LED.h
 *
 * Created: 7/5/2011 0:04:32
 *  Author: MMM
 */

#ifndef LED_H_
#define LED_H_

// Output Port pin LED_O
#define LED1		PORTC
#define LED1_DDR	DDRC
#define PinLED		7

void vLEDInit(void);
void vLEDFreeRun(void);

#endif /* LED_H_ */
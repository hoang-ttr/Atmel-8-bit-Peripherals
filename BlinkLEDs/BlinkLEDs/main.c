/*
 * BlinkLEDs.c
 *
 * Created: 09/20/16 02:29:03 PM
 * Author : Hoang Thien Tran
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define LED1_DDR	DDRA	//active-HIGH LEDs
#define LED2_DDR	DDRB	//active-LOW LEDs
#define LED1		PORTA
#define LED2		PORTB

int main(void)
{
	uint8_t i;
	LED1_DDR = 0xFF; //LEDs as OUTPUT
	LED2_DDR = 0xFF; //LEDs as OUTPUT
	
	while (1) 
    {
		for (i=0;i<7;i++)
		{
			LED1^= 0xFF;
			LED2= ~LED1;
			_delay_ms(100);
		}
		LED1=0;
		LED2=0xFF;
		for (i=0;i<7;i++)
		{
			LED1^= 0xAA;
			LED2= ~LED1;
			_delay_ms(100);
			LED1=0;
			LED2= ~LED1;
			_delay_ms(100);
			LED1^= 0x55;
			LED2= ~LED1;
			_delay_ms(100);
		}
		LED1=0;
		LED2=0xFF;
		for (i=0;i<7;i++)
		{
			LED1=(1<<i);
			LED2=~(0x80>>i);
			_delay_ms(100);
		}
		LED1=0;
		LED2=0xFF;
		for (i=0;i<7;i++)
		{
			LED1=(0x80>>i);
			LED2=~(1<<i);
			_delay_ms(100);
		}
		LED1=0;
		LED2=0xFF;
		for (i=0;i<7;i++)
		{
			LED1|=(1<<i);
			LED2&=~(0X80>>i);
			_delay_ms(100);
		}
		LED1=0;
		LED2=0xFF;
		for (i=0;i<7;i++)
		{
			LED1|=(0X80>>i);
			LED2&=~(1<<i);
			_delay_ms(100);
		}
		LED1=0;
		LED2=0xFF;
    }
}
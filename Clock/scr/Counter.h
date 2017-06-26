/*
 * Interrupt.h
 *
 * Created: 09/21/16 03:10:16 PM
 *  Author: Hoang Thien Tran
 */ 


#ifndef COUNTER_H_
#define COUNTER_H_

char volatile timevalue[3]; //Set initial value for time 0:SEC 1:MIN 2:HOUR
char timedisplay[2];

void CLK_Ini()
{
	TCCR1B |= (1<<CS12); //CS=011: clk/256
	TCNT1 = 3036; //Initial value of T/C0=6
	sei(); //SEt global Interrupt
	TIMSK |= (1<<TOIE1); //Enable Timer Overflow Interrupt
}

ISR(TIMER1_OVF_vect) //Interrupt when T/C1 has run for 1s
{
	TCNT1 = 3036;
	timevalue[0]++;
	if(timevalue[0]==60)
	{
		timevalue[0]=0;
		timevalue[1]++;
	}
	if(timevalue[1]==60)
	{
		timevalue[1]=0;
		timevalue[2]++;
	}
}

#endif
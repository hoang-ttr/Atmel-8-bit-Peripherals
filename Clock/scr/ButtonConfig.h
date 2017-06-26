/*
 * ButtonConfig.h
 *
 * Created: 09/22/16 01:13:38 PM
 *  Author: Hoang Thien Tran
 */ 


#ifndef BUTTONCONFIG_H_
#define BUTTONCONFIG_H_

int choosevalue() //choose what timevalue to modify
{
	int i=0;
	if(bit_is_clear(PINB,7))
	{
		i = 2;
	}
	if(bit_is_clear(PINB,6))
	{
		i = 1;
	}
	if(bit_is_clear(PINB,5))
	{
		i = 0;
	}
	return i;
}
void changevalue() //Increase, decrease, reset timevalue
{
	if(bit_is_clear(PINB,0))
	{
		timevalue[choosevalue()]++;
	}
	if(bit_is_clear(PINB,1))
	{
		timevalue[choosevalue()]--;
	}
	if(bit_is_clear(PINB,2))
	{
		timevalue[0]=0;
		timevalue[1]=0;
		timevalue[2]=0;
	}
}

#endif
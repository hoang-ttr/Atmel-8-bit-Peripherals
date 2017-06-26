/*
 * mytasks.c
 *
 * Created: 7/5/2011 23:45:23
 *  Author: MMM
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "mytasks.h"
#include "LED.h"
#include "lcd4bit.h"
#include "ADCIni.h"
#include "Buzzer.h"
#include "Button.h"

QueueHandle_t xADCQueue = 0;
TaskHandle_t vBuzzerButtonHandle = NULL;

void vLEDFlashTask( void *pvParameters )
{
	portTickType lastWake = xTaskGetTickCount();
	uint8_t i;

	for( ;; )
	{
		for (i=0;i<7;i++)
		{
			LED1^= 0xFF;
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
		}
		LED1=0;
		for (i=0;i<7;i++)
		{
			LED1 = 0xAA;
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
			LED1 = 0;
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
			LED1 = 0x55;
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
		}
		LED1=0;
		for (i=0;i<7;i++)
		{
			LED1=(1<<i);
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
		}
		LED1=0;
		for (i=0;i<7;i++)
		{
			LED1=(0x80>>i);
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
		}
		LED1=0;
		for (i=0;i<7;i++)
		{
			LED1|=(1<<i);
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
		}
		LED1=0;
		for (i=0;i<7;i++)
		{
			LED1|=(0X80>>i);
			vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
		}
		LED1=0;
	}
}

void vLCDPrintString1(void *pvParameters)
{
	portTickType lastWake = xTaskGetTickCount();
	char *printStr;

	printStr = (char *) pvParameters;

	for (;;)
	{
		Lcd4_Cursor(1,1);
		Lcd4_SetString(printStr);
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(5000));
	}

}

void vLCDPrintCounting(void *pvParameters)
{
	portTickType lastWake = xTaskGetTickCount();
	static uint8_t counter=0;

	for(;;)
	{
		Lcd4_Cursor(1,13);
		Lcd4_SetNum(counter++);
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(1000));
	}
}

void vLCDPrintADC(void *pvParameters)
{
	portTickType lastWake = xTaskGetTickCount();
	uint16_t ADCQueueValue_Get;

	for(;;)
	{
		xQueueReceive(xADCQueue, &ADCQueueValue_Get, 100);	//Receive vaule from the task ADC0
		Lcd4_Cursor(2,9);

		if(ADCQueueValue_Get==0)
		{
			Lcd4_SetString("________");
		}
		else if(ADCQueueValue_Get<128)
		{
			Lcd4_SetString("*");
			Lcd4_SetString("_______");
		}
		else if(ADCQueueValue_Get<(128*2))
		{
			Lcd4_SetString("**");
			Lcd4_SetString("______");
		}
		else if(ADCQueueValue_Get<(128*3))
		{
			Lcd4_SetString("***");
			Lcd4_SetString("_____");
		}
		else if(ADCQueueValue_Get<(128*4))
		{
			Lcd4_SetString("****");
			Lcd4_SetString("____");
		}
		else if(ADCQueueValue_Get<(128*5))
		{
			Lcd4_SetString("*****");
			Lcd4_SetString("___");
		}
		else if(ADCQueueValue_Get<(128*6))
		{
			Lcd4_SetString("******");
			Lcd4_SetString("__");
		}
		else if(ADCQueueValue_Get<(128*7))
		{
			Lcd4_SetString("*******");
			Lcd4_SetString("_");
		}
		else if(ADCQueueValue_Get<(128*8))
		{
			Lcd4_SetString("********");
		}
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(225));
	}
}

void vADCGetValue(void *pvParameters)
{
	portTickType lastWake = xTaskGetTickCount();
	uint16_t ADCQueueValue_Send;

	for(;;)
	{
		readAD(0);
		Lcd4_Cursor(2,1);
		Lcd4_SetNum(ADCresult);
		ADCQueueValue_Send = ADCresult;
		xQueueSend(xADCQueue, &ADCQueueValue_Send, 0);	//Send vaule to the task LCD2
		if (ADCQueueValue_Send>=1000)
		{
			for(uint8_t i=0; i<50; i++)
			{
				Buzzer ^= (1<<Buzzer_Pin);
				vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(20));
			}
			Buzzer |= (1<<Buzzer_Pin);
		}
		else
		{
			Buzzer |= (1<<Buzzer_Pin);
		}
		//xTaskNotify(vBuzzerADCHandle, ADCQueueValue_Send, eSetValueWithoutOverwrite);
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(200));
	}
}

void vButton(void *pvParameters)
{
	portTickType lastWake = xTaskGetTickCount();

	for(;;)
	{
		debounce();
		if (button_down)
		{
			button_down = 0;
			xTaskNotifyGive(vBuzzerButtonHandle);
		}
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(10));
	}
}

void vBuzzerFromButton(void *pvParameters)
{
	portTickType lastWake = xTaskGetTickCount();

	for(;;)
	{
		if (0 != ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(20)))
		{
			Lcd4_Cursor(1,1);
			Lcd4_SetString("Notified!");
			for(uint8_t i=0; i<20; i++)
			{
				Buzzer ^= (1<<Buzzer_Pin);
				vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(75));
			}
			Buzzer |= (1<<Buzzer_Pin);
		}
		else
		{
			Buzzer |= (1<<Buzzer_Pin);
		}
	}
}

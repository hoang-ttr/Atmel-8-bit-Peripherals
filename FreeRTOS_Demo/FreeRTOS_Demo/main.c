/*
 * freertosm128.c
 *
 * Created: 7/4/2011 18:43:58
 *  Author: MMM
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "mytasks.h"
#include "lcd4bit.h"
#include "LED.h"
#include "ADCIni.h"
#include "Buzzer.h"
#include "Button.h"

/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainLED_PRIORITY	( tskIDLE_PRIORITY)
#define mainLCD_PRIORITY	(tskIDLE_PRIORITY+1)
#define mainADC_PRIORITY	(tskIDLE_PRIORITY+2)
#define mainBUTTON_PRIORITY	(tskIDLE_PRIORITY+3)

static const char *thisString = "FreeRTOS!";

portSHORT main(void)
{
	vLEDInit();
	ADCInit();
	Lcd4_Init();
	Lcd4_Clear();
	Buzzer_Init();
	ButtonInit();
	//
	xADCQueue = xQueueCreate(1, sizeof(uint16_t));
	//
	xTaskCreate(vLEDFlashTask, "LED0", configMINIMAL_STACK_SIZE, NULL, mainLED_PRIORITY, NULL );
	xTaskCreate(vLCDPrintString1, "LCD0", configMINIMAL_STACK_SIZE, (void*) thisString, mainLCD_PRIORITY, NULL);
	xTaskCreate(vLCDPrintCounting, "LCD1", configMINIMAL_STACK_SIZE, NULL, mainLCD_PRIORITY, NULL);
	xTaskCreate(vLCDPrintADC, "LCD2", configMINIMAL_STACK_SIZE, NULL, mainLCD_PRIORITY, NULL);
	xTaskCreate(vADCGetValue, "ADC0", configMINIMAL_STACK_SIZE, NULL, mainADC_PRIORITY, NULL);
	xTaskCreate(vBuzzerFromButton, "Buzzer0",  configMINIMAL_STACK_SIZE, NULL, mainBUTTON_PRIORITY+1, &vBuzzerButtonHandle);
	xTaskCreate(vButton, "Button0", configMINIMAL_STACK_SIZE, NULL, mainBUTTON_PRIORITY, NULL);
	//start scheduler
	vTaskStartScheduler();
	//you should never get here
	while (1)
    {
		/*Infinite loop*/
    }
	return 0;
}

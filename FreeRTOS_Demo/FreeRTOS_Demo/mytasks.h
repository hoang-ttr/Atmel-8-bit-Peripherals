/*
 * mytasks.h
 *
 * Created: 7/5/2011 23:45:37
 *  Author: MMM
 */


#ifndef MYTASKS_H_
#define MYTASKS_H_

extern QueueHandle_t xADCQueue;
extern TaskHandle_t vBuzzerButtonHandle;

void vLEDFlashTask(void *pvParameters);
void vLCDPrintString1(void *pvParameters);
void vLCDPrintCounting(void *pvParameters);
void vADCGetValue(void *pvParameters);
void vLCDPrintADC(void *pvParameters);
void vBuzzerFromButton(void *pvParameters);
void vButton(void *pvParameters);

#endif /* MYTASKS_H_ */
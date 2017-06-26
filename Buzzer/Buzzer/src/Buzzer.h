/*
 * buzzer.h
 *
 * Created: 04/16/17 09:53:46 PM
 *  Author: Hoang Thien Tran
 */


#ifndef BUZZER_H_s
#define BUZZER_H_

#define Buzzer PORTB
#define Buzzer_DDR DDRB
#define Buzzer_Pin 0

#define freq 250

void Buzzer_Init(void);
void BuzzerRun(void);

#endif /* BUZZER_H_ */
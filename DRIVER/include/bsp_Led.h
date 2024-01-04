
#ifndef _LED_H
#define _LED_H

typedef enum
{
	LED1 = 0x00,
	LED2,
}LED_DefType;

#define STOP_LED_TYPE	(LED2)
#define START_LED_TYPE  (LED1)

//LEDiO�ĳ�ʼ��
extern void InitLed(void);

//���õƵ�״̬
extern void SetLedState(LED_DefType led, u8 fOn);

extern u8 TM1638_KEY[];

extern void TM1638_IO_Init(void);
#endif



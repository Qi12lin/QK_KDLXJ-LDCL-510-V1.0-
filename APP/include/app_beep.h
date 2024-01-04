#include "typedef.h"

#ifndef _BEEP_PROC_H
#define _BEEP_PROC_H

//������IO�˿�
#define BEEP_PORT GPIOC

//������IO����
#define BEEP_PIN  GPIO_Pin_5

//������ʱ��
#define	BEEP_RCC RCC_AHB1Periph_GPIOC //RCC_APB2Periph_GPIOC
 

//��������Ϣ�ṹ��
typedef struct 
{
		vu16 beepOnTime;		//һ����������������ڷ�����on�ļ�����
		vu16 beepOffTime;					//һ����������������ڷ�����off�ļ�����
		vu16 beepCycleOnTime;     //һ�������ڷ���������ʱ��
		vu16 beepCycleOffTime;		//һ�������ڷ������ص�ʱ��
		vu16  cnt;									//��Ĵ���   
		vu8  beepBusy;						//������æ��ʶ
		vu8  beepSignal; 			//�������ź�
}BEEP_TYPE;

extern BEEP_TYPE beepInfo;

extern void BeepInit(void);    //��������ʼ��
extern void Beep_SetTime(u16 cnt, u16 onTime, u16 offTime);
extern void Beep_RecordTimer(void); //��������ʱ����
extern void ClearBeep(void);    //���������
extern u8 BeepBusy(void);
//�������Ĵ������
extern void Beep_Proc(void);


#endif



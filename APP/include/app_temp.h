#include "typedef.h"

//
#ifndef _TEMP_H
#define _TEMP_H

//�¶�״̬����
typedef enum
{
	TEMP_NO = 0x00,             //��״̬
	TEMP_UP = 0x01,             //����
	TEMP_DOWN = 0x02,           // �½�
	TEMP_STABILITY = 0x03       //�ȶ�״̬
}TEMP_STATE;

//�¶Ȳ���
typedef struct
{
	vu8  state;    //״̬
	vu8 fCtrlSt; //���ƿ�ʼ��־
	vu8 CtrlCycle;     //����
	vu8 fTarTempr;     //����Ŀ���¶ȱ�־
	int realTemp;  //ʵʱ�¶�
	int targetTemp; //Ŀ���¶�
}TEMP_PARAM;

//�¶ȿ�������
typedef enum 
{
	TEMP_CYCLE0 = 0x00,
	TEMP_CYCLE1 ,
	TEMP_CYCLE2 ,
	TEMP_CYCLE3 ,
	TEMP_CYCLE4 ,
	TEMP_CYCLE5,
	TEMP_CYCLE_ALL = 6
}CTRL_TEMP_CYCLE;

//�¶Ȳ���
extern TEMP_PARAM temper; //
extern TIME_TYPE temprTimer;

//��ʾ�¶�
extern void DispTemperature(int t_tempr,u8 fDisp);
//����״̬�¶ȵ���ʾ
extern void dispSetTempr(void);

//�¶ȿ��ƹ���
extern void Temp_Proc(void);
//�¶ȳ�ʼ��
void TemprInit(void);
extern int temp_count(u16 ADC_bit);

extern u16 Y4_OFF_time,Y4_ON_time;
#endif


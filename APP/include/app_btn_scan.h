#include "typedef.h"

#ifndef _BUTTON_H
#define _BUTTON_H

//���̺궨��

#define SET_KEY    VK_KEY7
#define ADD_KEY    VK_KEY8
#define SUB_KEY    VK_KEY9
#define ENTER_KEY  VK_KEY1
#define CUT_KEY    VK_KEY2
//#define AJ_STOP_KEY   VK_KEY10
//#define AJ_START_KEY  VK_KEY4
//#define SHORT_KEY  VK_KEY3
#define OPEN_KEY  VK_KEY3

//һ������ܶ�Ӧ�Ĺ���
typedef enum 
{
		GROUP_FRIST =0,
		GROUP_PROG =0	,  //������
		GROUP_ROTOR 	,  //ת�Ӻ���
		GROUP_FORCE_SPEED ,  //��������ת���飬�ۼ����Ļ��� �ۼ�ʱ��
		GROUP_TEMP,      //�¶���   û��ʹ��
		GROUP_HOUR,      //ʱ��Сʱ
		GROUP_TIME  	,  //ʱ����
		GROUP_SECOND  ,  //����
		
		GROUP_UPGEAR 	,  //���ٵ�λ��
		GROUP_DOWNGEAR ,  //���ٵ�λ��
		
		GROUP_ALL ,      //�ۼ�����
		
		GROUP_FORCE,
	
}GROUPs;

extern vu8 DTLED_groud ; //�������
extern u8 DTLED_groud_set,DTLED_groud_set_no; 
//�����ź�
typedef struct
{
	vu8 setSignal:1; 		//���ü��ź�
	vu8 addSignal:1; 		//�Ӽ��ź�
	vu8 subSignal:1; 		//�����ź�
	vu8 enterSignal:1;	 //ȷ�����ź�
	vu8 cutSignal:1;   	//�л����ź�
	vu8 openSignal:1; 	 //���ż��ź�
	vu8 startSignal:1;	 //�������ź�
	vu8 stopSignal:1;  	//ֹͣ���ź�
	vu8 com1Signal:1;     //��ϼ��ź�
	vu8 com2Signal:1;     //��ϼ�2�ź�
	vu8 timeSignal:1; 		//���ü��ź�
	vu8 rotorSignal:1; 		//�Ӽ��ź�
	vu8 speedSignal:1; 		//�����ź�
	vu8 tempSignal:1;	 //ȷ�����ź�
}KEY_SIGNAL;

//�����ź�
extern KEY_SIGNAL keySignal2;

//��ʼ�����ź�
extern void InitKeySignal(void);
//��ȷ������ʱʱ��
extern void ClearEnterTimerC(void);

//����ɨ��ʹ���
extern void Key_Proc(void);

extern vu8 keySET_State; //���ü�״̬

extern u8 If_key_press(u8 key);//��ͬ��ֵ�Ƿ���
extern u8 IfStopPress(void);

//�߼������еļӼ�ɨ��
extern void HighADDKey_Scan(void);
//�߼����ù����м�������
extern void HighSUBKey_Scan(void);
//���ü�ɨ��
extern void SETKey_Scan(void);
//ȷ����ɨ��
extern void ENTERKey_Scan(void);

extern void LCD_DrawBMP_RotorMassege(uint16_t _usX, uint16_t _usY, char *_ptr);

//��ϼ���Ӧ
extern void Comb2_Scan(void);

extern void STOPKey_Scan(void);

#endif

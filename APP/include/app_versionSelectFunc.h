
#include "typedef.h"

#ifndef _VERSION_SELECT_FUNC_H
#define _VERSION_SELECT_FUNC_H

typedef enum
{
	LY_TDL5M =1 ,
	

	
	VER_MAX_NUM,
		 AJY_A5K_A,
	 AJY_A6K  ,
	LY_TGL_16M,
	LY_TGL18M,
	LY_TGL20M,	
	
	XZ_P6_N,
	XZ_P5,
	DM_P48,
	DM_P80,
	XZ_TD6B,
XZ_TD6B_N,
	CX_TD6B,
	
	MC20000,
	
	MC5000S
	

}P_TYPE;
#define LOW_TYPE  1  //���ٻ��͸���
//�ͺ���Ϣ�ṹ
typedef struct
{
	vu8 No;          //���
  vu16 pSpeedTab[20]; //���ת�ٱ�
	vu16 pForceTab[20]; //�����������
  vu8 RotorKind;  //ת������
	vu8 maxCur;    //�����
	vu8 baseFreq;   //��Ƶ
}VERSION; 

//����������
typedef struct
{
	vu8 id;   					//���
	vu8 Maxcur;		 			//�����
	vu16 baseFreq; 			//��Ƶ
	vu16 RatedPower; 		//�����
}MOTOR_PHYSICS_PARM;





//һ���ͺ�



//��ȡ��ǰ�İ汾��
//extern u8 GetCurrentVerNo(void);
//���õ�ǰ�İ汾��
//extern void SetCurrentVerNo(u8 ver);
//��ȡ�ͺ��ź�
extern void GetVersionInfo(void);
//�ͺŵĶ�ȡ
extern void GetMemVerInfo(void);
//�ͺŵı���
extern void SaveToMemVerInfo(void);

extern unsigned char get_rotor_N(unsigned char _type);
extern void get_rotor_info(unsigned char type,u16 *speed,u16 *force,unsigned char *rotorkind);
#endif

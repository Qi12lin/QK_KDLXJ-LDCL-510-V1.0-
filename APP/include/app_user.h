//#include "stm8s_lib.h"
#include "typedef.h"

#ifndef _USER_H_
#define _USER_H_

#define MAX_PROGRAM  40    //�������   ���100������

//�߼�������ʼ����ֵ
#define INIT_SYS_TYPE	       LY_TDL5M
#define INIT_LOCK_TYPE	 ELEC_LOCK_TYPE

#define INIT_BASE_F_HIGH  200//���ٻ���Ƶ  MC21000��ƵΪ120Hz
#define INTT_RATED_CURRENT_HIGH 17

#define INIT_BASE_F_LOW   85//���ٻ���Ƶ
#define INIT_RATED_CURRENT_LOW	 27

#define INIT_LANGUAGE 1       //1--����  2--Ӣ��
#define INTI_LOCK_PROTECT 2   
#define INIT_HALL_EN 2
#define INIT_BPH_SET  2
#define INIT_AUTOOPEN_EN 1      //1:ͣ�����Զ����� 2��ͣ���Զ�����
#define INIT_AUTO_ROTOR  1      //ת��ʶ��  1--��ʶ��  2 ת���Զ�ʶ��

#define INTI_MOTOR_TYPE  1 //�����������   0����Ƶ�����Ʊ�Ƶ���    1����ˢ���   2���ķ���Ƶ������
#define INIT_VOLTAGE    2  //��Դ��ѹ       2:220V   1��110V
#define INIT_DOOROPEN_TIME  100

#define INIT_STARTPAGE 1

typedef struct
{
	vu8 ucRotorNo;   //ת�Ӻ�
	vu16 uiSpeed;
	vu16 uiRCF;
	vu8  ucTime_HH;
	vu16 uiTime_MM;
	vu8  ucTime_SS;
	s16  iTemprature;
	vu8  ucAcc;
	vu8  ucDec;
	vu8  ucBack1;  //����1 2
	vu8  ucBack2;  
}PROGRAMDATA;  //��15�ֽ�
extern PROGRAMDATA  OneProgramData;   //һ���������
extern PROGRAMDATA  progData[10];    //һ�ζ�ȡ10������
//��λ����
typedef enum
{
	UPGEAR_TYPE,
	DOWNGEAR_TYPE
}GEAR_TYPE;

#define MAX_ROTOR 20
typedef struct
{
//	vu16 uiSpeed[MAX_ROTOR];
//	vu16 uiRCF[MAX_ROTOR];
//	vu8  ucTime_HH[MAX_ROTOR];
//	vu16 uiTime_MM[MAX_ROTOR];
//	vu8  ucTime_SS[MAX_ROTOR];
//	vs16  iTemprature[MAX_ROTOR];
	vu8  ucAcc[MAX_ROTOR];
	vu8  ucDec[MAX_ROTOR];
}ROTORPROGRAMDATA;   //��Ӧת�Ӻű��������  Ԥ��25��ת�Ӻ� �еĲ�ʹ�ó��������Ҫ����ת�Ӻű��ֲ������� 
extern ROTORPROGRAMDATA RotorPrgData;
//��λ������
typedef enum
{
	SLOW_GEAR = 0x01,     //��ͨ��λ
	FAST_GEAR = 0x02          //���ٵ�λ
}GEAR_KIND;

extern GEAR_KIND fFast;

//ת�������Ŀ
extern u8 RotorMaxNum ;
//���������ֵ
extern vu8 maxCurValue;
//������
extern u16 Force_max[];
//ת��
extern u16 Speed_max[];
void read_eepromRotorProgranData(void);
//ȡת�Ӷ�Ӧ������������Ϣ
void get_RotorProgranData( void );
//����ת�Ӷ�Ӧ������������Ϣ
void set_RotorProgranData(void)	;
//��ȡһ������
void read_eepromOneProgData(u8 _ProgNO);
//д��һ������
void write_eepromOneProgData(u8 _ProgNO);

//ȡ���������ݵ���������
void get_programData(PROGRAMDATA*  _ProgramData);	
//���ó���������   ȡ�������ݵ�����������
void set_programData(PROGRAMDATA*  _ProgramData);

//ת�Ӻ����ֵ
#define ROTOR_MAX	RotorMaxNum
//ת�����ת��
#define MAXROTORSPEED Speed_max[ HldReg[ROTOR]]

#define GEAR_NUM	10
#define TIMER_MAX 59


extern u8 setData_flag ; //�������޸��ǲ��ϴ�
//������ת��Ϊ�ٶ�
extern u16 Force_to_Speed(u16 force,u8 rotorNo);
//�ٶ�ת��Ϊ������
extern u16 Speed_to_Force(u16 speed, u8 rotorNo);
//��ȡ������������ת��
extern u16 Fc_SpeedMax (u8 rotorNo, u8 fSpeed);
extern void SetRotor (void);
//���б���������ٵ�λ
extern void GetAllGearFromMem(void);
//������Ӧת�Ӻŵ������ٵ�λ
extern u8 SaveGear( u8 rotorNo);
//��ȡ��Ӧת�Ӻŵ������ٵ�λ
extern u8 GetGear( u8 rotorNo);
//��ȡ���еĲ���
extern void GetLXLParamFromMem(void);
//��ȡ���еĲ���
extern void GetDataFromMem(void);
//�������еĵ�λ��Ϣ
extern void SaveAllGear(void);
//��������������
extern void SaveLXLParam(void);
//���ù��������õ�ǰ��ʱת�ӵĵ�λ
extern void SetGear(u8 rotorNo);
//��ȡ��λ��Ӧ��ʱ��
extern u16 GetGearTime(u8 gear,u8 fFst);
//��ȡ���ٽ��ٵ�λ��ʱ��
extern u16 GetGearFastTm(u8 gear);
//��ȡ���ת��
extern u16 GetMaxSpeed(u8 rotor);



	//������ʾ
extern void Win_Proc(void);

extern void DTLEDS_DispProc(void);
//����ת�Ӻ��������ʾ
extern void DTLEDs_UD_Rotor(u8 r, u8 fDisp);
//������ʾת��/�������������ʾ
extern void DTLEDs_UD_SpOrFc(u16 sp, u8 fDispDot, u8 fDisp);
//����ʱ����ʾ
extern void DTLEDs_UD_Time(u8 time,u8 fDisp);
//��������/���ٵ�λ����ʾ
extern void DTLEDs_UD_Gear(u8 gear,u8 fDispDot,u8 fDisp);

extern void Read_eepromData(void);
extern void Save_eepromData(void);
extern void Maxmin_Param(u8 _type);
extern void write_eepromAllRunTime(void);

#endif



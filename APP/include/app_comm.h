#include "typedef.h"

#ifndef _COMM_PROC_H
#define _COMM_PROC_H (1)
//-------------------------------------------------------
//MODBUS��վ����
//-------------------------------------------------------
#define	HLDREG_MAX_NUM		1100
#define	HLDREG_MAX_ADR		(HLDREG_MAX_NUM-1)
extern uint16_t	HldReg[HLDREG_MAX_NUM];		//4x 1-100	--cmd 16/3	(W/R)���ּĴ���	

//0~0x49(57)Ϊ�ϵ粻�������� 
//0~     �ϴ�������ʾ
//typedef enum
//{
//	RUN_SPEED  =0x1000,
//	RUN_RCF,
//	RUN_TIME_HH,
//	RUN_TIME_MM,
//	RUN_TIME_SS,
//	RUN_TEMPERATURE,       //ʵ�����в���
//	RUN_STATE,
//	ALARM,
//	DOOR_STATE,
//	
//	MAX_RUN_NUMBER     //���в�������   ע�ⲻ�����������ĵ�ַ�ص�
//	
//}RUN_DATA;
//   ~0x4F  ���ϴ�������ʾ �������Ŀ����ź�
typedef enum
{
	SHORT_KEY=0x1042,     //�㶯��  0x28 0x29�м�ֵ
	RCF_KEY =0x1043 ,    //�������л���
	CLEAR_ALLRUNTIME=0x1044,
	STOPATART_KEY =0x1045,
	CLEAR_ALARM =0x1046,
	DOOR_KEY =0x1047,
	STOP_KEY,
	START_KEY =0x1049,
}RUN_CONTORL;


//0x50-0xFFΪ�ϵ籣���ҷ��͵�������ʾ�Ĳ���  �������ò����͸߼����ò���

typedef enum
{
	START_SET_NUMBER =0,      //���ò������� MAX_SET_NUMBER - START_SET_NUMBER+1
	PROGRAM  =0,
	ROTOR,
	SET_SPEED,
	SET_RCF,
	SET_TIME_HH,
	SET_TIME_MM,
	SET_TIME_SS,
	SET_TEMPERATURE,       //�������ò���
  ACC,
	DEC ,
	SET_SPEEDRCF =0x0a,      //ת��������
	
	SET_CLOSE_TEMP,  //  �ر������¶�
	SET_OPEN_TEMP,   //���������¶�
	SET_hot_time,
	SET_cool_time,
	
	BASE_F =0x10,      //�߼����ò���
	RATED_CURRENT,
	SYSLOCK_TIME,
	SYS_TYPE,   
	LOCK_TYPE,
	AUTO_ROTOR=0x15,
	BPH_SET,
	LOCK_PROTECT,
	LANGUAGE,
	HALL_EN,
	
	MOTOR_TYPE,   //�����������   0����Ƶ�����Ʊ�Ƶ���    1����ˢ���   2���ķ���Ƶ������
	VOLTAGE,      //��Դ��ѹ       0:220V   1��110V
	DOOROPEN_TIME,    //����ʱ��
	AUTOOPEN_EN=0x1D,     //ͣ���Զ����� 
	SAVE_MIMA_H,      //��������� ռ�����ֽ�
	SAVE_MIMA_L,      //���������
	TEMPDISP,         //�¶���ʾ�˲�ϵ�� 0x20
	SET_STATRPAGE =0x21,   //������������
	SET0 =0x28,
	SET1 =0x29,
	MAX_SET_NUMBER,   
	
	
}SAVE_RUN_DATA;
	

typedef enum
{
	IN_MIMA_H=0x50,  //��������
	IN_MIMA_L,
	IN_MIMA1_H,
	IN_MIMA1_L,      //�޸�����ʱ��������1
	IN_MIMA2_H,
	IN_MIMA2_L,      //�޸�����ʱ��������2
	
	UPDATA_YEAR =0x56,
	UPDATA_MONTH,
	UPDATA_DATA,
	UPDATA_HOURS,
	UPDATA_MINUTES,
}GET_DATA;
//���ܵ��������� ������



#define CHANGE_PAGE				827		//�л�ҳ����Ϣ

#define HIGHSET_PAGE      3  //�߼���������ҳ��
//-------------------------------------------------------
//100-199Ϊ�ϵ籣���ҷ��͵�������ʾ�Ĳ���
//-------------------------------------------------------








//-------------------------------------------------------
//MODBUS��վ����
//-------------------------------------------------------
#define	SLAVE_HLDREG_MAX_NUM		30
#define	SLAVE_HLDREG_MAX_ADR		(SLAVE_HLDREG_MAX_NUM-1)
extern uint16_t	SlaveHldReg[SLAVE_HLDREG_MAX_NUM];		//�����ӵ�ַ40001��Ӧ��ַ0
#define	NUMBER_OF_VALUES_USED_EX_CTR	0//����ʹ������
#define	CLEANING_INTERVAL_EX_CTR		1//��Ҽ��ʱ��
#define	CLEANING_PULSE_WIDTH_EX_CTR	2//�������
#define	OFFLINE_CLEANING_TIME_EX_CTR	3//�������ʱ��
#define	CLEANING_MODE_EX_CTR			4//���ģʽ��0-����,1-ѹ��

#define	CLEANING_CYCLE_INTERVAL_EX_CTR		7//���ѭ�����ʱ��
#define	MASTER_STATE_EX_CTR		8//��������״̬


#define SLAVE_NUMBER			19//�ӻ�վ��
#define INPUT_STATE_EX_CTR		20//����������״̬
#define OUTPUT1_STATE_EX_CTR	21//���������1״̬
#define OUTPUT2_STATE_EX_CTR	22//���������2״̬
#define AD0_EX_CTR				23//AD0��ֵ
#define AD1_EX_CTR				24//AD1��ֵ
#define AD2_EX_CTR				25//AD2��ֵ
#define AD3_EX_CTR				26//AD3��ֵ



extern u8 CommSetFlag ;
extern u8 sys_setRunPage;
//-------------------------------------------------------
//��ʼ��ͨ������
extern void InitComm0Data(void);
//ͨ�Ź��̴���
extern void EXTEND_COMController(void);
extern void EXTEND_ComModule(void);
extern void VGUS_ComCtr(void);
extern void EXTEND_ComModule11(void);


#endif



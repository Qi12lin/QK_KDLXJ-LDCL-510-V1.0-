#include "typedef.h"

#ifndef _TIMER_DRIVER_H
#define _TIMER_DRIVER_H

#define FLICKER 2000
//#define flicker 1000
extern u16 T_flicker;
extern u16 X3_counter;
extern u16 glue_run_pulse_c1,glue_stop_pulse_c1,glue_run_pulse_c2,glue_stop_pulse_c2;
extern u8 glue_run_flag1,glue_run_flag2;
typedef struct
{
	volatile uint16_t count;	/* ����ֵ */
	volatile uint16_t number;	/* ��ʱֵ */
	volatile uint8_t enable;	/* ��ʱʹ�� */
	volatile uint8_t flag;		/* ��ʱ�����־ */
}TMR_10MS;
typedef struct
{
	volatile uint16_t count;	/* ����ֵ */
	volatile uint16_t number;	/* ��ʱֵ */
	volatile uint8_t enable;	/* ��ʱʹ�� */
	volatile uint8_t flag;		/* ��ʱ�����־ */
}TMR_100MS;
typedef struct
{
	volatile uint16_t count;	/* ����ֵ */
	volatile uint16_t number;	/* ��ʱֵ */
	volatile uint8_t enable;	/* ��ʱʹ�� */
	volatile uint8_t flag;		/* ��ʱ�����־ */
}TMR_1S;
//---------------------------------------------------------
#define NO_10MS	2


extern TMR_10MS time_10ms[NO_10MS];//����־λ��ʱ��
#define ADDKEY_TIMER   time_10ms[0]//�Ӳ�����ʱ��
#define DECKEY_TIMER   time_10ms[1]//��������ʱ��
//---------------------------------------------------------
#define NO_100MS	10
#define DEVICE_A_TIMER		0//�豸A��������ʱ��
#define DEVICE_B_TIMER		1//�豸B��������ʱ��
#define DEVICE_C_TIMER		2//�豸C��������ʱ��
#define ALARM_DISPLAY		3//������Ϣ����
#define DEVICE_A_4_TIMER		4//�豸A���ĸ߻�������ʱ��
extern TMR_100MS time_100ms[NO_100MS];//����־λ��ʱ��
//---------------------------------------------------------
#define NO_1S	3
#define OFF_DELAY_TIMER		0
extern TMR_1S time_1s[NO_1S];
//---------------------------------------------------------
#define	TIMER_1MS_MAX_NUM		8	//1ms�Զ��ݼ���ʱ��
#define WAIT_TIMER					0//ms�ȴ�������
#define EXTEND_COM_TIMER			1//��չģ�鱨����־
#define GJX_SEND_DELAY				2//��Ƶ�����ͼ��ʱ��
#define DGUS_SEND_DELAY				3//DGUSͨ�ż��ʱ��

#define DOOROPENTIME1_1MS  Timer_1MS[4]
#define DOOROPENTIME2_1MS  Timer_1MS[5]
#define UPDATATEMP_1MS     Timer_1MS[6]
#define OVERTEMP_1MS       Timer_1MS[7]
extern INT16U	Timer_1MS[TIMER_1MS_MAX_NUM];
//---------------------------------------------------------
#define	TIMER_1MS_32_MAX_NUM		5	//1ms�Զ��ݼ���ʱ��(32λ)

extern uint32_t	Timer_1MS_32[TIMER_1MS_32_MAX_NUM];

//---------------------------------------------------------
#define	TIMER_100MS_MAX_NUM		5
#define SWITCH_DELAY_START			0//ѹ���������ʱ����
#define SWITCH_DELAY_STOP			1//ѹ���ֹͣ��ʱ����
#define SENSOR_DELAY_START			2//ѹ�����������ʱ����
#define SENSOR_DELAY_STOP			3//ѹ�����ֹͣ��ʱ����

extern INT16U	Timer_100MS[TIMER_100MS_MAX_NUM];
//---------------------------------------------------------
#define	TIMER_1S_MAX_NUM		11
#define DELAY_ACT				0//�ϵ���ʱ����
#define CLEARNING_DELAY			1//����ģʽ��ʱ���
#define POWER_FAILURE			2//��Դ���ϱ���
#define OVERLOAD_OF_FAN			3//������ر���
#define PAIL_FULL				4//��Ͱ��������
#define OVER_TEMP				5//���±���
#define PRE_UP_ALARM			6//ѹ�����ޱ���
#define BATTERY_PULL_OUT_DELAY	7//��ʱ�жϵ���Ƿ�γ�
#define U_DISC_CONNECT			8//U�̼��ʱ��
#define REMAINING_TIMER			9//ʣ��ʱ�䶨ʱ��
#define UNQUALIFIED_OXYGEN_DELAY	10//���ϸ�����
extern INT16U	Timer_1S[TIMER_1S_MAX_NUM];
extern u8 short_key_time,short_key_flag;
//---------------------------------------------------------
//��ʱ����չ
typedef struct
{
	vu16 enable;  		//ʹ�ܱ�ʶλ
	vu16 signal;   	//�ź�
	vu16 Timer;     //ʱ��
	vu16 counter;   //������
	vu16 tNum;      //��ʱ����	
}TIME_TYPE;


extern __IO int32_t g_iRunTime;
extern uint16_t count_100ms, count_1s;
extern uint32_t SSR8_ration_pulse;
extern u32 bsp_GetRunTime(void);
extern void bsp_InitTimer(void);   //��ʼ��Ӳ����ʱ��
extern void bsp_DelayMS(uint16_t len);	//�ӳٺ���
extern void SysTick_ISR(void);
/* ���ⲿ���õĺ������� */
extern void Delay_us(u32 nus);
extern void Delay_ms(u32 nms);
extern void Bsp_Control_10ms(void);
extern void Bsp_Control_100ms(void);
extern void Bsp_Control_1s(void);
extern void Bsp_Control_20ms(void);
extern void Bsp_Control_200ms(void);
extern void Bsp_Control_2s(void);

extern void TIME2_ISR(void);
extern void TIM3_Init(void);
extern void TIME3_ISR(void);
extern void TIME4_ISR(void);

#endif



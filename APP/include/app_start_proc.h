#include "typedef.h"

#ifndef _START_PROC_H
#define _START_PROC_H (1)

#define	COILREG_MAX_NUM		12

//#define HALL_YES  2 
//#define HALL_NO   1

#define HIGHSET_ENABLE  2
#define HIGHSET_DISABLE 1
extern uint16_t glue_pluse1[],glue_pluse2[],glue_pluse0[],glue_pluse3[],product_length_pluse[];
extern uint8_t loop_flag[5];
extern vu8 getStartKey ,getStopKey ;
extern void calcAllRunFreq(u16 _setSpeed,u8 _Acc,u8 _Dec);

#define QK_VFD 1   //��Ƶ
#define QK_WS  2   //��ˢ
#define VFD    3   //ת�Ӱ���Ʊ�Ƶ��
//����ģʽ
typedef enum
{
	STOP      = (u8)0x00,      //ֹͣģʽ
	RUN_UP   = (u8)0x01,      //��������ģʽ
	RUN_DOWN = (u8)0x02,    //��������ģʽ
	RUN = (u8)0x03,    //�ȶ�����ģʽ
	RUN_LOW  = (u8)0x04,    //��������ģʽ
	POINTRUN_MODE  = (u8)0x05,    //�㶯ģʽ
	ZZSB_MODE      = (u8)0x06,     //�Զ�ʶ��ģʽ
	DC_STOP    = (u8)0x07,     //ֱ���ƶ�ģʽ
	ALL_MODE       = (u8)0x08  //���е�ģʽ
}RUN_MODE;
extern RUN_MODE m_runState;//����״̬����
//���й����е�Ƶ�ʲ���
typedef struct
{
	vu16 targetFreq;  //Ŀ��Ƶ��
	vu16 maxFreq   ;  //���Ƶ��
	vu16 upFreqInc;   //��������
	vu16 downFreqInc; //��������
}FREQUENCE_PRARM;

extern FREQUENCE_PRARM allFreq;//���й����е�����Ƶ�ʲ���

//���й��̱�־λ
typedef struct
{
	u8 transRunFlag:1;  //��Ƶ�����б�־λ
	u8 motorRunFlag:1;  //������б�־λ
	u8 curRunFlag:1;     //��ʾ�����б�־λ
	u8 arriveTargetFlag:1;  //����Ŀ��Ƶ�ʱ�־λ
	u8 arriveSpeedFlag:1;  //����Ŀ��ת�ٱ�־
	u8 timerCouter:1;      //��ʱ������
	u8 realSpeedFlag:1;
	u8 shut_run:1     ;    //�㶯��׼
	u8 shut_run_stop:1;     //�㶯ֹͣ
}RUN_FLAG;

//���й����еĿ��Ƹ�������
typedef struct
{
	vu8 startSignal;//�����ź�
	vu8 OUT0;//���״̬��Ϣ
	vu16 PWM0h_Freq;   //ͨ���õ�Ƶ��
	vu16 waitTimeToTFreq;  //�ȴ������趨Ƶ��
	vu16 TS_FreqInc;  //���ٹ����е�Ƶ������

	vu8 stop_mode;//ͣ��ģʽ
	vu8 TxStopMode;
  
}CONTROL_DATA;
extern CONTROL_DATA  controlData;  //��������

//���Ļ�ʵ�����в���  
typedef struct
{
	vu16 Speed; //ת��
	vu16 RCF;		//������
	
	vu16 time_HH;//ʱ�� ʱ����
	vu16 time_MM;   
	vu16 time_SS;  
	
	vs16  temprature;     //�¶�   
	
	vu16 run_state;//����״̬ 1006
	vu16 door_state;//�Ÿ�״̬
	vu16 alarm;     //������Ϣ
	vu16 Freq;     //ʵ������Ƶ��
	vu16 SpeedRCF; //ʵ��ת�ٻ������� 100A
	vu16 zzsb_angle;//ʶ�������ת�ӽǶ�
	vu16 state_alarm; //����״̬�ͱ�����Ϣ
	vu16 RCF_flag;//  ��������ʾ��־
  vu16 BIT_state;  //λ������ʾ 100E
	vu16 motorState;// �������״̬ 100F
	vu16 MaxSpeed;  //ת�Ӷ�Ӧ�����ת��
	vu16 MaxRCF;   //ת�Ӷ�Ӧ�����������
	
		 //mpu6050����������
		vs16 ACCEL_X;//���ٶȴ���������  1012
		vs16 ACCEL_Y;
		vs16 ACCEL_Z;

		vs16 GYRO_X;//�����Ǵ���������   1015
		vs16 GYRO_Y;
		vs16 GYRO_Z;

		u16 MAX_BPH;   //1018
		u16 ax_std;
		u16 gx_mean;
		u16 gx_std;

		u16 amplitude;  // ���ֵ   101C
		u16 amplitudeMean; 
		u16 BPH_value;   

}LXJ_RUNPARAM;

extern LXJ_RUNPARAM runParam;

typedef struct 
{
	vu16 start5AA5;
	vu16 Number;
	vu16 Curve1;
	vu16 Data11;
	vu16 Data12;
	vu16 Curve2;
	vu16 Data21;
	vu16 Data22;
	vu16 Curve3;
	vu16 Data31;
	vu16 Data32;
	vu16 Curve4;
	vu16 Data41;
	vu16 Data42;
	vu16 Curve5;
	vu16 Data51;
	vu16 Data52;
	vu16 Curve6;
	vu16 Data61;
	vu16 Data62;
	vu16 Curve7;
	vu16 Data71;
	vu16 Data72;
	vu16 Curve8;
	vu16 Data81;
	vu16 Data82;
}CURVEDATA;
extern CURVEDATA CurveData;
typedef struct 
{
	vu16 RTC_Year;
	vu16 RTC_Month;
	vu16 RTC_Date;
	vu16 RTC_Hours;
	vu16 RTC_Minutes;
	vu16 RTC_Seconds;
	vu16 RTC_WeekDay;
}RUNDATA;
extern RUNDATA runData;
typedef struct 
{
	u16 uiAllRunTime_HH;    //�ܻ� �ۼ�����ʱ��
	u16 uiAllRunTime_MM;
	u16 uiAllRunTime_SS;
	
	u16 uiAllRotorRunTime_HH[25];    //ת�� �ۼ�����ʱ��Ԥ��25��ת��
	u16 uiAllRotorRunTime_MM[25];
	u16 uiAllRotorRunTime_SS[25];
	
}LXJ_ALLRUNTIME;  //���Ļ��ۼ�����ʱ��
extern LXJ_ALLRUNTIME AllRunTime;

#define MOTOR_BASE_FREQ (HldReg[BASE_F])
#define RATE_CURRENT    (HldReg[RATED_CURRENT])

//����Ŀ��ת�ٱ�ʶ

#define ARRVIE_SPEED_FLAG  		runFlag.arriveSpeedFlag // 	 runFlag.curRunFlag
extern u8 enable_RunTime;

extern RUN_FLAG runFlag;//���б�־λ
#define INIT_BASE_F 90

#define STOP_ON  			controlData.OUT0  &= 0xdf;controlData.OUT0 |= 0x01  //����ֹͣ
#define STOP_OFF  			controlData.OUT0 &= 0xFE //����ֹͣλ��0
#define MOTOR_DEAD_ON		controlData.OUT0 |= 0x02  //�����ת
#define MOTOR_DEAD_OFF  controlData.OUT0 &= 0xFD  //�����תλ��0
#define Y4_ON           controlData.OUT0 |= 0x04        //�䶳
#define Y4_OFF          controlData.OUT0 &= 0xFB  
#define Y4              ((controlData.OUT0 & 0x04) >> 2)//��ȡ��ŷ�״̬
#define DOOR_ON         controlData.OUT0 |= 0x08        //����
#define DOOR_OFF        controlData.OUT0 &= 0xF7
#define DOOR            ((controlData.OUT0 & 0x08) >> 3)  //���õ�����״̬
#define START_ON        controlData.OUT0  |= 0x20          //��������
#define START_OFF       controlData.OUT0  &= 0xdf
#define START_STATE     (controlData.OUT0 & 0x20)		   //����״̬λ

//ѹ��������λ
#define makeCold()      (Y4_ON)      //����
#define makeHot()       (Y4_OFF)     //����


//�Ÿ�δ���ź��ź�1Ϊû�� | 0Ϊ��
//#define DOOR_CLOSURE ( (OldReceStr[1] & 0x20) >>5 )
extern vu8 DOOR_CLOSURE;
//�����ź�
//#define GATE_LOCK    ( (OldReceStr[1] & 0x02) >>1 )
#define GATE_LOCK    ( (OldReceStr[1] & 0x08) >>3 )

extern u16 watch_CurSpeed;     //ʵ������ת��
extern u8 g_ucfDispXg; //������ת��
//��������
typedef enum
{
	NO_ALARM        =   (u8)0x00, //û�о���
	IMBALANCE_ALARM =  (u8)0x01, //��ƽ�ⱨ��
	OVERSPEED_ALARM = (u8)0x02, //���ٱ���
	GATELOCK_ALARM1 = (u8)0x03, //��������1
	GATELOCK_ALARM2 = (u8)0x04, //��������2 
	OVERHEAT_ALARM  = (u8)0x05, //���±���
	ROTOR_ERROR_ALARM = (u8)0x06, //ת��ʶ����󱨾�
	MOTOR_NOTURN_ALARM = (u8)0x07, //�����ת����
	COMCN_ERROR_ALARM = (u8)0x08,  //ͨ�Ź��ϱ���
	MODULE_ERROR_ALARM = (u8)0x09, //ģ����ϱ���
	OVERCURRENT_ALARMA = (u8)0x0A, //�������� �����٣�
	OVERCURRENT_ALARMB = (u8)0x0B, //��������  (����)
	OVERVOLTAGE_ALARM  = (u8)0x0C, //��ѹ����
	LACKVOLAGE_ALARM   =(u8)0x0D,   //Ƿѹ����
	HALL_ALARM,
	OPERATOR_ALARM,
	VFD_COMM_ALARM ,
	VFD_ALARM, //��Ƶ������
	OVERTEMP_ALARM,        //���±���
	IMBALANCE_ERROR_ALARM,  //��ƽ����ϱ���
}ALARM_TYPE;


//��ȡ��ǰ������״̬
extern RUN_MODE GetCurRunState(void);

//���õ�ǰ������״̬
extern void SetRunState(RUN_MODE state);

//����Ŀ��Ƶ��
//speed����ת��
extern u16 calcTargetFreq(u16 speed);
extern u16 GetFKSpeed(u8 Hall_mode);
//�������Ƶ��
//maxSpeed����ת��
extern u16 calcMaxFreq(u16 maxSpeed);

extern void runTimeContor(void);
void MOTOR_control(u16 _setSpeed,u16 _watchSpeed);

extern u8 sys_startrun;

extern u16 set2;
extern uint8_t lock_value[];
extern uint8_t lock_flag;
extern u8 Remaining_Day;
extern uint8_t read_time_flag;
extern uint8_t beep_on_flag;
extern int16_t offset_position1,offset_length1,offset_position2,offset_length2;
extern int16_t offset_position0,offset_length0,offset_position3,offset_length3;
extern int16_t offset_position1_pls,offset_length1_pls,offset_position2_pls,offset_length2_pls;
extern int16_t offset_position0_pls,offset_length0_pls,offset_position3_pls,offset_length3_pls;
extern uint8_t VFD_rev_flag,VFD_rev_flag2,VFD_rev_flag3;
//���й���
extern void Run_Proc(void);
//extern void Code_Analysis(void);
//void Cal_Lock_Date(void);
//extern void Save_Date(void);

extern void InitTotalTime(void);

extern void SetAlarmFlag(ALARM_TYPE type);
extern void Lock_Analysis(void);
extern void ShutDown(void);
extern ALARM_TYPE GetAlarmFlag(void);
#endif



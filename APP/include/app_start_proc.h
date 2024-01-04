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

#define QK_VFD 1   //变频
#define QK_WS  2   //无刷
#define VFD    3   //转接板控制变频器
//运行模式
typedef enum
{
	STOP      = (u8)0x00,      //停止模式
	RUN_UP   = (u8)0x01,      //升速运行模式
	RUN_DOWN = (u8)0x02,    //降速运行模式
	RUN = (u8)0x03,    //稳定运行模式
	RUN_LOW  = (u8)0x04,    //滑行运行模式
	POINTRUN_MODE  = (u8)0x05,    //点动模式
	ZZSB_MODE      = (u8)0x06,     //自动识别模式
	DC_STOP    = (u8)0x07,     //直流制动模式
	ALL_MODE       = (u8)0x08  //所有的模式
}RUN_MODE;
extern RUN_MODE m_runState;//运行状态变量
//运行过程中的频率参数
typedef struct
{
	vu16 targetFreq;  //目标频率
	vu16 maxFreq   ;  //最高频率
	vu16 upFreqInc;   //加速增量
	vu16 downFreqInc; //减速增量
}FREQUENCE_PRARM;

extern FREQUENCE_PRARM allFreq;//运行过程中的所有频率参数

//运行过程标志位
typedef struct
{
	u8 transRunFlag:1;  //变频器运行标志位
	u8 motorRunFlag:1;  //电机运行标志位
	u8 curRunFlag:1;     //显示板运行标志位
	u8 arriveTargetFlag:1;  //到达目标频率标志位
	u8 arriveSpeedFlag:1;  //到达目标转速标志
	u8 timerCouter:1;      //定时器计数
	u8 realSpeedFlag:1;
	u8 shut_run:1     ;    //点动标准
	u8 shut_run_stop:1;     //点动停止
}RUN_FLAG;

//运行过程中的控制辅助参数
typedef struct
{
	vu8 startSignal;//启动信号
	vu8 OUT0;//输出状态信息
	vu16 PWM0h_Freq;   //通信用的频率
	vu16 waitTimeToTFreq;  //等待到达设定频率
	vu16 TS_FreqInc;  //调速过程中的频率增量

	vu8 stop_mode;//停车模式
	vu8 TxStopMode;
  
}CONTROL_DATA;
extern CONTROL_DATA  controlData;  //控制数据

//离心机实际运行参数  
typedef struct
{
	vu16 Speed; //转速
	vu16 RCF;		//离心力
	
	vu16 time_HH;//时间 时分秒
	vu16 time_MM;   
	vu16 time_SS;  
	
	vs16  temprature;     //温度   
	
	vu16 run_state;//运行状态 1006
	vu16 door_state;//门盖状态
	vu16 alarm;     //报警信息
	vu16 Freq;     //实际运行频率
	vu16 SpeedRCF; //实际转速或离心力 100A
	vu16 zzsb_angle;//识别出来的转子角度
	vu16 state_alarm; //运行状态和报警信息
	vu16 RCF_flag;//  离心力显示标志
  vu16 BIT_state;  //位变量显示 100E
	vu16 motorState;// 电机运行状态 100F
	vu16 MaxSpeed;  //转子对应的最大转速
	vu16 MaxRCF;   //转子对应的最大离心力
	
		 //mpu6050传感器数据
		vs16 ACCEL_X;//加速度传感器数据  1012
		vs16 ACCEL_Y;
		vs16 ACCEL_Z;

		vs16 GYRO_X;//陀螺仪传感器数据   1015
		vs16 GYRO_Y;
		vs16 GYRO_Z;

		u16 MAX_BPH;   //1018
		u16 ax_std;
		u16 gx_mean;
		u16 gx_std;

		u16 amplitude;  // 振幅值   101C
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
	u16 uiAllRunTime_HH;    //总机 累计运行时间
	u16 uiAllRunTime_MM;
	u16 uiAllRunTime_SS;
	
	u16 uiAllRotorRunTime_HH[25];    //转子 累计运行时间预留25个转子
	u16 uiAllRotorRunTime_MM[25];
	u16 uiAllRotorRunTime_SS[25];
	
}LXJ_ALLRUNTIME;  //离心机累计运行时间
extern LXJ_ALLRUNTIME AllRunTime;

#define MOTOR_BASE_FREQ (HldReg[BASE_F])
#define RATE_CURRENT    (HldReg[RATED_CURRENT])

//到达目标转速标识

#define ARRVIE_SPEED_FLAG  		runFlag.arriveSpeedFlag // 	 runFlag.curRunFlag
extern u8 enable_RunTime;

extern RUN_FLAG runFlag;//运行标志位
#define INIT_BASE_F 90

#define STOP_ON  			controlData.OUT0  &= 0xdf;controlData.OUT0 |= 0x01  //设置停止
#define STOP_OFF  			controlData.OUT0 &= 0xFE //设置停止位清0
#define MOTOR_DEAD_ON		controlData.OUT0 |= 0x02  //电机不转
#define MOTOR_DEAD_OFF  controlData.OUT0 &= 0xFD  //电机不转位清0
#define Y4_ON           controlData.OUT0 |= 0x04        //冷冻
#define Y4_OFF          controlData.OUT0 &= 0xFB  
#define Y4              ((controlData.OUT0 & 0x04) >> 2)//获取电磁阀状态
#define DOOR_ON         controlData.OUT0 |= 0x08        //门锁
#define DOOR_OFF        controlData.OUT0 &= 0xF7
#define DOOR            ((controlData.OUT0 & 0x08) >> 3)  //设置的门锁状态
#define START_ON        controlData.OUT0  |= 0x20          //设置启动
#define START_OFF       controlData.OUT0  &= 0xdf
#define START_STATE     (controlData.OUT0 & 0x20)		   //启动状态位

//压缩机控制位
#define makeCold()      (Y4_ON)      //制冷
#define makeHot()       (Y4_OFF)     //制热


//门盖未关信号信号1为没关 | 0为关
//#define DOOR_CLOSURE ( (OldReceStr[1] & 0x20) >>5 )
extern vu8 DOOR_CLOSURE;
//门锁信号
//#define GATE_LOCK    ( (OldReceStr[1] & 0x02) >>1 )
#define GATE_LOCK    ( (OldReceStr[1] & 0x08) >>3 )

extern u16 watch_CurSpeed;     //实际运行转速
extern u8 g_ucfDispXg; //离心力转换
//警报类型
typedef enum
{
	NO_ALARM        =   (u8)0x00, //没有警报
	IMBALANCE_ALARM =  (u8)0x01, //不平衡报警
	OVERSPEED_ALARM = (u8)0x02, //超速报警
	GATELOCK_ALARM1 = (u8)0x03, //门锁报警1
	GATELOCK_ALARM2 = (u8)0x04, //门锁报警2 
	OVERHEAT_ALARM  = (u8)0x05, //超温报警
	ROTOR_ERROR_ALARM = (u8)0x06, //转子识别错误报警
	MOTOR_NOTURN_ALARM = (u8)0x07, //电机不转报警
	COMCN_ERROR_ALARM = (u8)0x08,  //通信故障报警
	MODULE_ERROR_ALARM = (u8)0x09, //模块故障报警
	OVERCURRENT_ALARMA = (u8)0x0A, //过流报警 （升速）
	OVERCURRENT_ALARMB = (u8)0x0B, //过流报警  (运行)
	OVERVOLTAGE_ALARM  = (u8)0x0C, //过压报警
	LACKVOLAGE_ALARM   =(u8)0x0D,   //欠压报警
	HALL_ALARM,
	OPERATOR_ALARM,
	VFD_COMM_ALARM ,
	VFD_ALARM, //变频器报警
	OVERTEMP_ALARM,        //过温报警
	IMBALANCE_ERROR_ALARM,  //不平衡故障报警
}ALARM_TYPE;


//获取当前的运行状态
extern RUN_MODE GetCurRunState(void);

//设置当前的运行状态
extern void SetRunState(RUN_MODE state);

//计算目标频率
//speed设置转速
extern u16 calcTargetFreq(u16 speed);
extern u16 GetFKSpeed(u8 Hall_mode);
//计算最高频率
//maxSpeed最大的转速
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
//运行过程
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



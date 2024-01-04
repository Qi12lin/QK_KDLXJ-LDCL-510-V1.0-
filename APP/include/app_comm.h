#include "typedef.h"

#ifndef _COMM_PROC_H
#define _COMM_PROC_H (1)
//-------------------------------------------------------
//MODBUS主站参数
//-------------------------------------------------------
#define	HLDREG_MAX_NUM		1100
#define	HLDREG_MAX_ADR		(HLDREG_MAX_NUM-1)
extern uint16_t	HldReg[HLDREG_MAX_NUM];		//4x 1-100	--cmd 16/3	(W/R)保持寄存器	

//0~0x49(57)为断电不保持数据 
//0~     上传到屏显示
//typedef enum
//{
//	RUN_SPEED  =0x1000,
//	RUN_RCF,
//	RUN_TIME_HH,
//	RUN_TIME_MM,
//	RUN_TIME_SS,
//	RUN_TEMPERATURE,       //实际运行参数
//	RUN_STATE,
//	ALARM,
//	DOOR_STATE,
//	
//	MAX_RUN_NUMBER     //运行参数个数   注意不能与控制命令的地址重叠
//	
//}RUN_DATA;
//   ~0x4F  不上传到屏显示 接受屏的控制信号
typedef enum
{
	SHORT_KEY=0x1042,     //点动键  0x28 0x29中间值
	RCF_KEY =0x1043 ,    //离心力切换键
	CLEAR_ALLRUNTIME=0x1044,
	STOPATART_KEY =0x1045,
	CLEAR_ALARM =0x1046,
	DOOR_KEY =0x1047,
	STOP_KEY,
	START_KEY =0x1049,
}RUN_CONTORL;


//0x50-0xFF为断电保存且发送到屏上显示的参数  运行设置参数和高级设置参数

typedef enum
{
	START_SET_NUMBER =0,      //设置参数个数 MAX_SET_NUMBER - START_SET_NUMBER+1
	PROGRAM  =0,
	ROTOR,
	SET_SPEED,
	SET_RCF,
	SET_TIME_HH,
	SET_TIME_MM,
	SET_TIME_SS,
	SET_TEMPERATURE,       //运行设置参数
  ACC,
	DEC ,
	SET_SPEEDRCF =0x0a,      //转速离心力
	
	SET_CLOSE_TEMP,  //  关闭制冷温度
	SET_OPEN_TEMP,   //开启制冷温度
	SET_hot_time,
	SET_cool_time,
	
	BASE_F =0x10,      //高级设置参数
	RATED_CURRENT,
	SYSLOCK_TIME,
	SYS_TYPE,   
	LOCK_TYPE,
	AUTO_ROTOR=0x15,
	BPH_SET,
	LOCK_PROTECT,
	LANGUAGE,
	HALL_EN,
	
	MOTOR_TYPE,   //电机控制类型   0：变频器控制变频电机    1：无刷电机   2：四方变频器控制
	VOLTAGE,      //电源电压       0:220V   1：110V
	DOOROPEN_TIME,    //开锁时间
	AUTOOPEN_EN=0x1D,     //停机自动开门 
	SAVE_MIMA_H,      //保存的密码 占两个字节
	SAVE_MIMA_L,      //保存的密码
	TEMPDISP,         //温度显示滤波系数 0x20
	SET_STATRPAGE =0x21,   //开机界面设置
	SET0 =0x28,
	SET1 =0x29,
	MAX_SET_NUMBER,   
	
	
}SAVE_RUN_DATA;
	

typedef enum
{
	IN_MIMA_H=0x50,  //输入密码
	IN_MIMA_L,
	IN_MIMA1_H,
	IN_MIMA1_L,      //修改密码时输入密码1
	IN_MIMA2_H,
	IN_MIMA2_L,      //修改密码时输入密码2
	
	UPDATA_YEAR =0x56,
	UPDATA_MONTH,
	UPDATA_DATA,
	UPDATA_HOURS,
	UPDATA_MINUTES,
}GET_DATA;
//接受到屏的数据 不保存



#define CHANGE_PAGE				827		//切换页面信息

#define HIGHSET_PAGE      3  //高级参数设置页面
//-------------------------------------------------------
//100-199为断电保持且发送到屏上显示的参数
//-------------------------------------------------------








//-------------------------------------------------------
//MODBUS从站参数
//-------------------------------------------------------
#define	SLAVE_HLDREG_MAX_NUM		30
#define	SLAVE_HLDREG_MAX_ADR		(SLAVE_HLDREG_MAX_NUM-1)
extern uint16_t	SlaveHldReg[SLAVE_HLDREG_MAX_NUM];		//西门子地址40001对应地址0
#define	NUMBER_OF_VALUES_USED_EX_CTR	0//阀门使用数量
#define	CLEANING_INTERVAL_EX_CTR		1//清灰间隔时间
#define	CLEANING_PULSE_WIDTH_EX_CTR	2//清灰脉宽
#define	OFFLINE_CLEANING_TIME_EX_CTR	3//离线清灰时间
#define	CLEANING_MODE_EX_CTR			4//清灰模式，0-常规,1-压差

#define	CLEANING_CYCLE_INTERVAL_EX_CTR		7//清灰循环间隔时间
#define	MASTER_STATE_EX_CTR		8//主机运行状态


#define SLAVE_NUMBER			19//从机站号
#define INPUT_STATE_EX_CTR		20//开关量输入状态
#define OUTPUT1_STATE_EX_CTR	21//开关量输出1状态
#define OUTPUT2_STATE_EX_CTR	22//开关量输出2状态
#define AD0_EX_CTR				23//AD0数值
#define AD1_EX_CTR				24//AD1数值
#define AD2_EX_CTR				25//AD2数值
#define AD3_EX_CTR				26//AD3数值



extern u8 CommSetFlag ;
extern u8 sys_setRunPage;
//-------------------------------------------------------
//初始化通信数据
extern void InitComm0Data(void);
//通信过程处理
extern void EXTEND_COMController(void);
extern void EXTEND_ComModule(void);
extern void VGUS_ComCtr(void);
extern void EXTEND_ComModule11(void);


#endif



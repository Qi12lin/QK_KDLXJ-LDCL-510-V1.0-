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
	volatile uint16_t count;	/* 计数值 */
	volatile uint16_t number;	/* 定时值 */
	volatile uint8_t enable;	/* 定时使能 */
	volatile uint8_t flag;		/* 定时到达标志 */
}TMR_10MS;
typedef struct
{
	volatile uint16_t count;	/* 计数值 */
	volatile uint16_t number;	/* 定时值 */
	volatile uint8_t enable;	/* 定时使能 */
	volatile uint8_t flag;		/* 定时到达标志 */
}TMR_100MS;
typedef struct
{
	volatile uint16_t count;	/* 计数值 */
	volatile uint16_t number;	/* 定时值 */
	volatile uint8_t enable;	/* 定时使能 */
	volatile uint8_t flag;		/* 定时到达标志 */
}TMR_1S;
//---------------------------------------------------------
#define NO_10MS	2


extern TMR_10MS time_10ms[NO_10MS];//带标志位定时器
#define ADDKEY_TIMER   time_10ms[0]//加操作定时器
#define DECKEY_TIMER   time_10ms[1]//减操作定时器
//---------------------------------------------------------
#define NO_100MS	10
#define DEVICE_A_TIMER		0//设备A阀动作定时器
#define DEVICE_B_TIMER		1//设备B阀动作定时器
#define DEVICE_C_TIMER		2//设备C阀动作定时器
#define ALARM_DISPLAY		3//报警信息滚动
#define DEVICE_A_4_TIMER		4//设备A阀四高机动作定时器
extern TMR_100MS time_100ms[NO_100MS];//带标志位定时器
//---------------------------------------------------------
#define NO_1S	3
#define OFF_DELAY_TIMER		0
extern TMR_1S time_1s[NO_1S];
//---------------------------------------------------------
#define	TIMER_1MS_MAX_NUM		8	//1ms自动递减定时器
#define WAIT_TIMER					0//ms等待函数用
#define EXTEND_COM_TIMER			1//扩展模块报警标志
#define GJX_SEND_DELAY				2//变频器发送间隔时间
#define DGUS_SEND_DELAY				3//DGUS通信间隔时间

#define DOOROPENTIME1_1MS  Timer_1MS[4]
#define DOOROPENTIME2_1MS  Timer_1MS[5]
#define UPDATATEMP_1MS     Timer_1MS[6]
#define OVERTEMP_1MS       Timer_1MS[7]
extern INT16U	Timer_1MS[TIMER_1MS_MAX_NUM];
//---------------------------------------------------------
#define	TIMER_1MS_32_MAX_NUM		5	//1ms自动递减定时器(32位)

extern uint32_t	Timer_1MS_32[TIMER_1MS_32_MAX_NUM];

//---------------------------------------------------------
#define	TIMER_100MS_MAX_NUM		5
#define SWITCH_DELAY_START			0//压差开关启动延时动作
#define SWITCH_DELAY_STOP			1//压差开关停止延时动作
#define SENSOR_DELAY_START			2//压差传感器启动延时动作
#define SENSOR_DELAY_STOP			3//压差传感器停止延时动作

extern INT16U	Timer_100MS[TIMER_100MS_MAX_NUM];
//---------------------------------------------------------
#define	TIMER_1S_MAX_NUM		11
#define DELAY_ACT				0//上电延时动作
#define CLEARNING_DELAY			1//常规模式延时清灰
#define POWER_FAILURE			2//电源故障报警
#define OVERLOAD_OF_FAN			3//风机过载报警
#define PAIL_FULL				4//灰桶已满报警
#define OVER_TEMP				5//过温报警
#define PRE_UP_ALARM			6//压力上限报警
#define BATTERY_PULL_OUT_DELAY	7//延时判断电池是否拔出
#define U_DISC_CONNECT			8//U盘检测时间
#define REMAINING_TIMER			9//剩余时间定时器
#define UNQUALIFIED_OXYGEN_DELAY	10//不合格氧气
extern INT16U	Timer_1S[TIMER_1S_MAX_NUM];
extern u8 short_key_time,short_key_flag;
//---------------------------------------------------------
//定时器扩展
typedef struct
{
	vu16 enable;  		//使能标识位
	vu16 signal;   	//信号
	vu16 Timer;     //时长
	vu16 counter;   //计数器
	vu16 tNum;      //计时次数	
}TIME_TYPE;


extern __IO int32_t g_iRunTime;
extern uint16_t count_100ms, count_1s;
extern uint32_t SSR8_ration_pulse;
extern u32 bsp_GetRunTime(void);
extern void bsp_InitTimer(void);   //初始化硬件定时器
extern void bsp_DelayMS(uint16_t len);	//延迟函数
extern void SysTick_ISR(void);
/* 供外部调用的函数声明 */
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



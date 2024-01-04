#include "typedef.h"

//
#ifndef _TEMP_H
#define _TEMP_H

//温度状态参数
typedef enum
{
	TEMP_NO = 0x00,             //无状态
	TEMP_UP = 0x01,             //上升
	TEMP_DOWN = 0x02,           // 下降
	TEMP_STABILITY = 0x03       //稳定状态
}TEMP_STATE;

//温度参数
typedef struct
{
	vu8  state;    //状态
	vu8 fCtrlSt; //控制开始标志
	vu8 CtrlCycle;     //周期
	vu8 fTarTempr;     //到达目标温度标志
	int realTemp;  //实时温度
	int targetTemp; //目标温度
}TEMP_PARAM;

//温度控制周期
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

//温度参数
extern TEMP_PARAM temper; //
extern TIME_TYPE temprTimer;

//显示温度
extern void DispTemperature(int t_tempr,u8 fDisp);
//设置状态温度的显示
extern void dispSetTempr(void);

//温度控制过程
extern void Temp_Proc(void);
//温度初始化
void TemprInit(void);
extern int temp_count(u16 ADC_bit);

extern u16 Y4_OFF_time,Y4_ON_time;
#endif


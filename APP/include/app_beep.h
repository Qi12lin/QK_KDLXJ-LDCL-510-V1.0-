#include "typedef.h"

#ifndef _BEEP_PROC_H
#define _BEEP_PROC_H

//蜂鸣器IO端口
#define BEEP_PORT GPIOC

//蜂鸣器IO引脚
#define BEEP_PIN  GPIO_Pin_5

//蜂鸣器时钟
#define	BEEP_RCC RCC_AHB1Periph_GPIOC //RCC_APB2Periph_GPIOC
 

//蜂鸣器信息结构体
typedef struct 
{
		vu16 beepOnTime;		//一个蜂鸣器响的周期内蜂鸣器on的计数器
		vu16 beepOffTime;					//一个蜂鸣器响的周期内蜂鸣器off的计数器
		vu16 beepCycleOnTime;     //一个周期内蜂鸣器开的时间
		vu16 beepCycleOffTime;		//一个周期内蜂鸣器关的时间
		vu16  cnt;									//响的次数   
		vu8  beepBusy;						//蜂鸣器忙标识
		vu8  beepSignal; 			//蜂鸣器信号
}BEEP_TYPE;

extern BEEP_TYPE beepInfo;

extern void BeepInit(void);    //蜂鸣器初始化
extern void Beep_SetTime(u16 cnt, u16 onTime, u16 offTime);
extern void Beep_RecordTimer(void); //蜂鸣器计时任务
extern void ClearBeep(void);    //清除蜂鸣器
extern u8 BeepBusy(void);
//蜂鸣器的处理过程
extern void Beep_Proc(void);


#endif



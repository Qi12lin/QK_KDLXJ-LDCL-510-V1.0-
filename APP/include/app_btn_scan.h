#include "typedef.h"

#ifndef _BUTTON_H
#define _BUTTON_H

//键盘宏定义

#define SET_KEY    VK_KEY7
#define ADD_KEY    VK_KEY8
#define SUB_KEY    VK_KEY9
#define ENTER_KEY  VK_KEY1
#define CUT_KEY    VK_KEY2
//#define AJ_STOP_KEY   VK_KEY10
//#define AJ_START_KEY  VK_KEY4
//#define SHORT_KEY  VK_KEY3
#define OPEN_KEY  VK_KEY3

//一组数码管对应的功能
typedef enum 
{
		GROUP_FRIST =0,
		GROUP_PROG =0	,  //程序组
		GROUP_ROTOR 	,  //转子号组
		GROUP_FORCE_SPEED ,  //离心力和转速组，累计离心积， 累计时间
		GROUP_TEMP,      //温度组   没有使用
		GROUP_HOUR,      //时间小时
		GROUP_TIME  	,  //时间组
		GROUP_SECOND  ,  //秒数
		
		GROUP_UPGEAR 	,  //升速档位组
		GROUP_DOWNGEAR ,  //降速档位组
		
		GROUP_ALL ,      //累计组数
		
		GROUP_FORCE,
	
}GROUPs;

extern vu8 DTLED_groud ; //数码管组
extern u8 DTLED_groud_set,DTLED_groud_set_no; 
//按键信号
typedef struct
{
	vu8 setSignal:1; 		//设置键信号
	vu8 addSignal:1; 		//加键信号
	vu8 subSignal:1; 		//减键信号
	vu8 enterSignal:1;	 //确定键信号
	vu8 cutSignal:1;   	//切换键信号
	vu8 openSignal:1; 	 //开门键信号
	vu8 startSignal:1;	 //启动键信号
	vu8 stopSignal:1;  	//停止键信号
	vu8 com1Signal:1;     //组合键信号
	vu8 com2Signal:1;     //组合键2信号
	vu8 timeSignal:1; 		//设置键信号
	vu8 rotorSignal:1; 		//加键信号
	vu8 speedSignal:1; 		//减键信号
	vu8 tempSignal:1;	 //确定键信号
}KEY_SIGNAL;

//按键信号
extern KEY_SIGNAL keySignal2;

//初始化键信号
extern void InitKeySignal(void);
//清确定键计时时间
extern void ClearEnterTimerC(void);

//按键扫描和处理
extern void Key_Proc(void);

extern vu8 keySET_State; //设置键状态

extern u8 If_key_press(u8 key);//不同键值是否按下
extern u8 IfStopPress(void);

//高级设置中的加键扫描
extern void HighADDKey_Scan(void);
//高级设置过程中减键过程
extern void HighSUBKey_Scan(void);
//设置键扫描
extern void SETKey_Scan(void);
//确定键扫描
extern void ENTERKey_Scan(void);

extern void LCD_DrawBMP_RotorMassege(uint16_t _usX, uint16_t _usY, char *_ptr);

//组合键响应
extern void Comb2_Scan(void);

extern void STOPKey_Scan(void);

#endif

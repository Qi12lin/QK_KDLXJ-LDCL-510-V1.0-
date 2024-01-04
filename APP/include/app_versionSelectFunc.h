
#include "typedef.h"

#ifndef _VERSION_SELECT_FUNC_H
#define _VERSION_SELECT_FUNC_H

typedef enum
{
	LY_TDL5M =1 ,
	

	
	VER_MAX_NUM,
		 AJY_A5K_A,
	 AJY_A6K  ,
	LY_TGL_16M,
	LY_TGL18M,
	LY_TGL20M,	
	
	XZ_P6_N,
	XZ_P5,
	DM_P48,
	DM_P80,
	XZ_TD6B,
XZ_TD6B_N,
	CX_TD6B,
	
	MC20000,
	
	MC5000S
	

}P_TYPE;
#define LOW_TYPE  1  //低速机型个数
//型号信息结构
typedef struct
{
	vu8 No;          //编号
  vu16 pSpeedTab[20]; //最大转速表
	vu16 pForceTab[20]; //最大离心力表
  vu8 RotorKind;  //转子种类
	vu8 maxCur;    //额定电流
	vu8 baseFreq;   //基频
}VERSION; 

//电机物理参数
typedef struct
{
	vu8 id;   					//编号
	vu8 Maxcur;		 			//额定电流
	vu16 baseFreq; 			//基频
	vu16 RatedPower; 		//额定功率
}MOTOR_PHYSICS_PARM;





//一种型号



//获取当前的版本号
//extern u8 GetCurrentVerNo(void);
//设置当前的版本号
//extern void SetCurrentVerNo(u8 ver);
//获取型号信号
extern void GetVersionInfo(void);
//型号的读取
extern void GetMemVerInfo(void);
//型号的保存
extern void SaveToMemVerInfo(void);

extern unsigned char get_rotor_N(unsigned char _type);
extern void get_rotor_info(unsigned char type,u16 *speed,u16 *force,unsigned char *rotorkind);
#endif

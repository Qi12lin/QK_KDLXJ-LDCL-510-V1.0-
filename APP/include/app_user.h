//#include "stm8s_lib.h"
#include "typedef.h"

#ifndef _USER_H_
#define _USER_H_

#define MAX_PROGRAM  40    //最大程序号   最大100组数据

//高级参数初始设置值
#define INIT_SYS_TYPE	       LY_TDL5M
#define INIT_LOCK_TYPE	 ELEC_LOCK_TYPE

#define INIT_BASE_F_HIGH  200//高速机基频  MC21000基频为120Hz
#define INTT_RATED_CURRENT_HIGH 17

#define INIT_BASE_F_LOW   85//低速机基频
#define INIT_RATED_CURRENT_LOW	 27

#define INIT_LANGUAGE 1       //1--中文  2--英文
#define INTI_LOCK_PROTECT 2   
#define INIT_HALL_EN 2
#define INIT_BPH_SET  2
#define INIT_AUTOOPEN_EN 1      //1:停机不自动开门 2：停机自动开门
#define INIT_AUTO_ROTOR  1      //转子识别  1--不识别  2 转子自动识别

#define INTI_MOTOR_TYPE  1 //电机控制类型   0：变频器控制变频电机    1：无刷电机   2：四方变频器控制
#define INIT_VOLTAGE    2  //电源电压       2:220V   1：110V
#define INIT_DOOROPEN_TIME  100

#define INIT_STARTPAGE 1

typedef struct
{
	vu8 ucRotorNo;   //转子号
	vu16 uiSpeed;
	vu16 uiRCF;
	vu8  ucTime_HH;
	vu16 uiTime_MM;
	vu8  ucTime_SS;
	s16  iTemprature;
	vu8  ucAcc;
	vu8  ucDec;
	vu8  ucBack1;  //备用1 2
	vu8  ucBack2;  
}PROGRAMDATA;  //供15字节
extern PROGRAMDATA  OneProgramData;   //一组程序数据
extern PROGRAMDATA  progData[10];    //一次读取10组数据
//档位类型
typedef enum
{
	UPGEAR_TYPE,
	DOWNGEAR_TYPE
}GEAR_TYPE;

#define MAX_ROTOR 20
typedef struct
{
//	vu16 uiSpeed[MAX_ROTOR];
//	vu16 uiRCF[MAX_ROTOR];
//	vu8  ucTime_HH[MAX_ROTOR];
//	vu16 uiTime_MM[MAX_ROTOR];
//	vu8  ucTime_SS[MAX_ROTOR];
//	vs16  iTemprature[MAX_ROTOR];
	vu8  ucAcc[MAX_ROTOR];
	vu8  ucDec[MAX_ROTOR];
}ROTORPROGRAMDATA;   //对应转子号保存的数据  预留25个转子号 有的不使用程序组的需要根据转子号保持参数即可 
extern ROTORPROGRAMDATA RotorPrgData;
//档位的种类
typedef enum
{
	SLOW_GEAR = 0x01,     //普通档位
	FAST_GEAR = 0x02          //快速档位
}GEAR_KIND;

extern GEAR_KIND fFast;

//转子最大数目
extern u8 RotorMaxNum ;
//最大额定电流的值
extern vu8 maxCurValue;
//离心力
extern u16 Force_max[];
//转速
extern u16 Speed_max[];
void read_eepromRotorProgranData(void);
//取转子对应的运行数据信息
void get_RotorProgranData( void );
//保存转子对应的运行数据信息
void set_RotorProgranData(void)	;
//读取一组数据
void read_eepromOneProgData(u8 _ProgNO);
//写入一组数据
void write_eepromOneProgData(u8 _ProgNO);

//取程序组数据到运行数据
void get_programData(PROGRAMDATA*  _ProgramData);	
//设置程序组数据   取运行数据到程序组数据
void set_programData(PROGRAMDATA*  _ProgramData);

//转子号最大值
#define ROTOR_MAX	RotorMaxNum
//转子最大转速
#define MAXROTORSPEED Speed_max[ HldReg[ROTOR]]

#define GEAR_NUM	10
#define TIMER_MAX 59


extern u8 setData_flag ; //数据有修改是才上传
//离心力转换为速度
extern u16 Force_to_Speed(u16 force,u8 rotorNo);
//速度转换为离心力
extern u16 Speed_to_Force(u16 speed, u8 rotorNo);
//获取最大的离心力和转速
extern u16 Fc_SpeedMax (u8 rotorNo, u8 fSpeed);
extern void SetRotor (void);
//所有保存的升降速档位
extern void GetAllGearFromMem(void);
//保存相应转子号的升降速档位
extern u8 SaveGear( u8 rotorNo);
//获取相应转子号的升降速档位
extern u8 GetGear( u8 rotorNo);
//获取所有的参数
extern void GetLXLParamFromMem(void);
//获取所有的参数
extern void GetDataFromMem(void);
//保存所有的档位信息
extern void SaveAllGear(void);
//保存离心力参数
extern void SaveLXLParam(void);
//设置过程中设置当前零时转子的档位
extern void SetGear(u8 rotorNo);
//获取档位对应的时间
extern u16 GetGearTime(u8 gear,u8 fFst);
//获取快速降速档位的时间
extern u16 GetGearFastTm(u8 gear);
//获取最大转速
extern u16 GetMaxSpeed(u8 rotor);



	//窗口显示
extern void Win_Proc(void);

extern void DTLEDS_DispProc(void);
//更新转子号数码管显示
extern void DTLEDs_UD_Rotor(u8 r, u8 fDisp);
//更新显示转速/离心力数码管显示
extern void DTLEDs_UD_SpOrFc(u16 sp, u8 fDispDot, u8 fDisp);
//更新时间显示
extern void DTLEDs_UD_Time(u8 time,u8 fDisp);
//更新升速/降速档位的显示
extern void DTLEDs_UD_Gear(u8 gear,u8 fDispDot,u8 fDisp);

extern void Read_eepromData(void);
extern void Save_eepromData(void);
extern void Maxmin_Param(u8 _type);
extern void write_eepromAllRunTime(void);

#endif



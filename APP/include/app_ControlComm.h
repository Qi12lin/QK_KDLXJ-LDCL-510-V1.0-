#include "typedef.h"

#ifndef _APP_CONTROLCOMM_H
#define _APP_CONTROLCOMM_H (1)


//通信数据的格式
//显示板发送的数据格式
typedef  struct 
{
	vu8 stationNo;  //站号
	vu8 datNum;     //数据个数,不包括站号，和数据个数
	vu8 stateFlag1;  //状态标志
	vu8 wayFlag;    // 
	vu8 freqH;         //显示板发送频率高8位
	vu8 freqL;         //显示板发送频率低8位
	vu8 speedIncH;    //加速增量高8位
	vu8 speedIncL;    //加速增量低8位   
	vu8 speedDecH;    //减速增量高8位
	vu8 speedDecL;    //减速增量低8位 		
	vu8 motorBasefreq; //电机基频低8位
	vu8 motorRatedCur; //电机额定电流
	vu8 ReserveByte;   //预留字节
	vu8 verifyValue;   //校验值
}COMM_TEND_FORMAT;

//显示板接收的数据格式
typedef  struct
{
	u8 stationNo;  //站号
	u8 datNum;     //数据个数
	u8 transState;  //变频器的状态标志
	u8 signalByte;  //信号，模块故障，门信号等
	u8 motorSpeedH; //电机速度的高8位
	u8 motorSpeedL; //电机速度的低8位
	u8 transFreqH;  //变频器当前输出频率高8位
	u8 transFreqL;  //变频器当前输出频率低8位
	u8 transAlarmInfo; //变频器报警信息
	u8 ReserveByte;   //预留字节
	u8 backRatedCur;  //反馈回来额定电流
	u8 ReserveByte2[2];   //预留字节
	u8 verifyValue;   //校验值
}COMM_RECE_FORMAT;


typedef  struct
{
		vu8 sendEnable:1; //发送使能位
		vu8 receEnable:1; //接收使能位
		vu8 comm_failure:1; //通信失败标志 
		vu8 receComplete:1; //接收完成标志
}COMM_FLAG;

extern COMM_FLAG comFlag;
//extern TIME_TYPE txTime; //发送数据定时器

//
#define SENDDELAYTIME  300  //通讯不上是间隔发送数据时间
#define COMMERRTIME 5000
extern u16 commTm;        //通讯故障判断定时器
extern u16 comSenderTm;   //定时发送定时器


//通信标志位
extern COMM_FLAG comFlag;
//响应通信故障标志位
extern vu8 commErrsignal;

extern vu8 tranStr[20]; //发送缓冲区
extern vu8 sendPos;
//接收缓冲区
extern u8 OldReceStr[];
extern vu8 receStr[20];
extern vu8 LRC; //校验数据
extern u16	upFreqInc1,downFreqInc1;  //二断加减速增量

//发送一个字节的数据
extern void Comm_Send_Byte(u8 byte);
//接收一个字节的数据
extern u8 Comm_Rece_Byte(u8 *byte);
//发送一块数据
extern void Comm_Send_Block(u8 * dat, u8 length);
//接收一块数据
extern void Comm_Rece_Block(void);
//5s定时器计时
extern void Counter_5s(void);
//200ms定时器计时
extern void Counter_200ms(void);
//初始化5s和200ms定时器
extern void TM_5s_200ms_Init(void);
//在定时器中发送一个字节的数据
extern void SendByte(void);

//获取反馈频率
extern u16 GetFKFreq(void);
//设置发送数据
extern void SetTxDat(void);
//发送数据的设置
extern void setSendData(void);
//初始化通信数据
extern void InitCommData(void);
//通信模块
extern void CommModule(void);

//通信过程处理
//comFlag.sendEnable
extern void Comm_Proc(u8 _ucfSending);

#endif



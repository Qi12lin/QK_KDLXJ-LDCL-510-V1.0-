/*
**************************************************************
		文件名：	app_comm.c
		功	 能：	实现与驱动板的通信的功能函数，定义通信
					协议。


**************************************************************
*/
#include "typedef.h"

vu8 tranStr[20]; //发送缓冲区
vu8 receStr[20];
u8 OldReceStr[20];
vu8 recePos;
vu8 sendPos = 0;

//响应通信故障标志位
vu8 commErrsignal;

vu8 LRC = 0; //校验数据

COMM_FLAG comFlag;
//接收数据个数
//vu16 receData = 0;

//运行过程中判断电机的转的定时器
//TIME_TYPE MotorTimer;

//主站站号
#define ADMIN_NO    (u8)0x01 
//从站站号
#define SLAVE_NO    (u8)0x01



u16 commTm = 5000;        //通讯故障判断定时器
u16 comSenderTm =600;   //定时发送定时器
//初始化通信数据





extern vu8 g_ucState;

//200ms定时器计时
void Counter_200ms(void)
{
	//200ms

	if(comSenderTm ==0)
	{
		Comm_Proc(1);	 		//通信过程	
		//发送一次数据

		comSenderTm = SENDDELAYTIME;
		recePos = 0;
	}
	else
	{
		comSenderTm--;
	}
	if(commTm >0) commTm--;  
	else comFlag.comm_failure =1;  //通讯失败标志
}


//通信过程处理

void Comm_Proc(u8 _ucfSending)
{
	if(_ucfSending)
	{
		Comm_Send_Block((u8 *)tranStr, tranStr[1]+3);
    }
}

extern int send_start_flag;
//发送一块数据
void Comm_Send_Block(u8 * dat, u8 length)
{
//	u8* pStr = (u8*)dat;

	sendPos = 1;

	setSendData(); 
	//if(door_on_flag >0) door_on_flag--;
	USART_SendData(UART4, (uint16_t)(tranStr[0]));
  doorcommsendFlag =0;

}

//接收一块数据
void Comm_Rece_Block(void)
{
	//定时接收一个字节数据，如果接收满了，发送一个接收完成标志
	vu8 dat;
	vu8 j,i;
	if( uart_Rece_Byte((u8 *)&dat) != 0)
	{
		receStr[recePos] = dat;
		recePos++;
		if(recePos >= 20) recePos = 0;
		if(recePos == receStr[1] + 3) // 数据接收完成
		{
			j = 0;
			for(i = 0; i < receStr[1] + 2; i++) //0~13
			{
				j += receStr[i];
			}
			LRC = ~j + 1;
			
			if(LRC == receStr[receStr[1]+2] && receStr[0] == ADMIN_NO)
			{
					//获取接收到的数据
				for(i = 0; i < receStr[1]; i++ ) OldReceStr[i] = receStr[i+2]; 
				j = 0;
				//设置接收的数据
//						for (i=0;i<250;i++);          // delay
	
				

				commTm = COMMERRTIME;
				comSenderTm = SENDDELAYTIME;
				comFlag.comm_failure =0;
				if(COMCN_ERROR_ALARM == runParam.alarm)  
				{
					runParam.alarm =0;	
//				TM_200ms = TM_200MS_LEN;
				}
			
				Comm_Proc(1);
			}
			recePos = 0;   //接收位置复位
		}
	 }
	 else
	 {
	 }
}

//设置发送增量
void SetTransInc(void)
{
	//判断是否到达频率
	if(runFlag.arriveTargetFlag)
	{
		controlData.OUT0 |= 0x10; //设置到达转速
		tranStr[6]  = 0;
		tranStr[7]  = 200;
		tranStr[8] =  0;
		tranStr[9] =  200; //经验值
	}
	else
	{
		controlData.OUT0 &= 0xef; //设置没有到达转速

		if(controlData.waitTimeToTFreq >= 1)
		{
			//稳定状态调速增量
			tranStr[6]  = ((controlData.TS_FreqInc& 0xff00) >> 8);
			tranStr[7]  = (controlData.TS_FreqInc & 0x00ff);
			tranStr[8] =  ((controlData.TS_FreqInc& 0xff00) >> 8);
			tranStr[9] =  (controlData.TS_FreqInc & 0x00ff);
		}
		else
		{
			tranStr[6]  =  ((allFreq.upFreqInc & 0xff00) >> 8);
			tranStr[7]  = (allFreq.upFreqInc & 0x00ff);
			tranStr[8] =  ((allFreq.downFreqInc & 0xff00) >> 8);
			tranStr[9] =  (allFreq.downFreqInc & 0x00ff);
		}
	} 
}

//发送数据的设置   变频板
void setSendData(void)   
{
	u8 i;
	vu8 j;
//	vu16 LCRC0 = 0;
	
	
	
	//根据转速状态设置增量
	if(HldReg[MOTOR_TYPE] == QK_VFD)
	{
		if(controlData.stop_mode == 2) controlData.TxStopMode = 1;  //减速直接直流制动停车
		else if(controlData.stop_mode == 3) controlData.TxStopMode = 1;//滑行到1HZ ,直流制动停车
		else if(controlData.stop_mode == 4) controlData.TxStopMode = 0;//滑行到1HZ ,自由停车
		else {}
		
		SetTransInc();
		
		tranStr[0] = SLAVE_NO;	//01站号
		tranStr[1] = 12;	 //发送的数据的长度
		tranStr[2] = controlData.OUT0;    //标志位
		tranStr[3]= 0x10 + controlData.TxStopMode;  //停车方式,0x10是一圈脉冲数
		tranStr[4] = ((controlData.PWM0h_Freq & 0xff00) >> 8); //
		tranStr[5]=  (controlData.PWM0h_Freq & 0x00ff);//发送的目标频率
		tranStr[10] = (MOTOR_BASE_FREQ & 0x00ff); //基频低8位
		tranStr[11] = RATE_CURRENT; //电流
		tranStr[12]   = (MOTOR_BASE_FREQ & 0xff00) >> 8;//低频高8位
		
		tranStr[13]   = 0x00; //保留位
	}	
	else if(HldReg[MOTOR_TYPE] == QK_WS)
	{
		tranStr[0] = SLAVE_NO;	//01站号
		tranStr[1] = 9;	 //发送的数据的长度
		tranStr[2] = controlData.OUT0;    //标志位
		tranStr[3]=  HldReg[ROTOR];                          //转子号
		tranStr[4] = ((controlData.PWM0h_Freq & 0xff00) >> 8); //
		tranStr[5]=  (controlData.PWM0h_Freq & 0x00ff);//发送的目标频率
		if(HldReg[SYS_TYPE] > LOW_TYPE)  tranStr[6] = 0xAF;   //高速
		else tranStr[6] = 0xA0;  
		tranStr[7] = HldReg[DEC];                   //降速档
		tranStr[8]   = (Speed_max[HldReg[ROTOR]] & 0xff00) >>8; //最高转速
		tranStr[9]   = Speed_max[HldReg[ROTOR]] &0x00ff;  
		tranStr[10]   = 28;
	}
	else //发送数据的设置  转接板接变频器
	{
		tranStr[0] = SLAVE_NO;	//01站号
		tranStr[1] = 10;	 //发送的数据的长度
		tranStr[2] = controlData.OUT0;    //标志位

		tranStr[3] = ((set2 & 0xff00) >> 8); //
		tranStr[4]=  (set2 & 0x00ff);//转速
		tranStr[5] = ((HldReg[ACC] ) &0x0f) |(((HldReg[DEC])<<4) & 0xf0 ); //升降速档位  
		
		tranStr[6]   = (Speed_max[1] & 0xff00) >>8; //最高转速 系统的最高转速
		tranStr[7]   = Speed_max[1] &0x00ff;  
		tranStr[8]   = HldReg[SET_TEMPERATURE]/10 +80;
		tranStr[9]   = HldReg[ROTOR];
		tranStr[10]   = (Speed_max[HldReg[ROTOR]] & 0xff00) >>8; //最高转速  设置转子的最高转速
		tranStr[11]   = Speed_max[HldReg[ROTOR]] &0x00ff; 
	}
		//求校验值
		j = 0;
		for(i = 0; i < tranStr[1] + 2; i++)
		{
			j +=tranStr[i];
		}
	
		tranStr[tranStr[1] + 2] = ~j + 1;
	
}






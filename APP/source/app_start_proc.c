#include "typedef.h"
//#include "app_start_proc.h"
//#include "app_temp.h"
//#include "app_comm.h"
//#include "PID.h"
//#include "app_alarm_record.h"
//#include "app_wnd_proc.h"
uint16_t CoilReg[COILREG_MAX_NUM];
u8 startKey_flag =0;
const vu16 GearTm_Tab[] = {300,240,200,180,150,120,90,60,50,45,40};

u16 set2;  //运行转速
//#define autosee_over  ((OldReceStr[1] & 0x08) >> 3 )
vu8 autosee_over;
vu8 DOOR_CLOSURE;
vu8 getStartKey =0,getStopKey =0;
RUN_MODE m_runState;//运行状态变量
FREQUENCE_PRARM allFreq;//运行过程中的所有频率参数
RUN_FLAG runFlag;//运行标志位
CONTROL_DATA controlData;  //控制数据
LXJ_RUNPARAM runParam ={0,0,0,0,0};     //实际运行参数（显示）
LXJ_ALLRUNTIME AllRunTime ={1,2,3,4,};
u8 ucStartBit;

u16 watch_CurSpeed;     //实际运行转速

#define M2TIME  15  //秒

volatile u16 M2_Time =0;
//-------------------------------------------
//运行控制定时器
#define NO_1ms 5
u8  enable_T1ms[NO_1ms];
u16  T1ms[NO_1ms];
u8 signal_T1ms[NO_1ms];
u16 counter_T1ms[NO_1ms];

u8 enable_RunTime;
u8 signal_RunTime;
u32 RunTime;
u32 counter_RunTime;
vu16 start_DelayOFFTime;
u16 stop_DelayOFFTime;
void runTimeContor(void)
{
	uchar i;
	static u16 T_1s;
	//------------------------------------------	
	for(i=0; i < NO_1ms; i ++)
	{
		if(enable_T1ms[i] == 1)
		{
			if(counter_T1ms[i] < T1ms[i]) counter_T1ms[i] ++;
			if(counter_T1ms[i] >= T1ms[i]) signal_T1ms[i] = 1;
			else {}
		}
		else 
		{
			counter_T1ms[i] = 0;
			signal_T1ms[i] = 0;
		}
	}
	if(T_1s>=1000)
	{
		T_1s =0;
		
		if(enable_RunTime == 1)
		{
			if(counter_RunTime < RunTime) counter_RunTime ++;
			if(counter_RunTime >= RunTime) signal_RunTime = 1;
			else {}
		}
		else 
		{
			counter_RunTime = 0;
			signal_RunTime = 0;
		}
		if(runFlag.arriveSpeedFlag  ==1) 
		{
			if(M2_Time < M2TIME) M2_Time++;
			else M2_Time = M2TIME;
		}
		else M2_Time =0;
	}
	else T_1s++;
	
	runParam.Freq = GetFKFreq();
	watch_CurSpeed = GetFKSpeed(HldReg[HALL_EN]);
	if(HldReg[MOTOR_TYPE] ==QK_VFD) MOTOR_control(set2,watch_CurSpeed);  //变频板控制
		
	
	if(start_DelayOFFTime >0) start_DelayOFFTime--;
	if(stop_DelayOFFTime >0) stop_DelayOFFTime--;
}
//----------------------------------------------
//修改警报标志
void SetAlarmFlag(ALARM_TYPE type)
{
	if(type == 0 || runParam.alarm == 0)
	{
		runParam.alarm =  type;
	}
}
//获取当前的警报标志
ALARM_TYPE GetAlarmFlag(void)
{
	return runParam.alarm;
}
//电机不转报警
void MotorNoTurnAlarm(void)
{
	//
	if(runFlag.curRunFlag == 1 && watch_CurSpeed < 30)
	{
		enable_T1ms[1] =1;
		T1ms[1] =10000;
	}
	else
	{
		enable_T1ms[1] =0;
	}
	if(signal_T1ms[1])
	{
		enable_T1ms[1] =0;
		SetAlarmFlag(MOTOR_NOTURN_ALARM);
	}
}
u8 IfSartKeyPress(void)
{
	return ( getStartKey);
}
//获取门盖状态
extern  u8 open_door;
void GetDoorLockAlarm(void)
{
	static u8 door_flag;
		if(startKey_flag && DOOR_CLOSURE && HldReg[LOCK_PROTECT] !=1) //门盖未关报警	    || getKeyState(OPEN_KEY) ==OPEN_KEY
		{
			SetAlarmFlag(GATELOCK_ALARM1); // E3
		}
		if(runFlag.curRunFlag && DOOR_CLOSURE && HldReg[LOCK_PROTECT] !=1)
		{
			SetAlarmFlag(GATELOCK_ALARM2); //E4
		}
		if(door_flag != DOOR_CLOSURE && BeepBusy()==0)    //提示门盖状态
		{
			Beep_SetTime(1, 500, 50);
		}
		door_flag = DOOR_CLOSURE;
		startKey_flag =0;
}

//超速报警
void GreaterSpeedAlarm(void)
{
	 if(runFlag.arriveSpeedFlag && 
			(watch_CurSpeed > set2 + 350))
	 {
		   //开始计时 
		 enable_T1ms[2] =1;
		 T1ms[2] =5;
	 }
	 else
	 {
		enable_T1ms[2] =0;
	 }
	 if(signal_T1ms[2])
	 {
		 //清除定时器
		enable_T1ms[2] =0;

		 //设置超速报警
		SetAlarmFlag(OVERSPEED_ALARM);
	 }
}
vu8 BPH_signal;

//获取通信信息
void GetAlarmInfo(void)
{
	static u8 X4_temp,commErrsignal;
	//通信故障报警

	if(comFlag.comm_failure == 1)
	{
			SetAlarmFlag(COMCN_ERROR_ALARM); //通信故障警报
	}
	else if(HldReg[SET_TIME_HH]==0 && HldReg[SET_TIME_MM]==0 && HldReg[SET_TIME_SS]==0 && startKey_flag)  
	{
		SetAlarmFlag(OPERATOR_ALARM);startKey_flag =0;
	}
	else
	{
		//commErrsignal = 0;
		if(VFD == HldReg[MOTOR_TYPE] ) //转接板报警
		{
			if(OldReceStr[6] ==0xFF )  SetAlarmFlag(VFD_COMM_ALARM);
			else if(OldReceStr[6] >0 )  SetAlarmFlag(VFD_ALARM);
		}
		else if(QK_WS == HldReg[MOTOR_TYPE] ) //高压无刷
		{
			if((OldReceStr[1] &0x10) ==0x10 )SetAlarmFlag(HALL_ALARM);
			else 	if((OldReceStr[7] &0x01) ==0x01 )SetAlarmFlag(OVERVOLTAGE_ALARM);
			else if((OldReceStr[7] &0x02) ==0x02 )SetAlarmFlag(OVERTEMP_ALARM);
			else if((OldReceStr[7] &0x04) ==0x04 )SetAlarmFlag(MOTOR_NOTURN_ALARM);   
			
			else if((OldReceStr[1] &0x01) ==0x01 )SetAlarmFlag(MODULE_ERROR_ALARM);
		}
		else    //变频板
		{
			switch(OldReceStr[6]) //警报信息
			{
				case 7: //模块故障E9
						SetAlarmFlag(MODULE_ERROR_ALARM);
				break;
				case 5: //欠压故障
						SetAlarmFlag(LACKVOLAGE_ALARM);
				break;
				case 4: //过压  
						SetAlarmFlag(OVERVOLTAGE_ALARM);
				break;
				case 3: //加速中过流
						SetAlarmFlag(OVERCURRENT_ALARMA);
				break;
				case 2: //运行中过流
						SetAlarmFlag(OVERCURRENT_ALARMB);
				break;
				case 1: //过载
				break;
			}
		}

		if(((BPH_signal ==1  && HldReg[BPH_SET] ==2)||(BPH_signal ==0  && HldReg[BPH_SET] ==3) ) && sys_startrun) //系统稳定运行1秒
		{
//			SetAlarmFlag(IMBALANCE_ALARM);
		}

		MotorNoTurnAlarm();	 	//电机不转报警
		GetDoorLockAlarm();  	//门盖未关报警
		GreaterSpeedAlarm();//超速报警
	}
}
//----------------------------------------------
//根据设定的档位时间算出增量(加速/减速)
//maxSpeed 最大转速
//time 增量时间 通过升降速档位的时间进行计算获得
u16 calcIncFreq(u16 maxSpeed, u16 time)
{
	u16 incFreq;
	u32 t_Speed = (u32)maxSpeed;
	t_Speed = (t_Speed << 9) / time;
	incFreq = t_Speed / 60 + 1;
	return incFreq;
}
//----------------------------------------------------------------
//计算运行过程中的所有频率参数
//计算运行过程中的所有频率参数
void calcAllRunFreq(u16 _setSpeed,u8 _Acc,u8 _Dec)
{ 
	u16 time,maxSpeed;
	u32 speedFreq;
	u32 t_speed;

	//计算停止频率
	maxSpeed = MAXROTORSPEED;

	t_speed =  _setSpeed;
	speedFreq = (t_speed * 5) / 3; 
	if((t_speed % 6) >= 3)	speedFreq++;
	allFreq.targetFreq = speedFreq;

	if(m_runState == RUN_UP && runParam.Freq > allFreq.targetFreq)//
	{
	    allFreq.targetFreq = runParam.Freq;
	}
	else{}

    //---------------------------------------------
	//计算最高频率  在最高转速的情况下 加1000转
	t_speed = 	maxSpeed +1000;
	speedFreq = (t_speed * 5) / 3; 
	allFreq.maxFreq = speedFreq;
	//---------------------------------------------
	//根据设定的加速档位时间算出加速增量
	time = GearTm_Tab[_Acc]-3;
	allFreq.upFreqInc = calcIncFreq(maxSpeed, time);

	//---------------------------------------------
	//根据设定的减速档位时间算出减速增量
	time  = GearTm_Tab[_Dec]-3;
	allFreq.downFreqInc = calcIncFreq(maxSpeed, time);

	//设置停车方式
	if(_Dec == 0) 
	{
		controlData.stop_mode = 1;
	}
	else
	{
		controlData.stop_mode = 2;
	}
}
//获取反馈频率
u16 GetFKFreq(void)
{
	u16 t_Freq;

	t_Freq = OldReceStr[4]; //实际输出频率
	t_Freq <<= 8;
	t_Freq += OldReceStr[5];

	return(t_Freq);

}



//获取反馈转速
//Hall_mode 1：测试模式 0：无霍尔测速模式
u16 GetFKSpeed(u8 Hall_mode)
{
//	static u32 usSpeedSum = 0;

	u16 t_Speed = 0;

	if(Hall_mode ==HIGHSET_DISABLE && HldReg[MOTOR_TYPE] == QK_VFD)  //变频板无霍尔模式
	{
		  t_Speed = runParam.Freq/5*3;
	}
	else
	{
		t_Speed = OldReceStr[2] << 8;
		t_Speed += OldReceStr[3];
	}
	return(t_Speed);


}
/**********************************************************************************************************
*	函 数 名: IO_Write
*	功能说明: 动作控制
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void 	IO_Write1(uint16_t outdata)//Y0-Y15
{
//	(outdata&(1<<0)) ? (Y0_ON) : (Y0_OFF);
//	(outdata&(1<<1)) ? (Y1_ON) : (Y1_OFF);
//	(outdata&(1<<2)) ? (Y2_ON) : (Y2_OFF);
//	(outdata&(1<<3)) ? (Y3_ON) : (Y3_OFF);	
//	(outdata&(1<<4)) ? (Y4_ON) : (Y4_OFF);
//	(outdata&(1<<5)) ? (Y5_ON) : (Y5_OFF);
//	(outdata&(1<<6)) ? (Y6_ON) : (Y6_OFF);
//	(outdata&(1<<7)) ? (Y7_ON) : (Y7_OFF);	
//	(outdata&(1<<8)) ? (Y8_ON) : (Y8_OFF);
//	(outdata&(1<<9)) ? (Y9_ON) : (Y9_OFF);
//	(outdata&(1<<10)) ? (Y10_ON) : (Y10_OFF);
//	(outdata&(1<<11)) ? (Y11_ON) : (Y11_OFF);	
//	(outdata&(1<<12)) ? (Y12_ON) : (Y12_OFF);
//	(outdata&(1<<13)) ? (Y13_ON) : (Y13_OFF);
//	(outdata&(1<<14)) ? (Y14_ON) : (Y14_OFF);
//	(outdata&(1<<15)) ? (Y15_ON) : (Y15_OFF);	
}

//-----------------------------------------------------------------
//转子识别处理过程

//----------------------------------
//转子识别

u8 get_rotor_see(u16 angle)
{
	u8 i;
	u16 rotor_angle[]= {0,90,120,150,180,210,240,270,300,330,30,60}; //转子角度  30度一个角度
	for(i = 1; i<= ROTOR_MAX; i++)
	{
		if(angle >= (rotor_angle[i]-10) && angle < (rotor_angle[i]+10) )
			return(i);
	}
	return(0);
}

//----------------------------------------------
//电机运行控制
//_set2设定转速
void MOTOR_control(u16 _setSpeed,u16 _watchSpeed)
{
	static u8 AutoRotor;   //识别出来的转子号
	static vu16 watchp1 = 0; //前一次的转速
	static vu16 watchp2 = 0;//前第两次的转速
	static vu16 err = 0;
	static vu16 err1 = 0; //差值
//--------------------------------------------------------------
// 占空比
// T24为计算周期，时间到计算一次
// 低速时300ms一加，接近转速时1s一加
//--------------------------------------------------------------				
		if(runFlag.transRunFlag == 1) //电机处于非停机状态
		{
			enable_T1ms[0] = 1;
      if ( runFlag.arriveTargetFlag == 0)  T1ms[0] = 80;	
			else T1ms[0] = 500;	

			if(signal_T1ms[0] == 1)
			{ 
				signal_T1ms[0] = 0;
				counter_T1ms[0] = 0;	
				calcAllRunFreq(_setSpeed,HldReg[ACC],HldReg[DEC]);
				if(runFlag.curRunFlag)  //升速，降速，运行，不包括直流制动和减速停机 处于运行状态
				{
					switch(m_runState)
					{
						case ZZSB_MODE: //转子自动识别
							if(allFreq.targetFreq < 500 ) controlData.PWM0h_Freq = allFreq.targetFreq;  //防止设定转速太低
							else controlData.PWM0h_Freq = 500;//处于转子识别截止频?
							if( autosee_over == 0  && runParam.Freq == controlData.PWM0h_Freq) //转速大于80，开始转子自动识别
							{
								controlData.OUT0|= 0x40;  //使能转子自动识别
							}
							else{}
							if(autosee_over == 1) //转子识别完成
							{
								controlData.OUT0 &= 0xbf;  //使能转子自动识别
								runParam.zzsb_angle = OldReceStr[8] * 256 + OldReceStr[9];   //换算角度
						
								AutoRotor = get_rotor_see(runParam.zzsb_angle);
							
								if(AutoRotor ==0)
								{
									//SetAlarmFlag(ROTOR_ERROR_ALARM); //转子识别错误
									m_runState = RUN_DOWN;
								}
								else if(AutoRotor ==HldReg[ROTOR])
								{
									m_runState = RUN_UP;//开始按档位升速
								}
								
								else if(HldReg[ROTOR] !=AutoRotor && AutoRotor >0) //转子识别错误
								{

									if(_setSpeed > Speed_max[AutoRotor])
									{
										HldReg[SET_SPEED] = Speed_max[AutoRotor];
										HldReg[SET_RCF]   = Force_max[AutoRotor];
									}

									HldReg[ROTOR] =AutoRotor;
									HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],HldReg[ROTOR]);
									
									 //SetAlarmFlag(ROTOR_ERROR_ALARM); //转子识别错误
								}
							}
							break;
						case RUN_UP:  //加速状态
							if(controlData.waitTimeToTFreq == 0)
							{
								//if(pwm0h < stopfrequence) 
								controlData.PWM0h_Freq = allFreq.targetFreq;//处于提前加速状态
								if(runParam.Freq >= allFreq.targetFreq )
								{
									if( HldReg[HALL_EN] ==HIGHSET_DISABLE) m_runState = RUN;
									else controlData.waitTimeToTFreq = 1;
								}
								else{}
							}
							else    //调速状态
							{
								controlData.TS_FreqInc = 600;
								if(controlData.waitTimeToTFreq > 120) 
								{
									runFlag.arriveTargetFlag = 1;
									controlData.waitTimeToTFreq = 0;
									m_runState = RUN;
								}
								else controlData.waitTimeToTFreq ++;
								err = _watchSpeed -watchp1;
								err1 = watchp1 -watchp2;
								if (_watchSpeed + 4 * (err + err1) > (_setSpeed + 4)) 
								{
									controlData.PWM0h_Freq -= 2;		
								}
								else if (_watchSpeed + 4*(err + err1) < (_setSpeed - 4)) controlData.PWM0h_Freq += 4;
								else if (_watchSpeed + 4*(err + err1) < (_setSpeed - 16)) controlData.PWM0h_Freq += 8;	
								else {}
								if(controlData.PWM0h_Freq < allFreq.targetFreq) controlData.PWM0h_Freq = allFreq.targetFreq; //给定频率不能小于终止频率
								else{}	
								watchp2 = watchp1;
								watchp1 = _watchSpeed;
							}
							break;
						case  RUN_DOWN: //减速状态
							controlData.PWM0h_Freq = allFreq.targetFreq+50;//处于提前加速状态
							if(_watchSpeed <  (_setSpeed + 50))
							{
								runFlag.arriveTargetFlag = 1;
								m_runState = RUN;
							}
							else{}

							break;
						case RUN:  //为运行状态
							if(_setSpeed > _watchSpeed + 10) controlData.PWM0h_Freq += 10;
							else if(_setSpeed > _watchSpeed + 5) controlData.PWM0h_Freq += 4;
							else if(_watchSpeed > _setSpeed + 5) controlData.PWM0h_Freq -= 6;
							else if(_watchSpeed > _setSpeed + 10) controlData.PWM0h_Freq -= 10;
							else{}	
							if(controlData.PWM0h_Freq < allFreq.targetFreq)controlData.PWM0h_Freq = allFreq.targetFreq; //给定频率不能小于终止频率
							else{}
							break;
						default :break;
					}
				}
				else// 减速停机运行
				{
					if(controlData.stop_mode == 1)   //自由停车
					{
					}
					else 
					{
						controlData.PWM0h_Freq = 100;
						if(runParam.Freq > 500)
						{
						}
						else if(runParam.Freq > 100)
						{
							if(controlData.stop_mode == 2){}
							else
							{
								m_runState = RUN_LOW;  //滑行
								if(_watchSpeed < 30) 
								{
									STOP_ON;  //停止信号
								}
								else{}
							}
						}
						else 
						{
							m_runState = DC_STOP;  //直流制动
							STOP_ON;  //停止信号
						}
					}
				}
				if(controlData.PWM0h_Freq > allFreq.maxFreq) controlData.PWM0h_Freq = allFreq.maxFreq; //频率限定
				else{}	
			}
			else{}
			
		}	
		else 
		{
			runFlag.arriveTargetFlag = 0;
			controlData.waitTimeToTFreq = 0;
			enable_T1ms[0] = 0;
			controlData.PWM0h_Freq = 200;
			STOP_OFF;
		}
}

//-----------------------------------------------------------------
//函数功能：PWM控制,占空比计算
//T1ms[4]为计算周期，时间到计算一次。低速时按升速档位计算出更改PWM的周期，接近转速时1S更新一次
//-------------------------------------
void PWM_contrl(u16 _set2,u8 _set7)
{
	vu16 temp; 

	
	static u16 watchp1;
	static u16 watchp2;
	static signed int err;
	static signed int err1;
	
	
	
	if (runFlag.curRunFlag == 1)
	{
		T1ms[0] =600;
		enable_T1ms[0] = 1;
		if(M2_Time == M2TIME) T1ms[0]=400; //150  //转速稳定后调快一点
		else if(!( autosee_over == 1 || HldReg[AUTO_ROTOR]==HIGHSET_DISABLE))  T1ms[0]=800;	//转子识别阶段
		else  if(HldReg[SYS_TYPE] <= LOW_TYPE || HldReg[SYS_TYPE] == LY_TDL5M) //低速  通用情况 
		{
			if ( (_set2 > watch_CurSpeed + 600) || (watch_CurSpeed > _set2 + 600) )
			{
				T1ms[0]=_set7* 35 + 100;	
			}
			else
			{
				T1ms[0]=600;	
			}
		}
		else //if(HldReg[SYS_TYPE] > LOW_TYPE)  //高速  通用情况 
		{
			if(Speed_max[HldReg[ROTOR]] > 13000)    //高速转子
			{				
				if ( (_set2 > watch_CurSpeed + 4000) || (watch_CurSpeed > _set2 + 4000) )
				{
					T1ms[0]=(_set7)* 30 + 90;	
				}
				else if ( (_set2 > watch_CurSpeed + 3000) || (watch_CurSpeed > _set2 + 3000) )
				{
					T1ms[0]=(_set7)* 40 + 150;	
				}
				else if ( (_set2 > watch_CurSpeed + 2000) || (watch_CurSpeed > _set2 + 2000) )
				{
					T1ms[0]=(_set7)* 50 + 220;	
				}
				else if ( (_set2 > watch_CurSpeed + 1000) || (watch_CurSpeed > _set2 + 1000) )
				{
					T1ms[0]=(_set7)* 80 + 300;	
				}
				else
				{
					T1ms[0]=800;	
				}
			}
			else
			{
				if ( (_set2 > watch_CurSpeed + 4000) || (watch_CurSpeed > _set2 + 4000) )
				{
					T1ms[0]=(_set7)* 50 + 120;	
				}
				else if ( (_set2 > watch_CurSpeed + 3000) || (watch_CurSpeed > _set2 + 3000) )
				{
					T1ms[0]=(_set7)* 60 + 150;	
				}
				else if ( (_set2 > watch_CurSpeed + 2000) || (watch_CurSpeed > _set2 + 2000) )
				{
					T1ms[0]=(_set7)* 70 + 220;	
				}
				else if ( (_set2 > watch_CurSpeed + 1000) || (watch_CurSpeed > _set2 + 1000) )
				{
					T1ms[0]=(_set7)* 80 + 300;	
				}
				else
				{
					T1ms[0]=800;	
				}
			}
		}
//		else   //低速  通用情况 
//		{
//			if ( (_set2 > watch_CurSpeed + 600) || (watch_CurSpeed > _set2 + 600) )
//			{
//				T1ms[0]=_set7* 25 + 100;	
//			}
//			else
//			{
//				T1ms[0]=600;	
//			}
//		}
		if(signal_T1ms[0] == 1)
		{
			signal_T1ms[0] = 0;
			counter_T1ms[0] = 0;

			if (watch_CurSpeed < 40) 
			{
				controlData.PWM0h_Freq ++;
				if (controlData.PWM0h_Freq < 8) controlData.PWM0h_Freq = 8;
				else if(controlData.PWM0h_Freq > 40) controlData.PWM0h_Freq = 40;
				else {}
			}
			else
			{
				err = watch_CurSpeed -watchp1;
				err1 = watchp1 -watchp2;
				if (watch_CurSpeed + 3 * (err + err1) > (_set2 + 50)) 
				{
					if (runFlag.arriveSpeedFlag == 1 && watch_CurSpeed > _set2 && (watch_CurSpeed + err + err1) > (_set2 + 120)) controlData.PWM0h_Freq -= 2;
					else controlData.PWM0h_Freq -= 1;		
				}
				else if (watch_CurSpeed + 2*(err + err1) < (_set2 - 50)) controlData.PWM0h_Freq += 1;	
				else {}
			}
			watchp2 = watchp1;
			watchp1 =watch_CurSpeed;
		}
		else {}	
		if (controlData.PWM0h_Freq > 490) controlData.PWM0h_Freq = 490;
		else if (controlData.PWM0h_Freq < 8) controlData.PWM0h_Freq = 8;
		else {}
	}
	else 
	{
		controlData.PWM0h_Freq = 8;enable_T1ms[0] = 0;
	}
}

//获取到达目标转速的信号
void GetTargetSpeedFlag(void)
{
	if(runFlag.curRunFlag)  	  //transRunFlag
	{
		if((watch_CurSpeed <set2 + 50)&&
		(watch_CurSpeed > set2 -20) && runFlag.curRunFlag)
		{
			runFlag.arriveSpeedFlag = 1;
			SetRunState(RUN);
		}
	}
	else
	{
		if(watch_CurSpeed < 200)
		{
			runFlag.arriveSpeedFlag  = 0;
		}
	}
	
	//显示转速
/*	if(runFlag.curRunFlag && KEY6 ==1 && KEY11 ==1) runParam.Speed = OldReceStr[2] * 256 + OldReceStr[3]; 
	else */if(runFlag.arriveSpeedFlag  && ((watch_CurSpeed < set2 + 250)&&
		(watch_CurSpeed >set2 -100))) 
	{
		if(runFlag.curRunFlag ==0 && watch_CurSpeed < set2) runParam.Speed = watch_CurSpeed; //停机后小于设置转速显示实际转速
		else runParam.Speed = set2;
	}
	else runParam.Speed = watch_CurSpeed;
	
	if(runParam.Speed == set2)   runParam.RCF = HldReg[SET_RCF];
	else if(runParam.Speed == 0)   runParam.RCF = 0;
	else runParam.RCF = Speed_to_Force(runParam.Speed,HldReg[ROTOR]);
}
//获取运行时间计数开始信号
void GetCntSignal(void)
{
	if(runFlag.shut_run ==1) RunTime =10000; //点动 给定一个较大的时间
	else if(keySET_State ==0) 
	{
		if(HldReg[SET_TIME_HH]==0 && HldReg[SET_TIME_MM]==0 && HldReg[SET_TIME_SS]==0)   //设置时间为0
		{
			RunTime = 600000; 
		}
		else RunTime = (u32)HldReg[SET_TIME_HH] *3600+HldReg[SET_TIME_MM] *60+ HldReg[SET_TIME_SS];   //根据选用的时间 非设置状态下更新时间
	}
	if(ARRVIE_SPEED_FLAG)
	{
	  enable_RunTime =1;
	}
}
////初始化参数
void InitTotalTime(void)
{
	AllRunTime.uiAllRunTime_HH = 0;
	AllRunTime.uiAllRunTime_MM = 0;
	AllRunTime.uiAllRunTime_SS = 0;
}
//
//累计时间  秒计时
void CalcTimeSum(void)
{
	//总的累计时间
	if(AllRunTime.uiAllRunTime_SS < 59) AllRunTime.uiAllRunTime_SS++;
	else 
	{
		AllRunTime.uiAllRunTime_SS =0;
		if(AllRunTime.uiAllRunTime_MM <59) AllRunTime.uiAllRunTime_MM++;
		else
		{
			AllRunTime.uiAllRunTime_MM =0;
			AllRunTime.uiAllRunTime_HH++;
		}
	}
	
	//转子对应的累计时间
	if(AllRunTime.uiAllRotorRunTime_SS[HldReg[ROTOR]] < 59) AllRunTime.uiAllRotorRunTime_SS[HldReg[ROTOR]]++;
	else 
	{
		AllRunTime.uiAllRotorRunTime_SS[HldReg[ROTOR]] =0;
		if(AllRunTime.uiAllRotorRunTime_MM[HldReg[ROTOR]] <59) AllRunTime.uiAllRotorRunTime_MM[HldReg[ROTOR]]++;
		else
		{
			AllRunTime.uiAllRotorRunTime_MM[HldReg[ROTOR]] =0;
			AllRunTime.uiAllRotorRunTime_HH[HldReg[ROTOR]]++;
		}
	}
		
}
//清除运行时间计时
void ClearRunTimer(void)
{
	counter_RunTime =0;
}
//运行时间管理
void RunTimeManage(void)
{
	static vu32 t_time = 0;
	static vu16 vusOldTm = 0xffff;
	u8 ucLED[3];
	static  u32 run_time_last;
	static  u32 RunTime_old;
	
	HldReg[SET_TIME_HH] =0;    //没使用小时   
	
	//获取运行时间开始计时信号	
	GetCntSignal();

	//清除运行时间计数器
	if(run_time_last != RunTime)
	{
		ClearRunTimer();
		run_time_last = RunTime;
	}
	if(runFlag.curRunFlag)
	{
		//运行时间的显示
		
			if(1==runFlag.shut_run) t_time =counter_RunTime;  //点动  顺计时
			else
			{
//				if(HldReg[SET_TIME_HH]==0 && HldReg[SET_TIME_MM]==0 && HldReg[SET_TIME_SS]==0)   //设置时间为0
//				{
//					t_time = counter_RunTime;  //设置时间为0时 顺计时
//				}
//				else t_time = RunTime - counter_RunTime;
				t_time = RunTime - counter_RunTime;
			}
		if(ARRVIE_SPEED_FLAG)
		{
//			runParam.time_HH = t_time/3600;
//			runParam.time_MM = t_time%3600/60;
			runParam.time_MM = t_time/60;  //
			runParam.time_SS = t_time%60;
		}
		else
		{
			if(1==runFlag.shut_run)
			{
				runParam.time_HH = 0;
				runParam.time_MM = 0;
				runParam.time_SS = 0;
			}
			else 
			{
				runParam.time_HH = HldReg[SET_TIME_HH];
				runParam.time_MM = HldReg[SET_TIME_MM];
				runParam.time_SS = HldReg[SET_TIME_SS];
			}
		}
	}
	else
	{
		enable_RunTime =0;
	}

	if(counter_RunTime!= RunTime_old && counter_RunTime >0) CalcTimeSum();   //累计运行时间   启动运行计时后开始计时
	RunTime_old = counter_RunTime;
}
//设置当前的运行状态
void SetRunState(RUN_MODE state)
{
	if(state >= ALL_MODE) return ;
	m_runState = state;
}
//获取电机状态
void SetTxMotorState(void)
{
	if(GetAlarmFlag() != MOTOR_NOTURN_ALARM)
	{
		MOTOR_DEAD_OFF;
	}
	else
	{
			//电机不转
		MOTOR_DEAD_ON;
	}
}
//启动键的过程处理
void STARTKey_Proc(void)
{
	//判断是否符合开启运行状态
	//win_state = RUN_STATE; //处于设置状态
	//设置启动标志
	//不能处于设置状态，没有警报
	if( getStartKey ==1 )//|| AJ_START_KEY
	{
			if(( GetAlarmFlag() == NO_ALARM )&&watch_CurSpeed < 40 && keySET_State ==0&&  ((!DOOR_CLOSURE ) || HldReg[LOCK_PROTECT] ==1)
			&& !(HldReg[SET_TIME_HH]==0 && HldReg[SET_TIME_MM]==0 && HldReg[SET_TIME_SS]==0))
			
		{
			START_ON;
			STOP_OFF;
			
			ucStartBit =1;
			start_DelayOFFTime =1500;
			
		}
		startKey_flag =1;
		getStartKey =0;
	}
	//if(KEY13) shut_run =1;   //点动标志
//	else if(start_DelayOFFTime ==0) {START_OFF;}

}
void GetStopSignal(void)
{
	//停止键按下，有警报，时间到，
	vu8 am;
	vu8 flag ;
	vu8 stopFlag;
	static vu8 StopBpSignal = 0;
	
	am = GetAlarmFlag();
	if(am != LACKVOLAGE_ALARM) //欠压报警不停机
	{
		flag = (am > 0) && (runFlag.transRunFlag ||runFlag.curRunFlag );
	}
	//stopFlag = KEY16;//IfStopPress();
	if((getStopKey ==1 ) &&am>0 ) {getStopKey =0;SetAlarmFlag(NO_ALARM);}//|| AJ_STOP_KEY|| AJ_STOP_KEY

	if(flag || (getStopKey) || signal_RunTime  )  //shut_run_stop 点动停止  X0急停
  {
		if( (controlData.OUT0  | 0x20) ==0) getStopKey =0;	
		runFlag.shut_run =0;
		if(signal_RunTime)    //时间到停机 强制将时间写0  避免时间没有更新到0的情况
		{
			runParam.time_MM = 0;
			runParam.time_SS = 0;
		}
		if(runFlag.curRunFlag)
		{
			runFlag.curRunFlag = 0; //显示板运行标志置0
			SetRunState(RUN_DOWN); //设置处于停止模式

			runFlag.arriveTargetFlag = 0; //到达目标频率标志为0
			runFlag.arriveSpeedFlag  = 0; //到达目标转速为0
			runFlag.shut_run_stop =0;
			runFlag.shut_run =0;
			if(controlData.stop_mode == 1) //自由停车
		    {
				controlData.TxStopMode = 2; //自由停车
				STOP_ON;
			}

		}
	   
		controlData.waitTimeToTFreq = 0; //等待到达调速时间
		runFlag.realSpeedFlag = 0;
		//关定时器
		enable_RunTime =0;
	}
}
//转接板停止信号
void GetStopSignal_VFD(void)
{
	if ((getStopKey)|| X0 || signal_RunTime == 1 || (GetAlarmFlag() !=NO_ALARM && runFlag.curRunFlag == 1)
		    )
	{
		STOP_ON;
		if((getStopKey)) SetAlarmFlag(NO_ALARM);  //清除报警|| AJ_STOP_KEY|| AJ_STOP_KEY
		if( (controlData.OUT0  | 0x20) ==0) getStopKey =0;
		controlData.PWM0h_Freq = 8;
	  runFlag.shut_run =0;
		if(signal_RunTime)    //时间到停机 强制将时间写0  避免时间没有更新到0的情况
		{
			runParam.time_MM = 0;
			runParam.time_SS = 0;
		}
		
		//if(runFlag.curRunFlag == 1)  Beep_SetTime(3, 500, 500);	
		if(runFlag.curRunFlag == 1)  SetRunState(RUN_DOWN); //设置停止状态
		runFlag.curRunFlag =0;
		stop_DelayOFFTime =1500;
	}
	else if(stop_DelayOFFTime ==0) STOP_OFF;
}
//设置运行状态
void SetCurRunState(void)
{
   

	//变频器不处于停止状态
	if(OldReceStr[0] > 0) 
	{
		runFlag.transRunFlag = 1;  //变频器运行标志位置1
		runFlag.motorRunFlag = 1; //电机标志位置1
				
		if(ucStartBit == 1)
		{
			//启动
			ucStartBit = 0;
			runFlag.curRunFlag = 1; //显示板的运行状态
			
				if(HldReg[AUTO_ROTOR]==HIGHSET_ENABLE) SetRunState(ZZSB_MODE);
				else SetRunState(RUN_UP); //设置升速状态  按住确认键再启动 不启用转子识别功能
				
		}
		START_OFF;
	}
	else
	{
		runFlag.transRunFlag = 0; //变频器停机
		
	//	ucStartBit = 0;
	//	STOP_OFF;
	}
	
	//条件还要加一个当前的转速为0
	if((runFlag.motorRunFlag != 0) && runFlag.transRunFlag == 0 && watch_CurSpeed < 3) 
	{
		runFlag.motorRunFlag = 0; //电机运行状态为0
		SetRunState(STOP); //设置停止状态
		if(HldReg[AUTOOPEN_EN] ==2)   OnDrSgn=1;    //停机自动开门
		Beep_SetTime(3, 500, 500);	
		STOP_OFF;
		write_eepromAllRunTime();getStopKey =0;
	}

	if(runFlag.curRunFlag)
	{
		SetLedState(STOP_LED_TYPE, 0);
			SetLedState(START_LED_TYPE, 1);
	}
	else 
	{
		SetLedState(STOP_LED_TYPE, 1);
		SetLedState(START_LED_TYPE, 0);
	}
}
//-------------------------------------------
//无刷驱动转子识别
void autoRotorSee(void)
{
	//-------------------------------------------
//计算转子号
//-------------------------------------------
//		if(rotor_autosee == 1) 
//			rotor = rotor_check((revdata[5] <<8) |revdata[6]);
//		//else rotor =0;
//		if(autosee_over ==1)
//		{
//			if(set1 ==0 && rotor >0)  //智能识别转子
//			{

//				if(set2 > set2_temp1[rotor])
//					set2 = set2_temp1[rotor];
//				set2_temp =set2;
//				set1 =rotor;
//				set1_temp =rotor;
//				set4_temp =rpm_xg(set2_temp);
//				set4 = set4_temp;
//				
//			}
//			else if(set1 !=rotor && rotor >0) //转子识别错误
//			{

//				if(set2 > set2_temp1[rotor])
//					set2 = set2_temp1[rotor];
//				set2_temp =set2;
//				set1 =rotor;
//				set1_temp =set1;
//				set4_temp =rpm_xg(set2_temp);
//				set4 = set4_temp;
//				
//				rotor_see_err =1; //转转识别错误标志
//			}
//			
//		}
}

/**********************************************************************************************************
*	函 数 名: Run_Proc
*	功能说明: 运行过程控制
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/ 
void Run_Proc(void)
{
	if(keySET_State ==0)  set2 = HldReg[SET_SPEED];   //运行控制转速  在修改过程中不更新
	
	if(HldReg[MOTOR_TYPE] == QK_VFD)
	{
		autosee_over = ((OldReceStr[1] & 0x08) >> 3 );
		if(HldReg[LOCK_PROTECT] ==3) DOOR_CLOSURE =( (~OldReceStr[1] & 0x20) >>5 );
		else DOOR_CLOSURE =( (OldReceStr[1] & 0x20) >>5 );
		BPH_signal = ( ((~OldReceStr[1]) & 0x04) >> 2) ;	 //不平衡报警状态位
	}
	else 
	{
		autosee_over = (OldReceStr[1] & 0x80) >>7;
		
		if(HldReg[LOCK_PROTECT] ==3) DOOR_CLOSURE =( (~OldReceStr[1] & 0x02) >>1 );
		else DOOR_CLOSURE =( (OldReceStr[1] & 0x02) >>1 );
		
		BPH_signal = ( ((OldReceStr[1]) & 0x04) >> 2) ;	 //不平衡报警状态位
	}
	SetCurRunState();
	GetTargetSpeedFlag();
	RunTimeManage();
	if(HldReg[MOTOR_TYPE] == QK_VFD)  GetStopSignal();
	else GetStopSignal_VFD();
	if(HldReg[MOTOR_TYPE] == QK_WS) PWM_contrl(set2,HldReg[ACC] );   //无刷控制调速
	STARTKey_Proc();
	GetAlarmInfo();
}


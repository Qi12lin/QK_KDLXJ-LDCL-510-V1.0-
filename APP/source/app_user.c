#include "typedef.h"


//转速
u16 Speed_max[20] =
{
	0,
	16000,  //1号转子极限转速
	16000,  //2号
	14000,  //3号
	14000,  //4号
	12000,  //5号
	6000    //6号
};

//离心力
u16 Force_max[20]=
{
	0,
	14882, //1号转子极限离心力
	18603, //2号
	14243, //3号
	14681, //4号
	14006, //5号
	3200   //6号
};

//转子最大数目
u8 RotorMaxNum = 6;

//	vu8  ucAcc_R[MAX_ROTOR];
// 	vu8  ucDec_R[MAX_ROTOR];
ROTORPROGRAMDATA RotorPrgData;

PROGRAMDATA  OneProgramData;   //一组程序数据
PROGRAMDATA  progData[10];    //一次读取10组数据


//获取最大转速
u16 GetMaxSpeed(u8 rotor)
{
	if(rotor > ROTOR_MAX) return 0;
	return Speed_max[rotor];
}

//----------------------------------------------------------
// 获取转速及离心力最大值 fSpeed 0 转速| 1 离心力
//----------------------------------------------------------
u16 Fc_SpeedMax (u8 rotorNo, u8 fSpeed)
{
	if(!fSpeed) //转速
	{
		return (Speed_max[rotorNo]);
	}
	else //离心力
	{
		return (Force_max[rotorNo]);
	}
}

//--------------------------------------------------------------
// 设置转子号时自动显示最大值
//--------------------------------------------------------------
void SetRotor (void)
{
//	lxj_temp.LXForce = Force_max[lxj_temp.RotorNo];
//	lxj_temp.RotorSpeed = Speed_max[lxj_temp.RotorNo];
}

//--------------------------------------------------------------
// 牛顿迭代法求平方根
// 入口参数：被开方数data，猜测开放结果，guessResult
//--------------------------------------------------------------
u16 Newton_sqrt(u32 data,u16 guessResult)
{
	u8 x;
	u16 result = guessResult;
	for(x = 10 ; x > 0 ; x--)
	{
		result = (result + data / result)>> 1;
	}
	return result;
}

//离心力转换为速度
u16 Force_to_Speed(u16 force,u8 rotorNo)
{
	u32 tempx;
	if(force == Force_max[rotorNo]) tempx =Speed_max[rotorNo];
	else 
	{
		tempx = force;
		tempx *= Speed_max[rotorNo];
		tempx /= Force_max[rotorNo];
		tempx *= Speed_max[rotorNo];
		tempx = Newton_sqrt(tempx,1000); //开方函数
	}
	return(tempx);
}

//速度转换为离心力
u16 Speed_to_Force(u16 speed, u8 rotorNo)
{
	u32 tempx;
	if(speed == Speed_max[rotorNo]) tempx =Force_max[rotorNo];
	else
	{
		tempx = Force_max[rotorNo];
		tempx *= speed;
		tempx /= Speed_max[rotorNo];
		tempx *= speed;
		tempx /= Speed_max[rotorNo];
	}	
	return((u16)tempx);
	
}
/**********************************************************************************************************
*	函 数 名: Maxmin_Param
*	功能说明: 参数最大最小值限定
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/ 
//_type :0 初始化  1：修改参数


void Maxmin_Param(u8 _type)
{
	uint8_t i;         //程序 转子号 转速 离心力 时间H 时间M  时间S 温度 升速 降速             基频 电流 锁机时间 型号   门锁 识别 不平衡 门保 语言 霍尔 电机 电压 开门时间 自动开门 密码H 密码L 温度系数  开机页面
	uint16_t HldRegMin[40]={1,    1, 200,  10,   0,    0,    0,  -20,  1,  1, 0,1,1,1,1,0,  50,   8,    0,     1,     1,  1,   1,    1,  1,    1,  1,  1,   100,     1       ,0     ,0   ,0,1};
	uint16_t HldRegMax[40]={100,  0,   0,   0,  99,   99,   59,   40,  10, 10,0,90,90,60000,60000,0, 400,  35,65535,VER_MAX_NUM,5,2,   2,    3,  2,    3,   3,  2,   999,    2       ,65535,65535,9,2};
	uint16_t HldRegInit[40]={1,   2,   0,   0,   0,   10,    0,    4,   4,  4,0,10,40,1,90,0,  90,  22,     0,   1,INIT_LOCK_TYPE,INIT_AUTO_ROTOR,INIT_BPH_SET,INTI_LOCK_PROTECT,INIT_LANGUAGE,INIT_HALL_EN,INTI_MOTOR_TYPE,INIT_VOLTAGE,INIT_DOOROPEN_TIME,INIT_AUTO_ROTOR,0,0,1,
													INIT_STARTPAGE};
		//                                                               加热温度 制冷温度 加热时间 制冷时间           	                                                   
	static u16 set_speed_last;
  static s16 set_temprature_last;		
		
	//根据型号读取转子信息	
	if(HldReg[SYS_TYPE] <1 || HldReg[SYS_TYPE] >= HldRegMax[SYS_TYPE])  HldReg[SYS_TYPE]  = HldRegInit[SYS_TYPE];
		get_rotor_info(HldReg[SYS_TYPE],Speed_max,Force_max,&RotorMaxNum);
	HldRegMax[ROTOR]	=RotorMaxNum;  //最大转子号
		
	if(HldReg[ROTOR] <1 || HldReg[ROTOR] > HldRegMax[ROTOR])  HldReg[ROTOR]  = HldRegInit[ROTOR];	
	HldRegMax[2]	=Speed_max[HldReg[ROTOR]];  //最大转速
	HldRegMax[3]	=Force_max[HldReg[ROTOR]];  //最大离心力
	HldRegInit[2]	=Speed_max[HldReg[ROTOR]];  //最大转速
	HldRegInit[3]	=Force_max[HldReg[ROTOR]];  //最大离心力	
		
	if(INIT_SYS_TYPE >  LOW_TYPE)  //默认为高速电机的基频电流初始值
	{
		HldRegInit[BASE_F]  = INIT_BASE_F_HIGH;
		HldRegInit[RATED_CURRENT]  = INTT_RATED_CURRENT_HIGH;
	}
	else 
	{
		HldRegInit[BASE_F]  = INIT_BASE_F_LOW;
		HldRegInit[RATED_CURRENT]  = INIT_RATED_CURRENT_LOW;
	}
	
  switch(_type)
	{
		
		case 1:
			
			for(i = 0; i < (MAX_SET_NUMBER); i++)
			{
				if(i == SET_TEMPERATURE)   //温度 数据类型不一样
				{
					if((s16)HldReg[SET_TEMPERATURE] > 40) HldReg[SET_TEMPERATURE] =4;
					else if((s16)HldReg[SET_TEMPERATURE] < -20) HldReg[SET_TEMPERATURE] =4;
					else{}
				}
				else
				{
					if(HldReg[i] > HldRegMax[i] ) HldReg[i] = HldRegMax[i];
					else if(HldReg[i] < HldRegMin[i]) HldReg[i] = HldRegMin[i];
					else{}		
				}
			}
			break;
		default :
			
			for(i = 0; i < (MAX_SET_NUMBER); i++)
			{
				if(i == SET_TEMPERATURE)   //温度 数据类型不一样
				{
					if((s16)HldReg[SET_TEMPERATURE] > 40) HldReg[SET_TEMPERATURE] =40;
					else if((s16)HldReg[SET_TEMPERATURE] < -20) HldReg[SET_TEMPERATURE] =(s16) -20;
					else{}
				}
				else
				{
					if(HldReg[i] > HldRegMax[i]  || (HldReg[i] < HldRegMin[i])) HldReg[i] = HldRegInit[i];
					else{}		
				}
			}
			break;	
		}
		if(runFlag.curRunFlag  && set_speed_last != HldReg[SET_SPEED])
		{
			if(runParam.Speed < HldReg[SET_SPEED])  
			{
				SetRunState(RUN_UP);
			}
			else SetRunState(RUN_DOWN);
			runFlag.arriveTargetFlag = 0;
			controlData.waitTimeToTFreq = 0;
			runFlag.arriveSpeedFlag =0;
		}
		set_speed_last  = HldReg[SET_SPEED];
	calcAllRunFreq(HldReg[SET_SPEED],HldReg[ACC],HldReg[DEC]);
		if(g_ucfDispXg)  HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//显示离心力
		else HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
		
		
		if(set_temprature_last != HldReg[SET_TEMPERATURE])
		{
			TemprInit();
		}
		set_temprature_last = HldReg[SET_TEMPERATURE];
//	for(i = 1; i < 40; i++)
//	{
//		if(HldReg[i] > HldRegMax[i]) HldReg[i] = HldRegMax[i];
//		else if(HldReg[i] < HldRegMin[i]) HldReg[i] = HldRegMin[i];
//		else{}		
//	}
		CommSetFlag =0;	
		setData_flag =50;
		
		HldReg[SET1] =1;
		HldReg[SET0] =0;
}
//EEPROM数据保存地址分配
#define FRIST_SAVE_RUN_DATA_ADD  0   //运行数据保存地址  0~99用于运行数据保存

 

#define FRIST_ROTORPROGDATA_ADD 100       //每个转子对应的设置升降速档位 转速 温度 时间小时\分\秒 预留30个转子

#define ALLRUNTIME_ADD          460      //累计运行时间存储地址
//运行时间保存
#define FRIST_PROGRAM_ADD  1000        //程序组保存 大小约15 *100组 预设空间

#define SAVEFLAG_ADD      500      //用于判断是否数据  写入数据后标志不为初值

#define SAVEFLAG_DATA     0xAA
//读取升降速档位  初始每个转子号对应的升降速档位
void read_eepromRotorProgranData(void)
{
	u8 ucI;
	
	u8 ucFlag;

	ee_ReadBytes((uint8_t *)&ucFlag,   SAVEFLAG_ADD ,1 );
	
	ee_ReadBytes((uint8_t *)&RotorPrgData,   FRIST_ROTORPROGDATA_ADD ,sizeof(RotorPrgData) );

	for(ucI =0;ucI<MAX_ROTOR;ucI++)
	{
		if(RotorPrgData.ucAcc[ucI] <1 || RotorPrgData.ucAcc[ucI] >10 || ucFlag!= SAVEFLAG_DATA) RotorPrgData.ucAcc[ucI] =4;
		if(RotorPrgData.ucDec[ucI] <1 ||RotorPrgData.ucDec[ucI] >10 || ucFlag!= SAVEFLAG_DATA) RotorPrgData.ucDec[ucI] =5;
		
//		if( RotorPrgData.uiSpeed[ucI] <100 || RotorPrgData.uiSpeed[ucI] > Speed_max[HldReg[ROTOR]]|| ucFlag!= SAVEFLAG_DATA) RotorPrgData.uiSpeed[ucI] =Speed_max[HldReg[ROTOR]];
//		if( RotorPrgData.uiRCF[ucI]   > 10 || RotorPrgData.uiRCF[ucI]   > Force_max[HldReg[ROTOR]]|| ucFlag!= SAVEFLAG_DATA) RotorPrgData.uiRCF[ucI] =Force_max[HldReg[ROTOR]];
//		
	}
	 
	ucFlag = SAVEFLAG_DATA;
	ee_WriteBytes((uint8_t *)&ucFlag,   SAVEFLAG_ADD ,1 );
}
//保存升降速档位  每个转子号对应的升降速档位
void write_eepromRotorProgranData(void)
{
	u8 ucI;

	ee_WriteBytes((uint8_t *)&RotorPrgData,   FRIST_ROTORPROGDATA_ADD ,sizeof(RotorPrgData) );

}
//读取累计运行时间
void read_eepromAllRunTime(void)
{
	ee_ReadBytes((uint8_t *)&AllRunTime,   ALLRUNTIME_ADD ,sizeof(AllRunTime) );
}

//保存升降速档位  每个转子号对应的升降速档位
void write_eepromAllRunTime(void)
{
	u8 ucI;

	ee_WriteBytes((uint8_t *)&AllRunTime,   ALLRUNTIME_ADD ,sizeof(AllRunTime) );

}

//取转子对应的运行数据信息
void get_RotorProgranData( void)
{
//	HldReg[SET_SPEED] = RotorPrgData.uiSpeed[HldReg[ROTOR]];
//	HldReg[SET_RCF] = RotorPrgData.uiRCF[HldReg[ROTOR]];
//	HldReg[SET_TIME_HH] = RotorPrgData.ucTime_HH[HldReg[ROTOR]];
//	HldReg[SET_TIME_MM] = RotorPrgData.uiTime_MM[HldReg[ROTOR]];
//	HldReg[SET_TIME_SS] = RotorPrgData.ucTime_SS[HldReg[ROTOR]];
//	HldReg[SET_TEMPERATURE] = RotorPrgData.iTemprature[HldReg[ROTOR]];
	
	HldReg[SET_SPEED] = Speed_max[HldReg[ROTOR]];   //取最大转速和离心力
	HldReg[SET_RCF] = Force_max[HldReg[ROTOR]];
	
	HldReg[ACC] = RotorPrgData.ucAcc[HldReg[ROTOR]];
	HldReg[DEC] = RotorPrgData.ucDec[HldReg[ROTOR]];
	
	if(g_ucfDispXg)  HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//显示离心力
	else HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
}
//保存转子对应的运行数据信息
void set_RotorProgranData(void)
{
//		RotorPrgData.uiSpeed[HldReg[ROTOR]] = HldReg[SET_SPEED] ;
//		RotorPrgData.uiRCF[HldReg[ROTOR]]= HldReg[SET_RCF];
//		RotorPrgData.ucTime_HH[HldReg[ROTOR]]= HldReg[SET_TIME_HH] ;
//		RotorPrgData.uiTime_MM[HldReg[ROTOR]]= HldReg[SET_TIME_MM] ;
//		RotorPrgData.ucTime_SS[HldReg[ROTOR]]=HldReg[SET_TIME_SS] ;
//		RotorPrgData.iTemprature[HldReg[ROTOR]]= HldReg[SET_TEMPERATURE];
	
		RotorPrgData.ucAcc[HldReg[ROTOR]] = (vu8)HldReg[ACC];
		RotorPrgData.ucDec[HldReg[ROTOR]] = (vu8)HldReg[DEC];
	

}

//读取一组数据
void read_eepromOneProgData(u8 _ProgNO)
{
	u8 progDataLen;
	progDataLen =sizeof(OneProgramData);
	ee_ReadBytes((uint8_t *)&OneProgramData, FRIST_PROGRAM_ADD+progDataLen*_ProgNO ,progDataLen );//(MAX_SET_NUMBER - START_SET_NUMBER+1)
	if(OneProgramData.ucRotorNo <1 || OneProgramData.ucRotorNo >RotorMaxNum) OneProgramData.ucRotorNo =2;
	if(OneProgramData.iTemprature <-20 || OneProgramData.iTemprature >40) OneProgramData.iTemprature =4;
	if(OneProgramData.ucAcc >10 || OneProgramData.ucAcc<1)  OneProgramData.ucAcc =4;
	if(OneProgramData.ucDec >10 || OneProgramData.ucDec<1)  OneProgramData.ucDec =5;
	if(OneProgramData.ucTime_HH >99) OneProgramData.ucTime_HH =0;
	if(OneProgramData.ucTime_SS >59) OneProgramData.ucTime_SS =0;
	if(OneProgramData.uiTime_MM >99) OneProgramData.uiTime_MM =0;
	if(OneProgramData.uiSpeed > Speed_max[OneProgramData.ucRotorNo] || OneProgramData.uiSpeed<100) OneProgramData.uiSpeed = Speed_max[OneProgramData.ucRotorNo];
	if(OneProgramData.uiRCF > Force_max[OneProgramData.ucRotorNo] || OneProgramData.uiRCF<10) OneProgramData.uiRCF = Force_max[OneProgramData.ucRotorNo];
	if(OneProgramData.ucTime_HH ==0 && OneProgramData.uiTime_MM ==0 && OneProgramData.ucTime_SS==0)  OneProgramData.uiTime_MM=10;
}
//写入一组数据
void write_eepromOneProgData(u8 _ProgNO)
{
	u8 progDataLen;
	progDataLen =sizeof(OneProgramData);
	ee_WriteBytes((uint8_t *)&OneProgramData, FRIST_PROGRAM_ADD+progDataLen*_ProgNO ,progDataLen );//(MAX_SET_NUMBER - START_SET_NUMBER+1)
}
//取程序组数据到运行数据
void get_programData(PROGRAMDATA*  _ProgramData)
{
	
	HldReg[ROTOR] = _ProgramData->ucRotorNo;
	HldReg[SET_SPEED] = _ProgramData->uiSpeed;
	HldReg[SET_RCF] = _ProgramData->uiRCF;
	HldReg[SET_TIME_HH] = _ProgramData->ucTime_HH;
	HldReg[SET_TIME_MM] = _ProgramData->uiTime_MM;
	HldReg[SET_TIME_SS] = _ProgramData->ucTime_SS;
	HldReg[SET_TEMPERATURE] = _ProgramData->iTemprature;
	HldReg[ACC] = _ProgramData->ucAcc;
	HldReg[DEC] = _ProgramData->ucDec;
	
	if(g_ucfDispXg)  HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//显示离心力
	else HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
}
//设置程序组数据   取运行数据到程序组数据
void set_programData(PROGRAMDATA*  _ProgramData)
{
	_ProgramData->ucRotorNo     = HldReg[ROTOR];
	_ProgramData->uiSpeed     = HldReg[SET_SPEED];
	_ProgramData->uiRCF       = HldReg[SET_RCF];
	_ProgramData->ucTime_HH   = HldReg[SET_TIME_HH];
	_ProgramData->uiTime_MM   = HldReg[SET_TIME_MM];
	_ProgramData->ucTime_SS   = HldReg[SET_TIME_SS];
	_ProgramData->iTemprature = HldReg[SET_TEMPERATURE];
	_ProgramData->ucAcc       = HldReg[ACC];
  _ProgramData->ucDec       = HldReg[DEC];
}
void Read_eepromData(void)
{
	u16 i;
	uint8_t read_rom[MAX_SET_NUMBER*2] ={0};
	
	
	for(i = 0; i < 2000; i++);
	ee_ReadBytes(read_rom, 0,MAX_SET_NUMBER*2 );//(MAX_SET_NUMBER - START_SET_NUMBER+1)
	for(i = 0; i < MAX_SET_NUMBER; i++) HldReg[i] = (read_rom[(i << 1)] << 8) + read_rom[(i << 1) + 1];//获取运行参数
	Maxmin_Param(0);  //限制参数的最大值和最小值
//	for(i = 0; i < 2000; i++);
//	ee_ReadBytes(read_rom, 1600, 800);//
//	for(i = 0; i < 400; i++) AlarmRecord[i] = (read_rom[(i << 1)] << 8) + read_rom[(i << 1) + 1];//获取报警记录参数	
	read_eepromRotorProgranData();
	
	read_eepromAllRunTime();
	
	Balance_rede();//读取不平衡阈值

}
void Save_eepromData(void)
{
		u16 i;
	static u8 save_step=0;//,TemperatureSample=0;
	uint8_t write_rom[200];
//	HldReg[SYS_TYPE] =  1;
	HldReg[MOTOR_TYPE] =  QK_VFD;
	
 
	if(writeDataFlag ==1 )    //写数据标志
	{
		writeDataFlag =0;
		setData_flag =100;
	for (i = 0; i < (MAX_SET_NUMBER ); i ++) {write_rom[(i << 1)] = HldReg[i+START_SET_NUMBER] >> 8;write_rom[(i << 1) + 1 ] = HldReg[i+START_SET_NUMBER];}
				ee_WriteBytes(write_rom, FRIST_SAVE_RUN_DATA_ADD,MAX_SET_NUMBER*2);//保存数据，避开历史数据记录、历史记录查询、历史数据清零
				write_eepromRotorProgranData();
		if(HldReg[PROGRAM] <= MAX_PROGRAM ) write_eepromOneProgData(HldReg[PROGRAM]);   //保存程序组数据 只保存20组数据
	}
//		switch (save_step)//写400个数据的时间为19ms
//		{
//			case 0:	
//				for (i = 0; i < (MAX_SET_NUMBER ); i ++) {write_rom[(i << 1)] = HldReg[i+START_SET_NUMBER] >> 8;write_rom[(i << 1) + 1 ] = HldReg[i+START_SET_NUMBER];}
//					ee_WriteBytes(write_rom, FRIST_SAVE_RUN_DATA_ADD,MAX_SET_NUMBER*2);//保存数据，避开历史数据记录、历史记录查询、历史数据清零
//				break;
//			case 1:	
////				write_eepromOneProgData(HldReg[PROGRAM]);   //保存程序组数据
//				break;
//			case 2:	
//				write_eepromRotorProgranData();
//				break;
//			case 3:	
////				write_eepromAllRunTime();			
//				break;			
//	
//			default:
//				break;
//		}
		if (save_step <3) save_step ++;
		else save_step = 0;
		
		Balance_Storage();
}




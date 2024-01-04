/*
*************************************************************
			文件名：	app_btn_scan.c
			功	 能：	所有按键扫描函数文件
			版	 本：    V1.0
			时	 间：    2013年1月2号
			作	 者：    彭	赞
			版	 权：    长沙启科电子

修改日志：
————————————————————————————————————————————————————
修改时间   		修改人		修改内容/原因
2013年1月2号	彭	赞		建立文件

————————————————————————————————————————————————————
			
*************************************************************
*/

#include "typedef.h"

extern u8 ShortRun_flag;
extern u8 g_ucState;
u8 set_prog_flag;

u8 prog_state =0,select_prog =0,disp_page =0;

//addKey_Time[0]为进行设置增加键响应一次按键的时间间隔定时器
//addKey_Time[1]为记录一次按键的定时器


//确定键定时器
//TIME_TYPE enterKey_Timer;

vu8 keySET_State,highSetFlag; //设置键状态

//按键信号
KEY_SIGNAL keySignal2;

//初始化键盘信号参数
void InitKeySignal(void)
{
	keySignal2.setSignal = 0;
	keySignal2.addSignal = 0;
	keySignal2.subSignal = 0;
	keySignal2.enterSignal = 0;
	keySignal2.cutSignal = 0;
	keySignal2.openSignal = 0;
	keySignal2.startSignal = 0;
	keySignal2.stopSignal = 0;
}

//设置键扫描
void SETKey_Scan(void)
{
	 //设置键
 	if( SET_KEY)
	{
		if(keySignal2.setSignal == 0)
		{
			keySignal2.setSignal = 1;

			if(highSetFlag == 0)
			{  
				SET_Proc(); 					//响应SET状态下的按键
			}
			else if(highSetFlag == 1)
			{
				//HighStateCut_Proc();
			}
			else if(highSetFlag == 2)
			{
			//	HighStateCut_Proc();//GS_MoveToNext();
			}
		}
	}
	else
	{
		keySignal2.setSignal = 0;
	}
}

//高级设置中的加键扫描
void HighADDKey_Scan(void)
{
/*	if(addKey_Time[0].signal)
	{
		addKey_Time[0].counter = 0;
		addKey_Time[0].signal = 0;
		SetTimer(&addKey_Time[0],50);
		//锁机时间快加
		LockTimeQAdd();
	}
	if(getKeyState(ADD_KEY) && !getKeyState(SUB_KEY))//加键
	{
		if(!addKey_Time[0].enable)
		{
			SetTimer(&addKey_Time[0],1400);
		}
		if(keySignal2.addSignal == 0)
		{
			ClearEnterTimerC();
			keySignal2.addSignal = 1;
			Beep_SetTime(1,50, 50);
			HighAddKey_Proc();
		}
	}
	else
	{
		keySignal2.addSignal = 0;
		addKey_Time[0].enable = 0;
		addKey_Time[0].signal = 0;
		addKey_Time[0].counter = 0;
	}
	*/
	
}

//加键扫描
void ADDKey_Scan(void)
{
	static u8 add_n;
	if(ADD_KEY && !(SUB_KEY))//加键
	{
		ADDKEY_TIMER.enable =1;
		if(add_n < 5) ADDKEY_TIMER.number =30;//设置定时器长度
		else ADDKEY_TIMER.number =4;
	
		if(1 == ADDKEY_TIMER.flag || keySignal2.addSignal ==0)
		{
			keySignal2.addSignal = 1;
			ADDKEY_TIMER.flag =0;
			ADDKEY_TIMER.count =0;
			if(add_n <200) add_n++;
			if(highSetFlag == 1 || highSetFlag == 2)
			{
				//HighAddKey_Proc();
			}
			else if(highSetFlag == 0)
			{
				ADDKey_Proc();
			}
		}
	}
	else
	{
		keySignal2.addSignal = 0;
		add_n =0;
	}	
}

//高级设置过程中减键过程
void HighSUBKey_Scan(void)
{
/*	if(subKey_Time[0].signal)
	{
		subKey_Time[0].signal = 0;
		subKey_Time[0].counter = 0;
		SetTimer(&subKey_Time[0], 40);
		LockTimeQSub();
	}
	if(getKeyState(SUB_KEY))
	{
		if(keySignal2.subSignal== 0)
		{
			ClearEnterTimerC();
			keySignal2.subSignal = 1;
			Beep_SetTime(1,50, 50);
			HighSubKey_Proc();
		}
		if(!subKey_Time[0].enable)
		{
			SetTimer(&subKey_Time[0], 1400);
		}
	}
	else
	{
		keySignal2.subSignal = 0;
		subKey_Time[0].counter = 0;
		subKey_Time[0].signal = 0;
		subKey_Time[0].enable = 0;
	}
	*/
}

////////////////////////////////////////////
//减键扫描
void SUBKey_Scan(void)
{
	static u8 dec_n;
	
	if(SUB_KEY && !(ADD_KEY))
	{
		DECKEY_TIMER.enable =1;
		if(dec_n < 5) DECKEY_TIMER.number =30;//设置定时器长度
		else DECKEY_TIMER.number =4;
	
		if(1 == DECKEY_TIMER.flag || keySignal2.subSignal ==0)
		{
		  DECKEY_TIMER.flag =0;
			DECKEY_TIMER.count =0;
			keySignal2.subSignal =1;if(dec_n <200) dec_n++;
			if(highSetFlag == 1|| highSetFlag == 2)
			{
			//	HighSubKey_Proc();
			}
			else if(highSetFlag == 0)
			{
				SUBKey_Proc();
			}
		}
	}
	else
	{
		keySignal2.subSignal = 0;
		dec_n =0;
	}
	
}

/////////////////////////////////////////////////////
//停止键扫描
//是否停止键被按下
u8 IfStopPress(void)
{
	return (STOP_KEY);
}

void STOPKey_Scan(void)
{
	if( STOP_KEY)
	{
		if(keySignal2.stopSignal == 0)//开门锁
		{
			keySignal2.stopSignal = 1;
			//状态不变，开门锁
			if(highSetFlag == 0 && GetAlarmFlag() != NO_ALARM)
			{	
				STOPKey_Proc();
			}
			else if((highSetFlag == 0 || highSetFlag == 3 ) && runFlag.curRunFlag == 0)
			{
				OPENKey_Proc();
			}  
			else if(highSetFlag == 1)
			{
				//EndHighSet_Proc();
			}
			
		}
	}
	else
	{
		keySignal2.stopSignal = 0;
	}
//	if(!highSetFlag)
//		WaitToStopLock();
}



//清确定键计时时间
void ClearEnterTimerC(void)
{								  
///	enterKey_Timer.counter = 0;
//	enterKey_Timer.tNum = 0;
//	enterKey_Timer.signal = 0;	
}

//确定键扫描
void ENTERKey_Scan(void)
{
	if( ENTER_KEY && !(SET_KEY) )
	{
		if(keySignal2.enterSignal != 1)
		{
			keySignal2.enterSignal = 1;
			if(highSetFlag == 1 || highSetFlag == 2)
			{
			//	HighEnter_Proc();
			}
			else if(highSetFlag == 0)
			{
				ENTERKey_Proc();  //确定键处理过程
				
			}
		}
	}
	else
	{
		keySignal2.enterSignal = 0;
	}

}

//切换键扫描
void CUTKey_Scan(void)
{
	if( CUT_KEY)
	{
		if(keySignal2.cutSignal == 0)
		{
			keySignal2.cutSignal = 1;
			if(	highSetFlag != 1)
			{
				CUTKey_Proc();	//切换离心力/转速显示
			}
			return ;
		}
	}
	else
	{
		keySignal2.cutSignal = 0;
	}
	
}
//开门键扫描
void OpenKey_Scan(void)
{
	if( OPEN_KEY)
	{
		if(keySignal2.openSignal == 0)
		{
			keySignal2.openSignal = 1;
			OPENKey_Proc();
		}
	}
	else
	{
		keySignal2.openSignal = 0;
	}
}
//按键扫描
void Key_Proc(void)
{

	SETKey_Scan();        //设置键扫描
	CUTKey_Scan();
//	SPEEDKey_Scan(); 
//	TIMEKey_Scan(); 
//	TEMPKey_Scan(); 
//	ROTORKey_Scan(); 
//	ProgKEY_scan();
	if(keySET_State == 1 || highSetFlag != 0)
	{
		ADDKey_Scan();        //加键扫描
		SUBKey_Scan();        //减键扫描
		ENTERKey_Scan();      //确定键扫描	
	}
  //OpenKey_Scan();      //开门键
	STOPKey_Scan();       //停止键扫描



}

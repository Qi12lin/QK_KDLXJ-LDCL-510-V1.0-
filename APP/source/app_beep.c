/*******************************************************
				文件：beep_proc.c
				功能：包含对蜂鸣器的使用方法和使用场合
				版本：V1.0
				作者：Pz
				日期：2012.11.2
				备注：
********************************************************/
#include "typedef.h"

BEEP_TYPE beepInfo; //蜂鸣器参数

//蜂鸣器初始化
void BeepInit(void)
{
	GPIO_InitTypeDef TypeDef;
 
	//LED GPIO时钟配置
 	RCC_APB1PeriphClockCmd(BEEP_RCC , ENABLE);

	TypeDef.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	TypeDef.GPIO_OType = GPIO_OType_PP;//推挽输出
	TypeDef.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	TypeDef.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);

	TypeDef.GPIO_Pin = BEEP_PIN;
	GPIO_Init(BEEP_PORT, &TypeDef);
	
	beepInfo.beepOnTime = 0;
	beepInfo.beepBusy = 0;
	
}

void Beep_Set(u8 fOn)
{
	if(fOn)
	{
//		GPIO_SetBits(BEEP_PORT, BEEP_PIN);
	} 
	else
	{ 
//		GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
    }
}
//蜂鸣器的参数设置
void Beep_SetTime(u16 cnt, u16 onTime, u16 offTime)
{
    if(beepInfo.beepBusy == 0)
	{
		beepInfo.cnt = cnt;
		beepInfo.beepOnTime = onTime;
		beepInfo.beepCycleOnTime = onTime;
		beepInfo.beepOffTime  = offTime;
		beepInfo.beepCycleOffTime = offTime;
	}
	else
   {}
}

//蜂鸣器的定时（放在1ms的定时器中断中）
void Beep_RecordTimer(void)
{
   if(beepInfo.beepOnTime > 0)
	{
		beepInfo.beepOnTime--;
		beepInfo.beepBusy = 1;
		//beepInfo.beepSignal = 1;
		Beep_Set(1);
	}
	else
	{
		if(beepInfo.cnt > 1)
	    {
			if(beepInfo.beepOffTime > 0)
			{
				beepInfo.beepOffTime--;
				//beepInfo.beepSignal = 0;
				Beep_Set(0);
			}
			else
			{
				beepInfo.beepOffTime = beepInfo.beepCycleOffTime;
				beepInfo.beepOnTime = beepInfo.beepCycleOnTime;
				beepInfo.cnt--;
			}
		}
		else
	    {
			beepInfo.beepBusy = 0;
			//beepInfo.beepSignal = 0;
			Beep_Set(0);
		}
	}
}

//清除蜂鸣器
void ClearBeep(void)
{
	beepInfo.beepBusy = 0;
	beepInfo.cnt = 0;
	beepInfo.beepSignal = 0;
	//Beep_Set(0);
}

//蜂鸣器忙
u8 BeepBusy(void)
{
	return beepInfo.beepOnTime;
}

//蜂鸣器的处理过程
void Beep_Proc(void)
{
	static vu8 vucKeysignal = 0;
	static vu8 vucAlarmSignal = 0,alarm_no_temp = 0;;
	 	
	if(VK_KEY1 | VK_KEY2| VK_KEY3| VK_KEY7| VK_KEY8| VK_KEY9| VK_KEY4| VK_KEY10)//
	{
		if(vucKeysignal == 0)
		{
			vucKeysignal = 1;
		    Beep_SetTime(1, 50, 50);
		}
	}
	else
	{
		vucKeysignal = 0;	
	}
	if(GetAlarmFlag()  && alarm_no_temp != GetAlarmFlag())
	{
//		if (HldReg[FUNCTION_Y1Y2] == 2) //2嘉美高科
//		{//有报警时一直响
//			if(vucAlarmSignal == 0)
//			{
//				vucAlarmSignal = 1;
//				Beep_SetTime(65530, 300, 700);	
//			}
//			if (alarm_no > alarm_no_temp) vucAlarmSignal = 0;//新增报警时蜂鸣器响
//			else {}	
//		}
//		else
//		{//有报警时响三声
			if(vucAlarmSignal == 0)
			{
				vucAlarmSignal = 1;beepInfo.beepBusy = 0;
					Beep_SetTime(3, 500, 500);
			}
			if (alarm_no > alarm_no_temp) vucAlarmSignal = 0;//新增报警时蜂鸣器响
			else {}			
//		}
	}
	else
	{
		vucAlarmSignal = 0;
//		if (HldReg[FUNCTION_Y1Y2] == 2) beepInfo.cnt = 0;//清除报警
	}
	alarm_no_temp = GetAlarmFlag();
}


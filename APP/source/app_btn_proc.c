#include "typedef.h"

vu8 DTLED_groud ; //数码管组
u8 DTLED_groud_set,DTLED_groud_set_no; 
//响应SET状态下的按键
void SET_Proc(void)
{
	if(keySET_State == 0)
	{
		keySET_State = 1;
		if(runFlag.motorRunFlag != 1)
		{
			DTLED_groud = GROUP_PROG; //进入设置状态
		}
		else 
		{
			DTLED_groud = GROUP_FORCE_SPEED;
		}
		DTLED_groud_set = DTLED_groud+1;
	}
	else
	{
		DTLED_groud_set_no = DTLED_groud+1;
		DTLED_groud++;

		switch(DTLED_groud)
		{
		case GROUP_ROTOR:
		break;
		case GROUP_PROG:
		break;
		case GROUP_FORCE_SPEED:
		break;
		case GROUP_HOUR:
		break;
		case GROUP_TIME:
		break;
		case GROUP_SECOND:
		break;
		case GROUP_UPGEAR:
		break;
		case GROUP_DOWNGEAR:

		break;
		case GROUP_TEMP:
		break;
//		case GROUP_FORCE:
//			break;
		default:
			if(runFlag.motorRunFlag != 1)
			{
				DTLED_groud = GROUP_PROG; 
			}
			else
			{
				DTLED_groud = GROUP_FORCE_SPEED;
			}
			break;
		}
		DTLED_groud_set = DTLED_groud+1;
	}
}

//离心力与转速增加
void FC_Speed_add(u8 uXgState)
{

	if (uXgState != 1)
	{
		if(HldReg[SET_SPEED] % 50 != 0)
		{
			HldReg[SET_SPEED] = HldReg[SET_SPEED] - (HldReg[SET_SPEED] % 50);
		}
		HldReg[SET_SPEED] += 50;

		//进行判断是否已经到达转子号对应的最大转速

		if(Speed_max[HldReg[ROTOR]] < HldReg[SET_SPEED])
		{
			HldReg[SET_SPEED] = Speed_max[HldReg[ROTOR]];
		}
		else
		{
		}
		//离心力的重新计算
		HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],
														HldReg[ROTOR]);
	}
	else if(uXgState != 0)
	{
		HldReg[SET_RCF] += 10;
		//进行判断是否已经到达转子号对应的最大的离心力
		if(Force_max[HldReg[ROTOR]] < HldReg[SET_RCF])
		{
				HldReg[SET_RCF] = Force_max[HldReg[ROTOR]];
		}
		else
		{
		}
		//转速的重新计算
		HldReg[SET_SPEED]= Force_to_Speed(HldReg[SET_RCF],
														HldReg[ROTOR]);
		if(HldReg[SET_SPEED] < 100)
		{
				HldReg[SET_SPEED] = 100;
		}
    }
	else
	{
	}
	
	if (g_ucfDispXg != 1)
	{
		if(HldReg[SET_SPEEDRCF] % 50 != 0)
		{
			HldReg[SET_SPEEDRCF] = HldReg[SET_SPEEDRCF] - (HldReg[SET_SPEEDRCF] % 50);
		}
		HldReg[SET_SPEEDRCF] += 50;

		//进行判断是否已经到达转子号对应的最大转速

		if(Speed_max[HldReg[ROTOR]] < HldReg[SET_SPEEDRCF])
		{
			HldReg[SET_SPEEDRCF] = Speed_max[HldReg[ROTOR]];
		}
		else
		{
		}
		HldReg[SET_SPEED] = HldReg[SET_SPEEDRCF] ;
		//离心力的重新计算
		HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],
														HldReg[ROTOR]);
	}
	else if(g_ucfDispXg != 0)
	{
		HldReg[SET_SPEEDRCF] += 10;
		//进行判断是否已经到达转子号对应的最大的离心力
		if(Force_max[HldReg[ROTOR]] < HldReg[SET_SPEEDRCF])
		{
				HldReg[SET_SPEEDRCF] = Force_max[HldReg[ROTOR]];
		}
		else
		{
		}
		HldReg[SET_RCF] = HldReg[SET_SPEEDRCF] ;
		//转速的重新计算
		HldReg[SET_SPEED]= Force_to_Speed(HldReg[SET_RCF],
														HldReg[ROTOR]);
		if(HldReg[SET_SPEED] < 100)
		{
				HldReg[SET_SPEED] = 100;
		}
    }
	else
	{
	}
}

//加键过程处理
void ADDKey_Proc(void)
{

		switch (DTLED_groud)
		{
			case GROUP_PROG:
				if (HldReg[PROGRAM] < MAX_PROGRAM)
				{	
					HldReg[PROGRAM]++;
					read_eepromOneProgData(HldReg[PROGRAM]);    //读取EEPROM数据
					get_programData(&OneProgramData);
				}
				else
				{
					HldReg[PROGRAM] = MAX_PROGRAM;
				}	
				break;
			case GROUP_ROTOR://转子号
				if (HldReg[ROTOR] < ROTOR_MAX)
				{	
					HldReg[ROTOR]++;

					get_RotorProgranData();
				}
				else
				{HldReg[ROTOR] = ROTOR_MAX;}	
			break;
			case GROUP_FORCE_SPEED://离心力与转速
					FC_Speed_add(0);
				break;	
			case GROUP_FORCE:
				FC_Speed_add(1);
				break;
			case GROUP_HOUR:		//时间
				if (HldReg[SET_TIME_HH] < 99) 
				{
						HldReg[SET_TIME_HH]++;
				}
				else HldReg[SET_TIME_HH] = 99;				
				break;	
			case GROUP_TIME:		//时间
				if (HldReg[SET_TIME_MM] < TIMER_MAX) 
				{
						HldReg[SET_TIME_MM]++;
				}
				else HldReg[SET_TIME_MM] = TIMER_MAX;				
				break;	
			case GROUP_SECOND:
				if(HldReg[SET_TIME_SS] < 59)
				{
						HldReg[SET_TIME_SS]++;
				}
				else  HldReg[SET_TIME_SS] = 59;
				break;
			case GROUP_UPGEAR://升速档位
				if ((HldReg[ACC] + 1)<= GEAR_NUM)
				{
						HldReg[ACC] ++;
				}
				else HldReg[ACC] = GEAR_NUM;			
				break;
			case GROUP_DOWNGEAR://降速档位
				if ((HldReg[DEC] + 1)<= GEAR_NUM)
				{				
						HldReg[DEC] ++;
				}
				else HldReg[DEC] = GEAR_NUM;		
				break;
			case GROUP_TEMP:
				if(((s16)HldReg[SET_TEMPERATURE] + 1) <= 40)
				{
					HldReg[SET_TEMPERATURE] += 1;
				}
				else HldReg[SET_TEMPERATURE] =40;
			break;
			default:
				break;
		
	}
}

//离心力和转速的减少
void FC_Speed_sub(u8 uXgState)
{

	if (uXgState != 1)
	{
		if(HldReg[SET_SPEED] >= 150)
		{
			if(HldReg[SET_SPEED] % 50 != 0)
			{
				HldReg[SET_SPEED] = HldReg[SET_SPEED] - (HldReg[SET_SPEED] % 50);
			}
			else
			{
				HldReg[SET_SPEED] -= 50;
			}
		}
		else
		{
			HldReg[SET_SPEED] = 100;
		}
		//离心力的重新计算
		HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],
														HldReg[ROTOR]);
	}
	else if(uXgState != 0)
	{
		if(HldReg[SET_RCF] >= 20)
		{
			HldReg[SET_RCF] -= 10;
		}
		else
		{
			HldReg[SET_RCF] = 10;
		}
		//转速的重新计算
		HldReg[SET_SPEED] = Force_to_Speed( HldReg[SET_RCF],
														   HldReg[ROTOR]);
		if(HldReg[SET_SPEED] <= 100)
		{
			HldReg[SET_SPEED] = 100;
		}
	}
	else
	{
	}
	
	if (g_ucfDispXg != 1)
	{
		if(HldReg[SET_SPEEDRCF] >= 150)
		{
			if(HldReg[SET_SPEEDRCF] % 50 != 0)
			{
				HldReg[SET_SPEEDRCF] = HldReg[SET_SPEEDRCF] - (HldReg[SET_SPEEDRCF] % 50);
			}
			else
			{
				HldReg[SET_SPEEDRCF] -= 50;
			}
		}
		else
		{
			HldReg[SET_SPEEDRCF] = 100;
		}
		HldReg[SET_SPEEDRCF] = HldReg[SET_SPEEDRCF];
		//离心力的重新计算
		HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],
														HldReg[ROTOR]);
	}
	else if(g_ucfDispXg != 0)
	{
		if(HldReg[SET_SPEEDRCF] >= 20)
		{
			HldReg[SET_SPEEDRCF] -= 10;
		}
		else
		{
			HldReg[SET_SPEEDRCF] = 10;
		}
		HldReg[SET_RCF] = HldReg[SET_SPEEDRCF];	
		//转速的重新计算
		HldReg[SET_RCF] = Force_to_Speed( HldReg[SET_RCF],
														   HldReg[ROTOR]);
		if(HldReg[SET_SPEED] <= 100)
		{
			HldReg[SET_SPEED] = 100;
		}
	}
	else
	{
	}
}

//减键过程处理
void SUBKey_Proc(void)
{

		switch (DTLED_groud)
		{
			case GROUP_PROG:
				if (HldReg[PROGRAM] >1)
				{	
					HldReg[PROGRAM]--;
					read_eepromOneProgData(HldReg[PROGRAM]);    //读取EEPROM数据
					get_programData(&OneProgramData);
				}
				else
				{}	
				break;
			case GROUP_ROTOR://转子号
				if (HldReg[ROTOR] > 1)
				{
					HldReg[ROTOR]--;
					 		//更新转速/离心力数据		
					get_RotorProgranData();
				}
				else {}						
				break;
			case GROUP_FORCE_SPEED://离心力与转速
				FC_Speed_sub(0);
				break;	
			case GROUP_FORCE://离心力与转速
				FC_Speed_sub(1);
				break;	
			case GROUP_HOUR:		//时间
				if(HldReg[SET_TIME_HH] > 0)
				{
					HldReg[SET_TIME_HH]--;
				}
				else {}				
				break;
			case GROUP_TIME:		//时间
				if(HldReg[SET_TIME_MM] > 0)
				{
					HldReg[SET_TIME_MM]--;
				}
				else {}				
				break;	
			case GROUP_SECOND:
				if(HldReg[SET_TIME_SS] > 0)
				{
					 HldReg[SET_TIME_SS]--;
				}
				break;
			case GROUP_UPGEAR://升速档位
				if (HldReg[ACC] >= 2) 
				{
					HldReg[ACC]--;
				}
				else HldReg[ACC] = 1;		
				break;
			case GROUP_DOWNGEAR:
				if(HldReg[DEC] >= 2)
				{	
					HldReg[DEC]--;
				}
				else 	HldReg[DEC] = 1;
				break;
			case GROUP_TEMP:
				if(((s16)HldReg[SET_TEMPERATURE] - 1) >= -20)
				{
					HldReg[SET_TEMPERATURE] -= 1;
				}
				else HldReg[SET_TEMPERATURE] =-20;
			break;
			default:
				break;
			}
		
	}
											 
//停止键的过程处理
void STOPKey_Proc(void)
{

}
//切换转速状态的过程
void CutSpeed_Proc(void)
{
/*	
	if(keySET_State || !runFlag.motorRunFlag)
	{
			DTLEDs_UD_SpOrFc(HldReg[SET_SPEED], 0, 1);
    }
	else
	{
	}
	*/
}

//切换离心力状态过程
void CutForce_Proc(void)
{
/*	SetLEDState(CUT_LED_TYPE, 1);
	if(keySET_State || !runFlag.motorRunFlag)
	{
			DTLEDs_UD_SpOrFc(HldReg[SET_RCF], 1, 1);
    }
	else
	{
	}
	*/
}

//切换到累计离心积状态
void CutForceSum_Proc(void)
{
//	DispForceSum(1);
}

//切换到累计时间状态
void CutTimeSum_Proc(void)
{
//	DispTimeSum(1);
}

//切换键的过程处理
void CUTKey_Proc(void)
{
/*	CutFlag++;
	if(CutFlag >= CUT_MAX_NUM)  CutFlag = SPEED_STATE;//计算切换状态
	switch(CutFlag)
	{
		case SPEED_STATE: //转速状态
				CutSpeed_Proc();
		break;
		case FORCE_STATE://离心力状态
				CutForce_Proc();
		break;
		case FORCE_SUM_STATE: //累计离心积
				CutForceSum_Proc();
		break;
		case TIME_SUM_STATE:  //累计时间
				CutTimeSum_Proc();
		break;
		default:
		break;
	}
*/	
	if(g_ucfDispXg ==0) g_ucfDispXg =1;
	else g_ucfDispXg =0;
	
	
}


//确定键的过程处理
void ENTERKey_Proc(void)
{
	
	if(keySET_State) DTLED_groud_set_no = DTLED_groud+1;
	//退出设置状态
	keySET_State = 0;
	Maxmin_Param(1);//	
	set_programData(&OneProgramData); //保存程序组数据
	set_RotorProgranData();           //保存转子对应的运行数据
	writeDataFlag =1;  //  保存参数
}
//转子号键的过程处理
void ROTORKey_Proc(void)
{
		if(runFlag.motorRunFlag != 1)
		{
			keySET_State = 1;
			DTLED_groud = GROUP_ROTOR; //进入设置状态
		}
}
//时间键的过程处理
void TIMEKey_Proc(void)
{

			keySET_State = 1;
			if(DTLED_groud == GROUP_TIME) DTLED_groud = GROUP_SECOND; //进入设置状态
			else DTLED_groud = GROUP_TIME; //进入设置状态
}
//转速键的过程处理
void SPEEDKey_Proc(void)
{
			keySET_State = 1;
			DTLED_groud = GROUP_FORCE_SPEED; //进入设置状态
}
//温度键的过程处理
void TEMPKey_Proc(void)
{
			keySET_State = 1;
			DTLED_groud = GROUP_TEMP; //进入设置状态
}
//组合键1的过程处理
void Comb1_Proc(void)
{
	//清除累计时间
//	ClearTimeSum();
//	DispTimeSum(1);

}

void OPENKey_Proc(void)
{
	if(!runFlag.curRunFlag && watch_CurSpeed < 40 )
	{
		OnDrSgn = 1;
	}
}


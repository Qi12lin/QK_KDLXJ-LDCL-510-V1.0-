#include "typedef.h"

vu8 DTLED_groud ; //�������
u8 DTLED_groud_set,DTLED_groud_set_no; 
//��ӦSET״̬�µİ���
void SET_Proc(void)
{
	if(keySET_State == 0)
	{
		keySET_State = 1;
		if(runFlag.motorRunFlag != 1)
		{
			DTLED_groud = GROUP_PROG; //��������״̬
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

//��������ת������
void FC_Speed_add(u8 uXgState)
{

	if (uXgState != 1)
	{
		if(HldReg[SET_SPEED] % 50 != 0)
		{
			HldReg[SET_SPEED] = HldReg[SET_SPEED] - (HldReg[SET_SPEED] % 50);
		}
		HldReg[SET_SPEED] += 50;

		//�����ж��Ƿ��Ѿ�����ת�ӺŶ�Ӧ�����ת��

		if(Speed_max[HldReg[ROTOR]] < HldReg[SET_SPEED])
		{
			HldReg[SET_SPEED] = Speed_max[HldReg[ROTOR]];
		}
		else
		{
		}
		//�����������¼���
		HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],
														HldReg[ROTOR]);
	}
	else if(uXgState != 0)
	{
		HldReg[SET_RCF] += 10;
		//�����ж��Ƿ��Ѿ�����ת�ӺŶ�Ӧ������������
		if(Force_max[HldReg[ROTOR]] < HldReg[SET_RCF])
		{
				HldReg[SET_RCF] = Force_max[HldReg[ROTOR]];
		}
		else
		{
		}
		//ת�ٵ����¼���
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

		//�����ж��Ƿ��Ѿ�����ת�ӺŶ�Ӧ�����ת��

		if(Speed_max[HldReg[ROTOR]] < HldReg[SET_SPEEDRCF])
		{
			HldReg[SET_SPEEDRCF] = Speed_max[HldReg[ROTOR]];
		}
		else
		{
		}
		HldReg[SET_SPEED] = HldReg[SET_SPEEDRCF] ;
		//�����������¼���
		HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED],
														HldReg[ROTOR]);
	}
	else if(g_ucfDispXg != 0)
	{
		HldReg[SET_SPEEDRCF] += 10;
		//�����ж��Ƿ��Ѿ�����ת�ӺŶ�Ӧ������������
		if(Force_max[HldReg[ROTOR]] < HldReg[SET_SPEEDRCF])
		{
				HldReg[SET_SPEEDRCF] = Force_max[HldReg[ROTOR]];
		}
		else
		{
		}
		HldReg[SET_RCF] = HldReg[SET_SPEEDRCF] ;
		//ת�ٵ����¼���
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

//�Ӽ����̴���
void ADDKey_Proc(void)
{

		switch (DTLED_groud)
		{
			case GROUP_PROG:
				if (HldReg[PROGRAM] < MAX_PROGRAM)
				{	
					HldReg[PROGRAM]++;
					read_eepromOneProgData(HldReg[PROGRAM]);    //��ȡEEPROM����
					get_programData(&OneProgramData);
				}
				else
				{
					HldReg[PROGRAM] = MAX_PROGRAM;
				}	
				break;
			case GROUP_ROTOR://ת�Ӻ�
				if (HldReg[ROTOR] < ROTOR_MAX)
				{	
					HldReg[ROTOR]++;

					get_RotorProgranData();
				}
				else
				{HldReg[ROTOR] = ROTOR_MAX;}	
			break;
			case GROUP_FORCE_SPEED://��������ת��
					FC_Speed_add(0);
				break;	
			case GROUP_FORCE:
				FC_Speed_add(1);
				break;
			case GROUP_HOUR:		//ʱ��
				if (HldReg[SET_TIME_HH] < 99) 
				{
						HldReg[SET_TIME_HH]++;
				}
				else HldReg[SET_TIME_HH] = 99;				
				break;	
			case GROUP_TIME:		//ʱ��
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
			case GROUP_UPGEAR://���ٵ�λ
				if ((HldReg[ACC] + 1)<= GEAR_NUM)
				{
						HldReg[ACC] ++;
				}
				else HldReg[ACC] = GEAR_NUM;			
				break;
			case GROUP_DOWNGEAR://���ٵ�λ
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

//��������ת�ٵļ���
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
		//�����������¼���
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
		//ת�ٵ����¼���
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
		//�����������¼���
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
		//ת�ٵ����¼���
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

//�������̴���
void SUBKey_Proc(void)
{

		switch (DTLED_groud)
		{
			case GROUP_PROG:
				if (HldReg[PROGRAM] >1)
				{	
					HldReg[PROGRAM]--;
					read_eepromOneProgData(HldReg[PROGRAM]);    //��ȡEEPROM����
					get_programData(&OneProgramData);
				}
				else
				{}	
				break;
			case GROUP_ROTOR://ת�Ӻ�
				if (HldReg[ROTOR] > 1)
				{
					HldReg[ROTOR]--;
					 		//����ת��/����������		
					get_RotorProgranData();
				}
				else {}						
				break;
			case GROUP_FORCE_SPEED://��������ת��
				FC_Speed_sub(0);
				break;	
			case GROUP_FORCE://��������ת��
				FC_Speed_sub(1);
				break;	
			case GROUP_HOUR:		//ʱ��
				if(HldReg[SET_TIME_HH] > 0)
				{
					HldReg[SET_TIME_HH]--;
				}
				else {}				
				break;
			case GROUP_TIME:		//ʱ��
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
			case GROUP_UPGEAR://���ٵ�λ
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
											 
//ֹͣ���Ĺ��̴���
void STOPKey_Proc(void)
{

}
//�л�ת��״̬�Ĺ���
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

//�л�������״̬����
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

//�л����ۼ����Ļ�״̬
void CutForceSum_Proc(void)
{
//	DispForceSum(1);
}

//�л����ۼ�ʱ��״̬
void CutTimeSum_Proc(void)
{
//	DispTimeSum(1);
}

//�л����Ĺ��̴���
void CUTKey_Proc(void)
{
/*	CutFlag++;
	if(CutFlag >= CUT_MAX_NUM)  CutFlag = SPEED_STATE;//�����л�״̬
	switch(CutFlag)
	{
		case SPEED_STATE: //ת��״̬
				CutSpeed_Proc();
		break;
		case FORCE_STATE://������״̬
				CutForce_Proc();
		break;
		case FORCE_SUM_STATE: //�ۼ����Ļ�
				CutForceSum_Proc();
		break;
		case TIME_SUM_STATE:  //�ۼ�ʱ��
				CutTimeSum_Proc();
		break;
		default:
		break;
	}
*/	
	if(g_ucfDispXg ==0) g_ucfDispXg =1;
	else g_ucfDispXg =0;
	
	
}


//ȷ�����Ĺ��̴���
void ENTERKey_Proc(void)
{
	
	if(keySET_State) DTLED_groud_set_no = DTLED_groud+1;
	//�˳�����״̬
	keySET_State = 0;
	Maxmin_Param(1);//	
	set_programData(&OneProgramData); //�������������
	set_RotorProgranData();           //����ת�Ӷ�Ӧ����������
	writeDataFlag =1;  //  �������
}
//ת�Ӻż��Ĺ��̴���
void ROTORKey_Proc(void)
{
		if(runFlag.motorRunFlag != 1)
		{
			keySET_State = 1;
			DTLED_groud = GROUP_ROTOR; //��������״̬
		}
}
//ʱ����Ĺ��̴���
void TIMEKey_Proc(void)
{

			keySET_State = 1;
			if(DTLED_groud == GROUP_TIME) DTLED_groud = GROUP_SECOND; //��������״̬
			else DTLED_groud = GROUP_TIME; //��������״̬
}
//ת�ټ��Ĺ��̴���
void SPEEDKey_Proc(void)
{
			keySET_State = 1;
			DTLED_groud = GROUP_FORCE_SPEED; //��������״̬
}
//�¶ȼ��Ĺ��̴���
void TEMPKey_Proc(void)
{
			keySET_State = 1;
			DTLED_groud = GROUP_TEMP; //��������״̬
}
//��ϼ�1�Ĺ��̴���
void Comb1_Proc(void)
{
	//����ۼ�ʱ��
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


/*
*************************************************************
			�ļ�����	app_btn_scan.c
			��	 �ܣ�	���а���ɨ�躯���ļ�
			��	 ����    V1.0
			ʱ	 �䣺    2013��1��2��
			��	 �ߣ�    ��	��
			��	 Ȩ��    ��ɳ���Ƶ���

�޸���־��
��������������������������������������������������������������������������������������������������������
�޸�ʱ��   		�޸���		�޸�����/ԭ��
2013��1��2��	��	��		�����ļ�

��������������������������������������������������������������������������������������������������������
			
*************************************************************
*/

#include "typedef.h"

extern u8 ShortRun_flag;
extern u8 g_ucState;
u8 set_prog_flag;

u8 prog_state =0,select_prog =0,disp_page =0;

//addKey_Time[0]Ϊ�����������Ӽ���Ӧһ�ΰ�����ʱ������ʱ��
//addKey_Time[1]Ϊ��¼һ�ΰ����Ķ�ʱ��


//ȷ������ʱ��
//TIME_TYPE enterKey_Timer;

vu8 keySET_State,highSetFlag; //���ü�״̬

//�����ź�
KEY_SIGNAL keySignal2;

//��ʼ�������źŲ���
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

//���ü�ɨ��
void SETKey_Scan(void)
{
	 //���ü�
 	if( SET_KEY)
	{
		if(keySignal2.setSignal == 0)
		{
			keySignal2.setSignal = 1;

			if(highSetFlag == 0)
			{  
				SET_Proc(); 					//��ӦSET״̬�µİ���
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

//�߼������еļӼ�ɨ��
void HighADDKey_Scan(void)
{
/*	if(addKey_Time[0].signal)
	{
		addKey_Time[0].counter = 0;
		addKey_Time[0].signal = 0;
		SetTimer(&addKey_Time[0],50);
		//����ʱ����
		LockTimeQAdd();
	}
	if(getKeyState(ADD_KEY) && !getKeyState(SUB_KEY))//�Ӽ�
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

//�Ӽ�ɨ��
void ADDKey_Scan(void)
{
	static u8 add_n;
	if(ADD_KEY && !(SUB_KEY))//�Ӽ�
	{
		ADDKEY_TIMER.enable =1;
		if(add_n < 5) ADDKEY_TIMER.number =30;//���ö�ʱ������
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

//�߼����ù����м�������
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
//����ɨ��
void SUBKey_Scan(void)
{
	static u8 dec_n;
	
	if(SUB_KEY && !(ADD_KEY))
	{
		DECKEY_TIMER.enable =1;
		if(dec_n < 5) DECKEY_TIMER.number =30;//���ö�ʱ������
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
//ֹͣ��ɨ��
//�Ƿ�ֹͣ��������
u8 IfStopPress(void)
{
	return (STOP_KEY);
}

void STOPKey_Scan(void)
{
	if( STOP_KEY)
	{
		if(keySignal2.stopSignal == 0)//������
		{
			keySignal2.stopSignal = 1;
			//״̬���䣬������
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



//��ȷ������ʱʱ��
void ClearEnterTimerC(void)
{								  
///	enterKey_Timer.counter = 0;
//	enterKey_Timer.tNum = 0;
//	enterKey_Timer.signal = 0;	
}

//ȷ����ɨ��
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
				ENTERKey_Proc();  //ȷ�����������
				
			}
		}
	}
	else
	{
		keySignal2.enterSignal = 0;
	}

}

//�л���ɨ��
void CUTKey_Scan(void)
{
	if( CUT_KEY)
	{
		if(keySignal2.cutSignal == 0)
		{
			keySignal2.cutSignal = 1;
			if(	highSetFlag != 1)
			{
				CUTKey_Proc();	//�л�������/ת����ʾ
			}
			return ;
		}
	}
	else
	{
		keySignal2.cutSignal = 0;
	}
	
}
//���ż�ɨ��
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
//����ɨ��
void Key_Proc(void)
{

	SETKey_Scan();        //���ü�ɨ��
	CUTKey_Scan();
//	SPEEDKey_Scan(); 
//	TIMEKey_Scan(); 
//	TEMPKey_Scan(); 
//	ROTORKey_Scan(); 
//	ProgKEY_scan();
	if(keySET_State == 1 || highSetFlag != 0)
	{
		ADDKey_Scan();        //�Ӽ�ɨ��
		SUBKey_Scan();        //����ɨ��
		ENTERKey_Scan();      //ȷ����ɨ��	
	}
  //OpenKey_Scan();      //���ż�
	STOPKey_Scan();       //ֹͣ��ɨ��



}

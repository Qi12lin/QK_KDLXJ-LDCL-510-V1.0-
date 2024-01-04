/****************************************
*		�ļ�����temp.c
*   ��  �ܣ����¶��йصĲ���
*   
*****************************************/


#include "typedef.h"

int set5;  //�����¶�
//�¶Ȳ���
TEMP_PARAM temper; //

//�¶Ȼ�ȡʱ��
//TIME_TYPE temprTimer;

//#define TEMP_DTLEDs_FIRST DTLEDs_7
//��ʾʵʱ�¶�
extern void DispRTTemp(int _lTempValue, u8 _ucffresh);

//��˾ʹ�õ��¶ȼ�--��������Bֵ3380
//�¶ȼ������
const vu16 BIT_temp[27] = 
{
	921*4,  // < -25
	895*4,  // -25~-20
	865*4,  // -20~-15
	831*4,  //-15~-10
	793*4,  //-10~-5
	751*4,  //-5~0
	706*4,  //0~5
	658*4,  //5~10
	609*4,  //10~15
	560*4,  //15~20
	512*4,  //20~25
	465*4,  //25~30
	420*4,  //30~35
	377*4,  //35~40
	337*4,  //40~45
	301*4,  //45~50
	268*4,  //50~55
	238*4,  //55~60
	211*4,  //60~65
	187*4,  //65~70
	166*4,  //70~75
	147*4,  //75~80
	131*4,  //80~85
	112*4,  //85~90
	102*4,  //90~95
	91*4,   //95~100
	81*4    //100~105
};

//���5�ȵ����Զ��ڵ�ADC��ֵ
const vu8 BIT_temp1[27] = 
{
	0,  
	26*4,
	30*4,
	34*4,
	38*4,
	42*4,
	45*4,
	48*4,
	49*4,
	49*4,
	48*4,
	47*4,
	45*4,
	43*4,
	40*4,
	36*4,
	33*4,
	30*4,
	27*4,
	24*4,
	21*4,
	19*4,
	17*4,
	19*4,
	10*4,
	11*4,
	10*4
};

//�¶ȳ�ʼ��
void TemprInit(void)
{
	temper.fCtrlSt = 0;
	temper.fTarTempr = 0; 
}

//------------------------------------------------------
//�¶ȼ���
//��ڣ�AD����BITֵ
//���أ��¶�ֵ
//-------------------------------------------------------
int temp_count(u16 ADC_bit)
{
	u16 tempi = 0,i = 0;
	int Temperature;
	

	if(ADC_bit >=921*4) 
	{
		Temperature = -260;
		return Temperature;
	}
	else if(ADC_bit <= 81*4)
	{
		Temperature = 1100;
		return Temperature;
	}
	else 
	{
		for(i = 1 ; i< 26; i++)
		{
			if(ADC_bit >= BIT_temp[i])
			{
 				tempi =  (BIT_temp[i-1] - ADC_bit)*50/BIT_temp1[i];//�¶Ȳ�ֵ���� ������10��
				Temperature = i*50 + tempi;  //i*50��������¶�
				Temperature	= Temperature - 300;
				return Temperature;
			}
			else {}
		}
	}
	return (-260);
}

//���ƻ�ȡ�¶ȵ�ģ����
void CtrlTempRead(void)
{
//	if(!temprTimer.enable)
//	{
//		SetTimer(&temprTimer,10);
//		ADC1_StartConversion();
//	}
//	if(temprTimer.signal)
//	{
//		temprTimer.counter = 0;
//		temprTimer.signal = 0;
//		temprTimer.tNum = 0;
//		ADC1_StartConversion();
//	}
}

//�¶�ģ����ת�����¶�
void AngleToTemp(void)
{
	static u16 m_temp = 0xffff;
 u16 AD_Param;
	
	AD_Param = GetADC();

//	DispRTTemp(AD_Param);
	if(m_temp != AD_Param)
	{
		m_temp = AD_Param;	 //
		temper.realTemp = temp_count(AD_Param);
    }
}
u16 Y4_OFF_time=0,Y4_ON_time;
s16 ON_Time =3000,OFF_Time =1000;
signed int temp_ek,last_temp;
u8 M7;
u8 Y4_temp;
//�����¶ȴﵽĿ���¶ȹ���
void CtrlTempToTarget(int  _watch3,int _set5)  //������  �¶ȿ��Ʒ�Χ��������4��
{
		int close_T,open_T;
	  u16 hot_time,cool_time;
	
		close_T = HldReg[SET_CLOSE_TEMP];
		open_T = HldReg[SET_OPEN_TEMP];
	  hot_time  = HldReg[SET_hot_time];
	cool_time  = HldReg[SET_cool_time];
	  
		if (temper.fCtrlSt == 0)
		{
			if (_watch3 >= (_set5 + 2) ) Y4_ON;						
			else if (_watch3 <= (_set5 - 20) ) Y4_OFF;	//20
			else temper.fCtrlSt = 1;//��ʾ�����һ������
		}
		else{}	
		
		if(temper.fCtrlSt ==1)
		{
//			if((TEMP_DOWN ==temper.state  && (_watch3 <= (_set5 - 30) )) || _watch3 <= (_set5 - 50)) Y4_OFF;
//			else if((TEMP_UP ==temper.state  && (_watch3 >= (_set5 - 35) ))||_watch3 >= (_set5 + 10))  Y4_ON;
			
			if((TEMP_UP ==temper.state  && (_watch3 >= (_set5 -open_T) ))||_watch3 >= (_set5 + 10) || Y4_OFF_time > hot_time)  
			{
				Y4_ON;Y4_OFF_time =0;
			}
			else if((TEMP_DOWN ==temper.state  && (_watch3 <= (_set5 -close_T) ) &&Y4_ON_time >=cool_time) || _watch3 <= (_set5 - 50)) 
			{
				if(Y4) Y4_OFF_time =0;
				Y4_OFF;Y4_ON_time =0;
			}
		}
}
void CtrlTempToTarget1(int  _watch3,int _set5)  //������  �¶ȿ��Ʒ�Χ��������4��
{
		int close_T,open_T;
	
		close_T = HldReg[SET_CLOSE_TEMP];
		open_T = HldReg[SET_OPEN_TEMP];
	 
		if (temper.fCtrlSt == 0)
		{
			if (_watch3 >= (_set5 + 2) ) Y4_ON;						
			else if (_watch3 <= (_set5 - 20) ) Y4_OFF;	//20
			else temper.fCtrlSt = 1;//��ʾ�����һ������
		}
		else{}	
		
		if(temper.fCtrlSt ==1)
		{

			if((TEMP_DOWN ==temper.state  && (_watch3 <= (_set5 -close_T) )) || _watch3 <= (_set5 - 50)) Y4_OFF;
			else if((TEMP_UP ==temper.state  && (_watch3 >= (_set5 -open_T) ))||_watch3 >= (_set5 + 10))  Y4_ON;
		}
}
//�����¶ȴﵽĿ���¶ȹ���   ��ʱ����Y4_OFF_timeҪ�޸�
void CtrlTempToTarget_lxy(int  watch3,int set5) 
{
	static u8 Y4c;
	
	Y4c = temper.fCtrlSt;	
	if (Y4c == 0)
		{
			if (watch3 >= (set5 + 2) ) Y4_ON;						
			else if (watch3 <= (set5 - 22) ) Y4_OFF;	//20
			else Y4c = 1;//��ʾ�����һ������
			Y4_temp = Y4;
			M7 = 0;
			Y4c = 1;
		}
		else{}	
		
		if(Y4c ==1)
		{
			if ((TEMP_UP ==temper.state && watch3 >= (set5-5)) || Y4_OFF_time >=800/*&& watch3 >= (set5+1)*/ ) 
			{
				Y4_ON;//���뾫�ȵ���
				Y4_OFF_time =0;
			}
				else {}
				if (TEMP_DOWN ==temper.state && watch3 <= (set5-20)) 
				{
					
					if(Y4) Y4_OFF_time =0;
					Y4_OFF;
				}
				else{}	
			
		}	

}
void CtrlTempToTarget5(int  watch3,int set5)     //�������� ���һ�η�������˵� ��Ƶ��4���ȶ� �ͻ���Ϊ��ŷ�̫Ƶ����15�����ң��¿�5
{
		static u8 Y4_last,get_time;

	if (temper.fCtrlSt == 0)
		{
			if (watch3 > (set5 + 0) ) Y4_ON;						
			else if (watch3 <= (set5 - 20) ) Y4_OFF;	//20
			else temper.fCtrlSt = 1;//��ʾ�����һ������
			Y4_temp = Y4;
			M7 = 0;

			Y4_OFF_time =0;
			Y4_ON_time =0;

			last_temp = watch3;
		}
		else{}	
		
		if(temper.fCtrlSt ==1)
		{
			switch(M7)
			{
				case 0:
					
					if( watch3 > set5)
					{
						 Y4_ON;Y4_ON_time =0;
						 M7 =2;
					}
					else 
					{
						 Y4_OFF;Y4_OFF_time =0;
						 M7 =1;
					}
					break;
				case 1:
					if(Y4_OFF_time >OFF_Time)
					{
						Y4_ON;Y4_ON_time =0;
						 M7 =2;
					}
					break;
				case 2:
					if(Y4_ON_time >ON_Time)
					{
						 Y4_OFF;Y4_OFF_time =0;
						 M7 =1;
					}
					break;
				case 10:
					Y4_ON;	
					if(TEMP_DOWN ==temper.state)  M7 =0;
				
				default:
					temper.fCtrlSt =0;
					break;
			}
			if(Y4_last != Y4 && Y4 ==0)
			{
				/*if(watch3_MAX > set5+10)
				{
					OFF_Time-=100;
					ON_Time +=500;
				}
				else if(watch3_MAX < set5-10)
				{
					OFF_Time+=100;
				}
				if(watch3_MIN > set5-5)
				{
					ON_Time+=500;
				}
				else if(watch3_MIN < set5-10)
				{
					ON_Time-=500;
				}
				*/
				if(last_temp > watch3)
				{
					temp_ek = last_temp -watch3;
					if(temp_ek > 5) 
					{
						ON_Time-= temp_ek *1000;
						OFF_Time +=temp_ek *80;//200
					}
					else if(temp_ek > 3) 
					{
						ON_Time-= temp_ek *1000;
						OFF_Time +=temp_ek *45;
					}
					else if(temp_ek > 0) 
					{
						ON_Time -= temp_ek *500;
						OFF_Time +=temp_ek *25;
					}
				}
				else
				{
					temp_ek = watch3 -last_temp;
					{
						
						if(temp_ek >3) 
						{
							ON_Time+= temp_ek *1000;
							OFF_Time -=temp_ek *200;
						}
						else if(temp_ek >0) 
						{
							ON_Time+= temp_ek *500;
							OFF_Time -=temp_ek *100;
						}
					}
				}
				

				if(watch3 > set5 -15 && last_temp < watch3)
				{
					ON_Time +=3000;
					OFF_Time -= 500;
				}
				else if(watch3 < set5 -15 && last_temp > watch3)  //�¶�̫����������
				{
					ON_Time -=1000;
					OFF_Time += 1000;
				}
				else if(watch3 < set5 -10 && last_temp > watch3)  //�¶�̫����������
				{
					ON_Time -=500;
					OFF_Time += 200;
				}
				last_temp = watch3;	
			}
			Y4_last = Y4;
			if(watch3 > set5+10  && M7!=10)
			{
				M7 =10;ON_Time+=1000;
				OFF_Time -=500;
			}
			if(ON_Time <5000) ON_Time =5000;
			else if(ON_Time >15000) ON_Time =15000;
			
			if(OFF_Time <1000 || OFF_Time >10000) OFF_Time =1000;
			else if(OFF_Time >6000) OFF_Time =6000;
		}	
	
}




void CtrlTempToTarget0( s16 _setTemprature)
{
	static vu8 OldY4 = 0;//��ŷ�֮ǰ״̬
	static int AccMax = 0;     //���ȵ���ʱ�����ֵ
	static int AccMin = 0x7fff;     //���ȵ���ʱ����Сֵ
	int ek_off ,ek_on;
	static int ek1,ek2;
	
	if(temper.realTemp <= _setTemprature + 2 && 
			temper.realTemp >= _setTemprature - 2)
	{
		temper.fTarTempr = 1;
	}
	
		//������һ����Χ��
	if(temper.fCtrlSt != 1)
	{
		if(temper.realTemp >= _setTemprature+20)
		{
			makeCold();
		}
		else if(temper.realTemp <= _setTemprature-20)
		{
			makeHot();
		}
		else temper.fCtrlSt = 1; 
		OldY4 = Y4;
		temper.CtrlCycle = TEMP_CYCLE0;
  }
	
	//���ȿ���
	if(temper.fCtrlSt)
	{
		if(OldY4 != Y4) //����һ����Ϊһ����������
		{
			if(temper.CtrlCycle < TEMP_CYCLE_ALL)
			{
				temper.CtrlCycle++;
			}
			if(temper.CtrlCycle == TEMP_CYCLE3)
			{
				AccMax = _setTemprature; //Ϊ���ĸ�������׼��
				AccMin = _setTemprature;
			}
			else if(temper.CtrlCycle >= TEMP_CYCLE4)
			{ 
				//��ʼ��ֵ
				if(temper.CtrlCycle == TEMP_CYCLE4)
				{
					ek_off = _setTemprature - AccMin;
					ek_on = AccMax - _setTemprature;
					if(ek_off - ek_on >= 10)
					{
						ek1 = ek_off - 10;
						ek2 = ek1;
					}
					else if(ek_off - ek_on <= -10)
					{
						ek1 = 10-ek_on;
						ek2 = ek1;
					}
				}
				//��һ�ε�����ֵ
				if(Y4 != 1)
				{
					if( AccMax >= (_setTemprature + 8) ) ek1= ek1- 2;
					else if((AccMax <= (_setTemprature + 2)) )ek1+=2;
					else if(AccMax <= (_setTemprature + 5) ) ek1++;
					else{}
					AccMax = _setTemprature;//����ʼֵ
				}
				else
				{
					if( (AccMin <= (_setTemprature - 8)) ) ek2+=2;
					else if(AccMin >= (_setTemprature - 2)) ek2 -= 2; 
					else if(AccMin >= (_setTemprature - 5)) ek2--;
					else{}
					AccMin = _setTemprature;//�����һ���ڵ���Сֵ
				}
			}
		}
		else
		{
			if(temper.realTemp >= AccMax) AccMax = temper.realTemp;
			if(temper.realTemp <= AccMin) AccMin = temper.realTemp;
		}
		OldY4 = Y4;
		switch(temper.CtrlCycle)
		{
			case TEMP_CYCLE0: //���ٻ��壬��ֹ���Թ���
				if(temper.state == TEMP_UP &&
						temper.realTemp >= _setTemprature - 15
				  )
				{
					 //����
					 makeCold();
				}
				if(temper.state == TEMP_DOWN &&
					temper.realTemp <= _setTemprature + 15
					)
				{
					//����
					makeHot();	
				}
			break;
			case TEMP_CYCLE1: //������������Ͻ����������������
				if(Y4 == 1)
				{
					if(temper.state != TEMP_UP)
					{
						if(temper.realTemp < _setTemprature) 
						{
							makeHot();
						}
						else temper.CtrlCycle++;
					}
				}
				else
				{
					if(temper.state != TEMP_DOWN)
					{
						if(temper.realTemp > _setTemprature) 
						{
							makeCold();
						}
						else temper.CtrlCycle++;
					}
				}
			break;
		  case TEMP_CYCLE2:
			case TEMP_CYCLE3:
			case TEMP_CYCLE4: //
				if(temper.state == TEMP_UP &&
						temper.realTemp >= _setTemprature
					)
				{
					makeCold();
				}
				if(temper.state == TEMP_DOWN &&
					temper.realTemp <= _setTemprature
					)
				{
					makeHot();
				}
			break;
			default: //���յ���
				if((temper.state == TEMP_UP &&
						temper.realTemp >= _setTemprature + ek1
					))  //����3��ǿ������   ��ֹ�¶Ȼ����仯û�иı�״̬��ʱ��
				{
					makeCold();
				}
				if((temper.state == TEMP_DOWN &&
					temper.realTemp <= _setTemprature + ek2
					))
				{
					makeHot();
				}
				if(ek1 > 10) ek1 = 10;
				else if(ek1 < - 10 ) ek1 = - 10;
				else{}
				if(ek2>10) ek2 = 10;
				else if(ek2 < -10 ) ek2 = -10;
				else{}	
			break;
		}
	}
	else
	{
	}
	if(temper.realTemp >= _setTemprature+30)  //����3��ǿ������   ��ֹ�¶Ȼ����仯û�иı�״̬��ʱ�򲻸ı�״̬
	{
		makeCold();
	}
	if(temper.realTemp <= _setTemprature-30)
	{
		makeHot();
	}
}


//��ʾʵ���¶�
void DispTemp_Proc(void)
{
	static int m_tmpVa = 0;
	static int m_Watch[4];
	static int OldTemp1;
	static int NewTemp;
	static vu8 cntWatch = 0;
	
	//1s����һ��ʱ���¶�

	if(UPDATATEMP_1MS ==0 )
	{
		UPDATATEMP_1MS =800;

		m_tmpVa = temper.realTemp;
		
		m_Watch[cntWatch++] = m_tmpVa;
		if(cntWatch < 3)
		{
			cntWatch++;
		}
		else
		{
			cntWatch = 0;
			NewTemp = m_Watch[0]+m_Watch[1]+m_Watch[2];
			if(NewTemp >= OldTemp1 + 2)
			{
				temper.state = TEMP_UP;
			}
			else if(NewTemp <= OldTemp1 - 2)
			{
				temper.state = TEMP_DOWN;
			}
			OldTemp1 = NewTemp;
		}
	}

//	//��ʾʵʱ�¶�
//	if(temper.fTarTempr != 0)
//	{
//		//�����趨�¶Ⱥ���ʾ�趨�¶�
//		runParam.temprature = set5; 
//	}
//	else
//	{
//		runParam.temprature = m_tmpVa; 
//	}
	
	HldReg[TEMPDISP] =2;
	
	/*if(temper.fTarTempr)
	{
		if(HldReg[TEMPDISP] ==0)
		{
			runParam.temprature = (s16)HldReg[SET_TEMPERATURE]*10;  //�����¶�
		}
		else if(HldReg[TEMPDISP]  ==1)
		{
			runParam.temprature = m_tmpVa;  //ʵ���¶�
		}
		else if(HldReg[TEMPDISP]  >1)  //���ݱ�����ʾ С��ֵ
		{
			runParam.temprature =(s16)HldReg[SET_TEMPERATURE]*10+ ((m_tmpVa - (s16)HldReg[SET_TEMPERATURE]*10)/(HldReg[TEMPDISP] ));
			if(runParam.temprature < HldReg[SET_TEMPERATURE]*10+20 && runParam.temprature > HldReg[SET_TEMPERATURE]*10-20)
					runParam.temprature = HldReg[SET_TEMPERATURE]*10;
		}
//		if(m_tmpVa > (s16)HldReg[SET_TEMPERATURE]*10 +40 && m_tmpVa < (s16)HldReg[SET_TEMPERATURE]*10 -40)  runParam.temprature = (s16)HldReg[SET_TEMPERATURE]*10;  //�����¶�
//		else runParam.temprature = m_tmpVa;  //ʵ���¶�	   �¶���ʾ����4��
		
	}	
	else  runParam.temprature = m_tmpVa;  //ʵ���¶�	
	*/
	HldReg[TEMPDISP]  =3;
	if(temper.fTarTempr)
	{
		if(HldReg[TEMPDISP] ==0)
		{
			runParam.temprature = (s16)HldReg[SET_TEMPERATURE]*10;  //�����¶�
		}
		else if(HldReg[TEMPDISP]  ==1)
		{
			runParam.temprature = m_tmpVa;  //ʵ���¶�
		}
		else if(HldReg[TEMPDISP]  >1)  //���ݱ�����ʾ С��ֵ
		{
			runParam.temprature =(s16)HldReg[SET_TEMPERATURE]*10+ ((m_tmpVa - (s16)HldReg[SET_TEMPERATURE]*10)/(HldReg[TEMPDISP] ));
		}
	}	
	else  runParam.temprature = m_tmpVa;  //ʵ���¶�	
	
}

//����ʱ��

//���±���
void OverTemprAlarm(void)
{
	s16 overTemp;
	overTemp = set5 + 60;
	if(overTemp <400)  overTemp =400;
	//���±���
	if(temper.realTemp >= 800 || temper.realTemp <= -250)
	{
		SetAlarmFlag(OVERHEAT_ALARM);
	}
	
	if(	temper.fTarTempr == 1  &&
			runFlag.curRunFlag == 1 &&
			temper.realTemp >= overTemp)
	{
		
	}
	else 
	{

			OVERTEMP_1MS =15000;

	}
	
	if(OVERTEMP_1MS == 0)
	{
		SetAlarmFlag(OVERHEAT_ALARM);
	}
}

//�¶ȿ��ƹ���
void Temp_Proc(void)
{
	if(keySET_State ==0)  set5 = (s16)HldReg[SET_TEMPERATURE] *10;   //�����¶� ���޸Ĺ����в�����
	AngleToTemp();//�¶�ģ����ת�����¶�

	if(temper.realTemp <= set5 + 2  && 
			temper.realTemp >= set5 - 2)
	{
		temper.fTarTempr = 1;
	}
	
//	CtrlTempToTarget(temper.realTemp,set5);//�����¶ȴﵽĿ���¶ȹ���
CtrlTempToTarget0(set5);   //����ԭ�������¿ط�ʽ  2020.10.22
	//temperature(temper.realTemp, HldReg[SET_TEMPERATURE]*10);

	//���±���
//	OverTemprAlarm();

	//��ʾʵ���¶�
	DispTemp_Proc(); 
}




/****************************************
*		文件名：temp.c
*   功  能：与温度有关的操作
*   
*****************************************/


#include "typedef.h"

int set5;  //控制温度
//温度参数
TEMP_PARAM temper; //

//温度获取时间
//TIME_TYPE temprTimer;

//#define TEMP_DTLEDs_FIRST DTLEDs_7
//显示实时温度
extern void DispRTTemp(int _lTempValue, u8 _ucffresh);

//公司使用的温度计--热敏电阻B值3380
//温度计算参数
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

//相差5度的线性段内的ADC差值
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

//温度初始化
void TemprInit(void)
{
	temper.fCtrlSt = 0;
	temper.fTarTempr = 0; 
}

//------------------------------------------------------
//温度计算
//入口：AD采样BIT值
//返回：温度值
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
 				tempi =  (BIT_temp[i-1] - ADC_bit)*50/BIT_temp1[i];//温度差值计算 扩大了10倍
				Temperature = i*50 + tempi;  //i*50线性起点温度
				Temperature	= Temperature - 300;
				return Temperature;
			}
			else {}
		}
	}
	return (-260);
}

//控制获取温度的模拟量
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

//温度模拟量转换成温度
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
//控制温度达到目标温度过程
void CtrlTempToTarget(int  _watch3,int _set5)  //阀动少  温度控制范围超过正负4度
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
			else temper.fCtrlSt = 1;//表示进入第一个周期
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
void CtrlTempToTarget1(int  _watch3,int _set5)  //阀动少  温度控制范围超过正负4度
{
		int close_T,open_T;
	
		close_T = HldReg[SET_CLOSE_TEMP];
		open_T = HldReg[SET_OPEN_TEMP];
	 
		if (temper.fCtrlSt == 0)
		{
			if (_watch3 >= (_set5 + 2) ) Y4_ON;						
			else if (_watch3 <= (_set5 - 20) ) Y4_OFF;	//20
			else temper.fCtrlSt = 1;//表示进入第一个周期
		}
		else{}	
		
		if(temper.fCtrlSt ==1)
		{

			if((TEMP_DOWN ==temper.state  && (_watch3 <= (_set5 -close_T) )) || _watch3 <= (_set5 - 50)) Y4_OFF;
			else if((TEMP_UP ==temper.state  && (_watch3 >= (_set5 -open_T) ))||_watch3 >= (_set5 + 10))  Y4_ON;
		}
}
//控制温度达到目标温度过程   定时器中Y4_OFF_time要修改
void CtrlTempToTarget_lxy(int  watch3,int set5) 
{
	static u8 Y4c;
	
	Y4c = temper.fCtrlSt;	
	if (Y4c == 0)
		{
			if (watch3 >= (set5 + 2) ) Y4_ON;						
			else if (watch3 <= (set5 - 22) ) Y4_OFF;	//20
			else Y4c = 1;//表示进入第一个周期
			Y4_temp = Y4;
			M7 = 0;
			Y4c = 1;
		}
		else{}	
		
		if(Y4c ==1)
		{
			if ((TEMP_UP ==temper.state && watch3 >= (set5-5)) || Y4_OFF_time >=800/*&& watch3 >= (set5+1)*/ ) 
			{
				Y4_ON;//进入精度调节
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
void CtrlTempToTarget5(int  watch3,int set5)     //继续测试 最后一次发给美瑞克的 视频看4度稳定 客户认为电磁阀太频繁（15秒左右）温控5
{
		static u8 Y4_last,get_time;

	if (temper.fCtrlSt == 0)
		{
			if (watch3 > (set5 + 0) ) Y4_ON;						
			else if (watch3 <= (set5 - 20) ) Y4_OFF;	//20
			else temper.fCtrlSt = 1;//表示进入第一个周期
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
				else if(watch3 < set5 -15 && last_temp > watch3)  //温度太低升不上来
				{
					ON_Time -=1000;
					OFF_Time += 1000;
				}
				else if(watch3 < set5 -10 && last_temp > watch3)  //温度太低升不上来
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
	static vu8 OldY4 = 0;//电磁阀之前状态
	static int AccMax = 0;     //精度调节时的最大值
	static int AccMin = 0x7fff;     //精度调节时的最小值
	int ek_off ,ek_on;
	static int ek1,ek2;
	
	if(temper.realTemp <= _setTemprature + 2 && 
			temper.realTemp >= _setTemprature - 2)
	{
		temper.fTarTempr = 1;
	}
	
		//控制在一个范围内
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
	
	//精度控制
	if(temper.fCtrlSt)
	{
		if(OldY4 != Y4) //阀动一次作为一个控制周期
		{
			if(temper.CtrlCycle < TEMP_CYCLE_ALL)
			{
				temper.CtrlCycle++;
			}
			if(temper.CtrlCycle == TEMP_CYCLE3)
			{
				AccMax = _setTemprature; //为第四个周期做准备
				AccMin = _setTemprature;
			}
			else if(temper.CtrlCycle >= TEMP_CYCLE4)
			{ 
				//初始差值
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
				//第一次调整差值
				if(Y4 != 1)
				{
					if( AccMax >= (_setTemprature + 8) ) ek1= ek1- 2;
					else if((AccMax <= (_setTemprature + 2)) )ek1+=2;
					else if(AccMax <= (_setTemprature + 5) ) ek1++;
					else{}
					AccMax = _setTemprature;//给初始值
				}
				else
				{
					if( (AccMin <= (_setTemprature - 8)) ) ek2+=2;
					else if(AccMin >= (_setTemprature - 2)) ek2 -= 2; 
					else if(AccMin >= (_setTemprature - 5)) ek2--;
					else{}
					AccMin = _setTemprature;//清除上一周期的最小值
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
			case TEMP_CYCLE0: //减少缓冲，防止惯性过大
				if(temper.state == TEMP_UP &&
						temper.realTemp >= _setTemprature - 15
				  )
				{
					 //制冷
					 makeCold();
				}
				if(temper.state == TEMP_DOWN &&
					temper.realTemp <= _setTemprature + 15
					)
				{
					//制热
					makeHot();	
				}
			break;
			case TEMP_CYCLE1: //缓冲过来后，马上进入第三个阀动周期
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
			default: //最终调节
				if((temper.state == TEMP_UP &&
						temper.realTemp >= _setTemprature + ek1
					))  //大于3度强制制冷   防止温度缓慢变化没有改变状态的时候
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
	if(temper.realTemp >= _setTemprature+30)  //大于3度强制制冷   防止温度缓慢变化没有改变状态的时候不改变状态
	{
		makeCold();
	}
	if(temper.realTemp <= _setTemprature-30)
	{
		makeHot();
	}
}


//显示实际温度
void DispTemp_Proc(void)
{
	static int m_tmpVa = 0;
	static int m_Watch[4];
	static int OldTemp1;
	static int NewTemp;
	static vu8 cntWatch = 0;
	
	//1s更新一次时间温度

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

//	//显示实时温度
//	if(temper.fTarTempr != 0)
//	{
//		//到底设定温度后显示设定温度
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
			runParam.temprature = (s16)HldReg[SET_TEMPERATURE]*10;  //设置温度
		}
		else if(HldReg[TEMPDISP]  ==1)
		{
			runParam.temprature = m_tmpVa;  //实际温度
		}
		else if(HldReg[TEMPDISP]  >1)  //根据倍率显示 小于值
		{
			runParam.temprature =(s16)HldReg[SET_TEMPERATURE]*10+ ((m_tmpVa - (s16)HldReg[SET_TEMPERATURE]*10)/(HldReg[TEMPDISP] ));
			if(runParam.temprature < HldReg[SET_TEMPERATURE]*10+20 && runParam.temprature > HldReg[SET_TEMPERATURE]*10-20)
					runParam.temprature = HldReg[SET_TEMPERATURE]*10;
		}
//		if(m_tmpVa > (s16)HldReg[SET_TEMPERATURE]*10 +40 && m_tmpVa < (s16)HldReg[SET_TEMPERATURE]*10 -40)  runParam.temprature = (s16)HldReg[SET_TEMPERATURE]*10;  //设置温度
//		else runParam.temprature = m_tmpVa;  //实际温度	   温度显示屏蔽4度
		
	}	
	else  runParam.temprature = m_tmpVa;  //实际温度	
	*/
	HldReg[TEMPDISP]  =3;
	if(temper.fTarTempr)
	{
		if(HldReg[TEMPDISP] ==0)
		{
			runParam.temprature = (s16)HldReg[SET_TEMPERATURE]*10;  //设置温度
		}
		else if(HldReg[TEMPDISP]  ==1)
		{
			runParam.temprature = m_tmpVa;  //实际温度
		}
		else if(HldReg[TEMPDISP]  >1)  //根据倍率显示 小于值
		{
			runParam.temprature =(s16)HldReg[SET_TEMPERATURE]*10+ ((m_tmpVa - (s16)HldReg[SET_TEMPERATURE]*10)/(HldReg[TEMPDISP] ));
		}
	}	
	else  runParam.temprature = m_tmpVa;  //实际温度	
	
}

//超温时间

//超温报警
void OverTemprAlarm(void)
{
	s16 overTemp;
	overTemp = set5 + 60;
	if(overTemp <400)  overTemp =400;
	//超温报警
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

//温度控制过程
void Temp_Proc(void)
{
	if(keySET_State ==0)  set5 = (s16)HldReg[SET_TEMPERATURE] *10;   //控制温度 在修改过程中不更新
	AngleToTemp();//温度模拟量转换成温度

	if(temper.realTemp <= set5 + 2  && 
			temper.realTemp >= set5 - 2)
	{
		temper.fTarTempr = 1;
	}
	
//	CtrlTempToTarget(temper.realTemp,set5);//控制温度达到目标温度过程
CtrlTempToTarget0(set5);   //测试原来的老温控方式  2020.10.22
	//temperature(temper.realTemp, HldReg[SET_TEMPERATURE]*10);

	//超温报警
//	OverTemprAlarm();

	//显示实际温度
	DispTemp_Proc(); 
}




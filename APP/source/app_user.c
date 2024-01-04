#include "typedef.h"


//ת��
u16 Speed_max[20] =
{
	0,
	16000,  //1��ת�Ӽ���ת��
	16000,  //2��
	14000,  //3��
	14000,  //4��
	12000,  //5��
	6000    //6��
};

//������
u16 Force_max[20]=
{
	0,
	14882, //1��ת�Ӽ���������
	18603, //2��
	14243, //3��
	14681, //4��
	14006, //5��
	3200   //6��
};

//ת�������Ŀ
u8 RotorMaxNum = 6;

//	vu8  ucAcc_R[MAX_ROTOR];
// 	vu8  ucDec_R[MAX_ROTOR];
ROTORPROGRAMDATA RotorPrgData;

PROGRAMDATA  OneProgramData;   //һ���������
PROGRAMDATA  progData[10];    //һ�ζ�ȡ10������


//��ȡ���ת��
u16 GetMaxSpeed(u8 rotor)
{
	if(rotor > ROTOR_MAX) return 0;
	return Speed_max[rotor];
}

//----------------------------------------------------------
// ��ȡת�ټ����������ֵ fSpeed 0 ת��| 1 ������
//----------------------------------------------------------
u16 Fc_SpeedMax (u8 rotorNo, u8 fSpeed)
{
	if(!fSpeed) //ת��
	{
		return (Speed_max[rotorNo]);
	}
	else //������
	{
		return (Force_max[rotorNo]);
	}
}

//--------------------------------------------------------------
// ����ת�Ӻ�ʱ�Զ���ʾ���ֵ
//--------------------------------------------------------------
void SetRotor (void)
{
//	lxj_temp.LXForce = Force_max[lxj_temp.RotorNo];
//	lxj_temp.RotorSpeed = Speed_max[lxj_temp.RotorNo];
}

//--------------------------------------------------------------
// ţ�ٵ�������ƽ����
// ��ڲ�������������data���²⿪�Ž����guessResult
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

//������ת��Ϊ�ٶ�
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
		tempx = Newton_sqrt(tempx,1000); //��������
	}
	return(tempx);
}

//�ٶ�ת��Ϊ������
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
*	�� �� ��: Maxmin_Param
*	����˵��: ���������Сֵ�޶�
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/ 
//_type :0 ��ʼ��  1���޸Ĳ���


void Maxmin_Param(u8 _type)
{
	uint8_t i;         //���� ת�Ӻ� ת�� ������ ʱ��H ʱ��M  ʱ��S �¶� ���� ����             ��Ƶ ���� ����ʱ�� �ͺ�   ���� ʶ�� ��ƽ�� �ű� ���� ���� ��� ��ѹ ����ʱ�� �Զ����� ����H ����L �¶�ϵ��  ����ҳ��
	uint16_t HldRegMin[40]={1,    1, 200,  10,   0,    0,    0,  -20,  1,  1, 0,1,1,1,1,0,  50,   8,    0,     1,     1,  1,   1,    1,  1,    1,  1,  1,   100,     1       ,0     ,0   ,0,1};
	uint16_t HldRegMax[40]={100,  0,   0,   0,  99,   99,   59,   40,  10, 10,0,90,90,60000,60000,0, 400,  35,65535,VER_MAX_NUM,5,2,   2,    3,  2,    3,   3,  2,   999,    2       ,65535,65535,9,2};
	uint16_t HldRegInit[40]={1,   2,   0,   0,   0,   10,    0,    4,   4,  4,0,10,40,1,90,0,  90,  22,     0,   1,INIT_LOCK_TYPE,INIT_AUTO_ROTOR,INIT_BPH_SET,INTI_LOCK_PROTECT,INIT_LANGUAGE,INIT_HALL_EN,INTI_MOTOR_TYPE,INIT_VOLTAGE,INIT_DOOROPEN_TIME,INIT_AUTO_ROTOR,0,0,1,
													INIT_STARTPAGE};
		//                                                               �����¶� �����¶� ����ʱ�� ����ʱ��           	                                                   
	static u16 set_speed_last;
  static s16 set_temprature_last;		
		
	//�����ͺŶ�ȡת����Ϣ	
	if(HldReg[SYS_TYPE] <1 || HldReg[SYS_TYPE] >= HldRegMax[SYS_TYPE])  HldReg[SYS_TYPE]  = HldRegInit[SYS_TYPE];
		get_rotor_info(HldReg[SYS_TYPE],Speed_max,Force_max,&RotorMaxNum);
	HldRegMax[ROTOR]	=RotorMaxNum;  //���ת�Ӻ�
		
	if(HldReg[ROTOR] <1 || HldReg[ROTOR] > HldRegMax[ROTOR])  HldReg[ROTOR]  = HldRegInit[ROTOR];	
	HldRegMax[2]	=Speed_max[HldReg[ROTOR]];  //���ת��
	HldRegMax[3]	=Force_max[HldReg[ROTOR]];  //���������
	HldRegInit[2]	=Speed_max[HldReg[ROTOR]];  //���ת��
	HldRegInit[3]	=Force_max[HldReg[ROTOR]];  //���������	
		
	if(INIT_SYS_TYPE >  LOW_TYPE)  //Ĭ��Ϊ���ٵ���Ļ�Ƶ������ʼֵ
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
				if(i == SET_TEMPERATURE)   //�¶� �������Ͳ�һ��
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
				if(i == SET_TEMPERATURE)   //�¶� �������Ͳ�һ��
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
		if(g_ucfDispXg)  HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//��ʾ������
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
//EEPROM���ݱ����ַ����
#define FRIST_SAVE_RUN_DATA_ADD  0   //�������ݱ����ַ  0~99�����������ݱ���

 

#define FRIST_ROTORPROGDATA_ADD 100       //ÿ��ת�Ӷ�Ӧ�����������ٵ�λ ת�� �¶� ʱ��Сʱ\��\�� Ԥ��30��ת��

#define ALLRUNTIME_ADD          460      //�ۼ�����ʱ��洢��ַ
//����ʱ�䱣��
#define FRIST_PROGRAM_ADD  1000        //�����鱣�� ��СԼ15 *100�� Ԥ��ռ�

#define SAVEFLAG_ADD      500      //�����ж��Ƿ�����  д�����ݺ��־��Ϊ��ֵ

#define SAVEFLAG_DATA     0xAA
//��ȡ�����ٵ�λ  ��ʼÿ��ת�ӺŶ�Ӧ�������ٵ�λ
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
//���������ٵ�λ  ÿ��ת�ӺŶ�Ӧ�������ٵ�λ
void write_eepromRotorProgranData(void)
{
	u8 ucI;

	ee_WriteBytes((uint8_t *)&RotorPrgData,   FRIST_ROTORPROGDATA_ADD ,sizeof(RotorPrgData) );

}
//��ȡ�ۼ�����ʱ��
void read_eepromAllRunTime(void)
{
	ee_ReadBytes((uint8_t *)&AllRunTime,   ALLRUNTIME_ADD ,sizeof(AllRunTime) );
}

//���������ٵ�λ  ÿ��ת�ӺŶ�Ӧ�������ٵ�λ
void write_eepromAllRunTime(void)
{
	u8 ucI;

	ee_WriteBytes((uint8_t *)&AllRunTime,   ALLRUNTIME_ADD ,sizeof(AllRunTime) );

}

//ȡת�Ӷ�Ӧ������������Ϣ
void get_RotorProgranData( void)
{
//	HldReg[SET_SPEED] = RotorPrgData.uiSpeed[HldReg[ROTOR]];
//	HldReg[SET_RCF] = RotorPrgData.uiRCF[HldReg[ROTOR]];
//	HldReg[SET_TIME_HH] = RotorPrgData.ucTime_HH[HldReg[ROTOR]];
//	HldReg[SET_TIME_MM] = RotorPrgData.uiTime_MM[HldReg[ROTOR]];
//	HldReg[SET_TIME_SS] = RotorPrgData.ucTime_SS[HldReg[ROTOR]];
//	HldReg[SET_TEMPERATURE] = RotorPrgData.iTemprature[HldReg[ROTOR]];
	
	HldReg[SET_SPEED] = Speed_max[HldReg[ROTOR]];   //ȡ���ת�ٺ�������
	HldReg[SET_RCF] = Force_max[HldReg[ROTOR]];
	
	HldReg[ACC] = RotorPrgData.ucAcc[HldReg[ROTOR]];
	HldReg[DEC] = RotorPrgData.ucDec[HldReg[ROTOR]];
	
	if(g_ucfDispXg)  HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//��ʾ������
	else HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
}
//����ת�Ӷ�Ӧ������������Ϣ
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

//��ȡһ������
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
//д��һ������
void write_eepromOneProgData(u8 _ProgNO)
{
	u8 progDataLen;
	progDataLen =sizeof(OneProgramData);
	ee_WriteBytes((uint8_t *)&OneProgramData, FRIST_PROGRAM_ADD+progDataLen*_ProgNO ,progDataLen );//(MAX_SET_NUMBER - START_SET_NUMBER+1)
}
//ȡ���������ݵ���������
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
	
	if(g_ucfDispXg)  HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//��ʾ������
	else HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
}
//���ó���������   ȡ�������ݵ�����������
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
	for(i = 0; i < MAX_SET_NUMBER; i++) HldReg[i] = (read_rom[(i << 1)] << 8) + read_rom[(i << 1) + 1];//��ȡ���в���
	Maxmin_Param(0);  //���Ʋ��������ֵ����Сֵ
//	for(i = 0; i < 2000; i++);
//	ee_ReadBytes(read_rom, 1600, 800);//
//	for(i = 0; i < 400; i++) AlarmRecord[i] = (read_rom[(i << 1)] << 8) + read_rom[(i << 1) + 1];//��ȡ������¼����	
	read_eepromRotorProgranData();
	
	read_eepromAllRunTime();
	
	Balance_rede();//��ȡ��ƽ����ֵ

}
void Save_eepromData(void)
{
		u16 i;
	static u8 save_step=0;//,TemperatureSample=0;
	uint8_t write_rom[200];
//	HldReg[SYS_TYPE] =  1;
	HldReg[MOTOR_TYPE] =  QK_VFD;
	
 
	if(writeDataFlag ==1 )    //д���ݱ�־
	{
		writeDataFlag =0;
		setData_flag =100;
	for (i = 0; i < (MAX_SET_NUMBER ); i ++) {write_rom[(i << 1)] = HldReg[i+START_SET_NUMBER] >> 8;write_rom[(i << 1) + 1 ] = HldReg[i+START_SET_NUMBER];}
				ee_WriteBytes(write_rom, FRIST_SAVE_RUN_DATA_ADD,MAX_SET_NUMBER*2);//�������ݣ��ܿ���ʷ���ݼ�¼����ʷ��¼��ѯ����ʷ��������
				write_eepromRotorProgranData();
		if(HldReg[PROGRAM] <= MAX_PROGRAM ) write_eepromOneProgData(HldReg[PROGRAM]);   //������������� ֻ����20������
	}
//		switch (save_step)//д400�����ݵ�ʱ��Ϊ19ms
//		{
//			case 0:	
//				for (i = 0; i < (MAX_SET_NUMBER ); i ++) {write_rom[(i << 1)] = HldReg[i+START_SET_NUMBER] >> 8;write_rom[(i << 1) + 1 ] = HldReg[i+START_SET_NUMBER];}
//					ee_WriteBytes(write_rom, FRIST_SAVE_RUN_DATA_ADD,MAX_SET_NUMBER*2);//�������ݣ��ܿ���ʷ���ݼ�¼����ʷ��¼��ѯ����ʷ��������
//				break;
//			case 1:	
////				write_eepromOneProgData(HldReg[PROGRAM]);   //�������������
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




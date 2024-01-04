/*
**************************************************************
		�ļ�����	app_comm.c
		��	 �ܣ�	ʵ�����Ƶ����ͨ�ŵĹ��ܺ���������ͨ��
					Э�顣


**************************************************************
*/

#include "typedef.h"
//#include "MODBUS_RTU.H"
//#include "UART_VGUS.H"
MODBUS_MODULE ModbusModule0;
MODBUS_MODULE ModbusModule1;
VGUS_MODULE VgusModule0;

u8 CommSetFlag;  //ͨѶ���ñ�־  ͨѶ��������ʱ���ϴ���������
INT16U	HldReg[HLDREG_MAX_NUM];// __attribute__((at(0X2000E000)));// __attribute__ ((section ("ER_RSV")));
INT16U	SlaveHldReg[SLAVE_HLDREG_MAX_NUM];//��վ

RUNDATA runData;
CURVEDATA CurveData;
//��ʼ��ͨ������
void InitComm0Data(void)
{
//	//��ƽ̨		TR100DΪ��վ��վ�ſ���
	ModbusModule0.ID = 1;//�ӻ�վ��;
	ModbusModule0.p_uart=&UART_6;
	ModbusModule0.p_uart->Status= SEND_END;//IDLE_WAIT;
	ModbusModule0.FunCode=READ_HLD_REG;
	ModbusModule0.RegAddr=0;
	ModbusModule0.RegNum=0;
	ModbusModule0.ReTryTimeMax=5;
	ModbusModule0.OverTimeMax=200;	
//	//������չ������		TR100DΪ��վ
//	ModbusModule0.ID=1;
//	ModbusModule0.p_uart=&UART_6;
//	ModbusModule0.p_uart->Status=IDLE_WAIT;
//	ModbusModule0.FunCode=READ_HLD_REG;
//	ModbusModule0.RegAddr=0;
//	ModbusModule0.RegNum=0;
//	ModbusModule0.ReTryTimeMax=5;
//	ModbusModule0.OverTimeMax=20;	
	//��չģ��		TR100DΪ��վ
//	ModbusModule1.ID=1;//
//	ModbusModule1.p_uart=&UART_4;
//	ModbusModule1.p_uart->Status=IDLE_WAIT;
//	ModbusModule1.FunCode=READ_HLD_REG;
//	ModbusModule1.RegAddr=0;
//	ModbusModule1.RegNum=0;
//	ModbusModule1.ReTryTimeMax=5;
//	ModbusModule1.OverTimeMax=40;		
	//������
	VgusModule0.ID_HI=0xA5;
	VgusModule0.ID_LO=0x5A;
	VgusModule0.p_uart=&UART_3;
	VgusModule0.p_uart->Status=IDLE_WAIT;
	VgusModule0.p_uart->RxStatus=IDLE_WAIT;
	VgusModule0.p_uart->RxPoint=0;
	VgusModule0.FunCode=WRITE_VAR;
	VgusModule0.RegAddr=0;
	VgusModule0.RegNum=0;
	VgusModule0.ReTryTimeMax=5;
	VgusModule0.OverTimeMax=10;//5;//������������ʱ��		
    VgusModule0.SendVarFlag = 0;		
}

//--------------------------------------------------------------------
//��չ������
//��չģ����ֱ������վ����
//��չ����������վֻ�������Ϣ��ȥ�����ɴ�վ���ѿ�����
//--------------------------------------------------------------------
//void EXTEND_COMController(void)
//{
//	static uint8_t com_step=0;
//	uint16_t temp;
//		
//	if(ModbusModule0.p_uart->Status==SEND_EXCHANGE)
//	{
//		if(com_step < 4) com_step++;//
//		else com_step=0;
//		ModbusModule0.p_uart->Status=IDLE_WAIT;
//	}

//	if(ModbusModule1.p_uart->Status==IDLE_WAIT)
//	{
//		switch (com_step)
//		{
//   			case 0:
//				ModbusModule0.ID=1;
//				ModbusModule0.FunCode=SET_N_HLD_REG;//0X10
//				ModbusModule0.RegAddr=0;//д��������
//				ModbusModule0.RegNum=10;
//				ModbusModule0.P_Data=&HldReg[0];
//				ConstructFrame_RTUSetNHldReg(&ModbusModule0);
//				break;			
//   			case 1:
//				ModbusModule0.ID=1;
//				ModbusModule0.FunCode=READ_HLD_REG;//03
//				ModbusModule0.RegAddr=INPUT_STATE_EX_CTR;//����ز���
//				ModbusModule0.RegNum=8;
//				ModbusModule0.P_Data=&HldReg[INPUT_STATE_C];
//				ConstructFrame_RTUReadHldReg(&ModbusModule0);
//				break;
// 			case 2://���ôӻ�վ��
//				if (HldReg[SLAVE_NUMBER_W_FLAG]	== 1)//�������ʹ�վ��ַ�ı�־
//				{
//					ModbusModule0.ID=250;
//					ModbusModule0.FunCode=SET_1_HLD_REG;//06
//					ModbusModule0.RegAddr=SLAVE_NUMBER;//�ӻ�վ�Ŵ�ŵ�ַ
//					ModbusModule0.RegNum=1;
//					ModbusModule0.P_Data=&HldReg[SLAVE_NUMBER_W];
//					ConstructFrame_RTUSet1HldReg(&ModbusModule0);
//					HldReg[SLAVE_NUMBER_W_FLAG]	= 0;//�������ʹ�վ��ַ�ı�־
//				}
//				else com_step ++;
//				break;		
//			case 3://��ȡ�ӻ�վ��
//				ModbusModule0.ID=250;
//				ModbusModule0.FunCode=READ_HLD_REG;//03
//				ModbusModule0.RegAddr=SLAVE_NUMBER;//
//				ModbusModule0.RegNum=1;
//				ModbusModule0.P_Data=&HldReg[SLAVE_NUMBER_R];//
//				ConstructFrame_RTUReadHldReg(&ModbusModule0);
//				break;				
//			default: 
//				ModbusModule1.p_uart->Status=SEND_EXCHANGE;
//				break;			
//		}	
//	}	
//}
//--------------------------------------------------------------------
//��չģ��
//--------------------------------------------------------------------
void EXTEND_ComModule(void)
{
	static uint8_t com_step=0;
	uint16_t temp;
		
	if(ModbusModule1.p_uart->Status==SEND_EXCHANGE )
	{
		if(com_step < 1) com_step++;//
		else com_step=0;
		ModbusModule1.p_uart->Status=IDLE_WAIT;
	}

	if(ModbusModule1.p_uart->Status==IDLE_WAIT)
	{
		switch (com_step)
		{
   			case 0:
//				ModbusModule1.ID=1;
//				ModbusModule1.FunCode=SET_1_HLD_REG;//06
//				ModbusModule1.RegAddr=0x0000;//
//				ModbusModule1.RegNum=1;
//				ModbusModule1.P_Data=&HldReg[MAIN_OUTPUT0];//
//				ConstructFrame_RTUSet1HldReg(&ModbusModule1);						
				break;
 				   			
			default:
				ModbusModule1.p_uart->Status=SEND_EXCHANGE;
				break;			
		}	
	}		
}
//--------------------------------------------------------------------
//��չģ��
//--------------------------------------------------------------------
void EXTEND_ComModule11(void)
{
	static uint8_t com_step=0;
	uint16_t temp;
		
	if(ModbusModule0.p_uart->Status==SEND_EXCHANGE )
	{
		if(com_step < 1) com_step++;//
		else com_step=0;
		ModbusModule0.p_uart->Status=IDLE_WAIT;
	}

	if(ModbusModule0.p_uart->Status==IDLE_WAIT)
	{
		switch (com_step)
		{
   			case 0:
//				ModbusModule0.ID=1;
//				ModbusModule0.FunCode=SET_1_HLD_REG;//06
//				ModbusModule0.RegAddr=0x0000;//
//				ModbusModule0.RegNum=1;
//				ModbusModule0.P_Data=&HldReg[0];//
//				ConstructFrame_RTUSet1HldReg(&ModbusModule0);						
//				break;
 			case 1:
				ModbusModule0.ID=1;
				ModbusModule0.FunCode=SET_1_HLD_REG;//06
				ModbusModule0.RegAddr=0x0001;//
				ModbusModule0.RegNum=1;
				ModbusModule0.P_Data=&HldReg[0];//
				ConstructFrame_RTUSet1HldReg(&ModbusModule0);						
				break;	   			
			default:
				ModbusModule0.p_uart->Status=SEND_EXCHANGE;
				break;			
		}	
	}		
}
u16 setColor =0xF2FF;                          //����״̬����ɫ
u16 color[]= {0,0,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0,0,0,0,0,};   //������״̬����ɫ
u8 setData_flag =100; //�������޸��ǲ��ϴ�
//--------------------------------------------------------------------
//������ͨ��
//--------------------------------------------------------------------
void VGUS_ComCtr(void)
{
//	uint8_t i;
	static u8 Curve_flag=0;  //���ʱ�䷢��

	static uint8_t com_step=0,change_page_temp=0,change_page_send_times=0,send_setData =0;
	static uint8_t beep_flag=0;
	static u8 DTLED_groud_back1,DTLED_groud_back2,DTLED_groud_last;//keySET_State
	static uint16_t e16[3];
	uint16_t a16=0;
	
	
	if(VgusModule0.p_uart->RxStatus==IDLE_WAIT && VgusModule0.p_uart->Status == SEND_EXCHANGE )
	{
		if (Timer_1MS[DGUS_SEND_DELAY] == 0)
		{
			if (change_page_temp != HldReg[CHANGE_PAGE] && HldReg[CHANGE_PAGE]  >0) //��Ҫ�л�ҳ��ʱ���ٷ�Ӧ
				com_step = 26;
//			else if(DTLED_groud_set >0) com_step = 13;
//			else if(DTLED_groud_set_no >0) com_step = 14;   //ȡ������
			else (com_step < 4) ? (com_step++) : (com_step = 0);
			VgusModule0.p_uart->Status=IDLE_WAIT;

		}
	}
	else 
	{
		if(sys_setRunPage !=0) Timer_1MS[DGUS_SEND_DELAY] = 25;//
		else Timer_1MS[DGUS_SEND_DELAY] = 25;//
	}
	runParam.door_state = DOOR_CLOSURE;
	runParam.run_state  = m_runState;//runFlag.curRunFlag;
  runParam.RCF_flag   = g_ucfDispXg;
	runParam.BIT_state  =(runFlag.motorRunFlag)|(runFlag.curRunFlag<<1)|(Y4<<2)|(enable_RunTime<<3);
	runParam.motorState = runFlag.curRunFlag;
	runParam.MaxSpeed   = Speed_max[HldReg[ROTOR]];
	runParam.MaxRCF   = Force_max[HldReg[ROTOR]];

	if(g_ucfDispXg)  
	{
		HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//��ʾ������
		runParam.SpeedRCF  = runParam.RCF;
	}
	else 
	{
		HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
		runParam.SpeedRCF  = runParam.Speed;
	}
	//0:�������״̬  1����ͣ״̬  2�����ȿ���״̬
	runData.RTC_Year = RTC_DateStruct.RTC_Year+2000;
	runData.RTC_Date = RTC_DateStruct.RTC_Date;
	runData.RTC_Month = RTC_DateStruct.RTC_Month;
	runData.RTC_WeekDay = RTC_DateStruct.RTC_WeekDay;
	runData.RTC_Hours =RTC_TimeStruct.RTC_Hours;
	runData.RTC_Minutes =RTC_TimeStruct.RTC_Minutes;
	runData.RTC_Seconds =RTC_TimeStruct.RTC_Seconds;
	
//	runParam.RCF = Y4_OFF_time;
//	runParam.Speed = HldReg[SET_hot_time];
	
	CurveData.start5AA5 =0x5AA5;
	CurveData.Number =0x0400;
	CurveData.Curve1 =0x0402;
	CurveData.Curve2 =0x0502;
	CurveData.Curve3 =0x0602;
	CurveData.Curve4 =0x0702;
	
	CurveData.Data11 =runParam.Speed;
	CurveData.Data12 =runParam.Speed;
	CurveData.Data21 =HldReg[SET_SPEED];
	CurveData.Data22 =HldReg[SET_SPEED];
	CurveData.Data31 =(s16)HldReg[SET_TEMPERATURE]*10+200;//runParam.Speed;
	CurveData.Data32 =(s16)HldReg[SET_TEMPERATURE]*10+200;//runParam.Speed;
	CurveData.Data41 =(s16)runParam.temprature+200;
	CurveData.Data42 =(s16)runParam.temprature+200;
	

	if(runParam.alarm >0)
	{
		runParam.state_alarm  = runParam.alarm+10;  //������ʾͼ�������11��ʼ
	}
	else runParam.state_alarm  = runParam.run_state;

	if(VgusModule0.p_uart->Status==IDLE_WAIT)
	{
		switch (com_step)
		{
			case 0:	
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+sizeof(runParam);//����1+��ַ2+����2*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x82
				VgusModule0.RegAddr=0x1000;			//��ز���  ʵ�����в���
				VgusModule0.P_Data=(INT16U *)&runParam;//
				VGUS_WriteVar(&VgusModule0);	
				VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
				break;					
					
			case 1:	
			
//				if(setData_flag ==0 && send_setData!=0){com_step++;if(send_setData >0) send_setData--;}   //�в����޸�ʱ���ϴ�����
//				else 
//				{
					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
					VgusModule0.RegNum=5+100*2;//����1+��ַ2+����2*N+CRC2
					VgusModule0.FunCode=WRITE_VAR;//0x82
					VgusModule0.RegAddr=0;			//�ϵ籣�ֲ���
					VgusModule0.P_Data=&HldReg[0];//
					VGUS_WriteVar(&VgusModule0);	
					VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
					
					if(setData_flag>0)setData_flag--;
					send_setData =5;
					break;	
//				}
			case 2: 
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+sizeof(AllRunTime);//����1+��ַ2+����2*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x82
				VgusModule0.RegAddr=0x1050;			//������ʱ��  �ۼ�����ʱ��
				VgusModule0.P_Data=(INT16U *)&AllRunTime;//
				VGUS_WriteVar(&VgusModule0);	
				VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
				break;
			
			case 3:    //����
				
				if(Curve_flag ==0 || sys_setRunPage!=0)
				{
					Curve_flag =4;
					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
					VgusModule0.RegNum=0x21;//����1+��ַ2+����2*N+CRC2
					VgusModule0.FunCode=WRITE_VAR;//0x82
					VgusModule0.RegAddr=0x0310;			//��ز���  ʵ�����в���
					VgusModule0.P_Data=(INT16U *)&CurveData;//
					VGUS_WriteReg(&VgusModule0);	
					VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
			
					break;
				}
				else 
				{
					Curve_flag--;com_step++;
				}
			case 4:	
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+sizeof(runData);//����1+��ַ2+����2*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x82
				VgusModule0.RegAddr=0x1020;			//��ز���  ʵ�����в���
				VgusModule0.P_Data=(INT16U *)&runData;//
				VGUS_WriteVar(&VgusModule0);	
				VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
				break;
			case 5:
				break;
//			case 13:   //�������ݸı���ɫ
//				DTLED_groud_set--;
//				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//				VgusModule0.RegNum=5+2;//����1+��ַ2+����2*N+CRC2
//				VgusModule0.FunCode=WRITE_VAR;//0x82
//				//VgusModule0.RegAddr=0x4023;			//������ʱ��  �ۼ�����ʱ��
//				VgusModule0.RegAddr=0x4003+DTLED_groud_set*0x10;			//
//				VgusModule0.P_Data=(INT16U *)&setColor ;//
//				VGUS_WriteVar(&VgusModule0);	
//				VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
//				DTLED_groud_set =0;
//				break;
//			case 14:   //ȡ��������ɫ
//				DTLED_groud_set_no--;
//				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//				VgusModule0.RegNum=5+2;//����1+��ַ2+����2*N+CRC2
//				VgusModule0.FunCode=WRITE_VAR;//0x82
//				VgusModule0.RegAddr=0x4003+DTLED_groud_set_no*0x10;			//
//				VgusModule0.P_Data=(INT16U *)&color[DTLED_groud_set_no] ;//
//				VGUS_WriteVar(&VgusModule0);	
//				DTLED_groud_set_no=0;
//				VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
//				break;
			case 24: 
				
//				if(keySET_State)
//				{
//					if(DTLED_groud_back1 != DTLED_groud )
//					{
//						
//							
//							VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//							VgusModule0.RegNum=5+2;//����1+��ַ2+����2*N+CRC2
//							VgusModule0.FunCode=WRITE_VAR;//0x82
//							//VgusModule0.RegAddr=0x4023;			//������ʱ��  �ۼ�����ʱ��
//							VgusModule0.RegAddr=0x4003+DTLED_groud*0x10;			//
//							VgusModule0.P_Data=(INT16U *)&setColor ;//
//							VGUS_WriteVar(&VgusModule0);	
//							VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
//							if(DTLED_groud == GROUP_FRIST) DTLED_groud_last = GROUP_ALL-1;
//							else DTLED_groud_last = DTLED_groud-1;
//							DTLED_groud_back1 = DTLED_groud;
//					}
//					else 
//					{
//						if(DTLED_groud_last >=GROUP_ALL ) DTLED_groud_last = GROUP_FRIST;
//						if(DTLED_groud_last == DTLED_groud) DTLED_groud_last++;
//						
//						VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//						VgusModule0.RegNum=5+2;//����1+��ַ2+����2*N+CRC2
//						VgusModule0.FunCode=WRITE_VAR;//0x82
//						//VgusModule0.RegAddr=0x4023;			//������ʱ��  �ۼ�����ʱ��
//						VgusModule0.RegAddr=0x4003+DTLED_groud_last*0x10;			//
//						VgusModule0.P_Data=(INT16U *)&color[DTLED_groud_last] ;//
//						VGUS_WriteVar(&VgusModule0);	
//						DTLED_groud_last++;
//						VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
//					}
//				}
//				else 
//				{
//					DTLED_groud_back1 =0xFF;
//					if(DTLED_groud_last >=GROUP_ALL ) DTLED_groud_last = GROUP_FRIST;

//						
//						VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//						VgusModule0.RegNum=5+2;//����1+��ַ2+����2*N+CRC2
//						VgusModule0.FunCode=WRITE_VAR;//0x82
//						//VgusModule0.RegAddr=0x4023;			//������ʱ��  �ۼ�����ʱ��
//						VgusModule0.RegAddr=0x4003+DTLED_groud_last*0x10;			//
//						VgusModule0.P_Data=(INT16U *)&color[DTLED_groud_last] ;//
//						VGUS_WriteVar(&VgusModule0);	
//						DTLED_groud_last++;
//						VgusModule0.BatchSendVarFlag &= 0xFE;//������Ͳ�����־
//				}
//				break;
					
			case 21:
//				if (HldReg[CHANGE_PAGE] == ALARMRECORD_PAGE)	//������¼��ʾ
				{	
//					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//					VgusModule0.RegNum=5+2*120;//����1+��ַ2+����2*N+CRC2
//					VgusModule0.FunCode=WRITE_VAR;//0x82
//					VgusModule0.RegAddr=ALARMNO_FIRST;			//������ʾ
//					VgusModule0.P_Data=&HldReg[ALARMNO_FIRST];//
//					VGUS_WriteVar(&VgusModule0);
//					break;
				}
			//	else com_step ++;	
			case 22:
//				if (HldReg[CHANGE_PAGE] == RUNRECORD_PAGE)	//���м�¼��ʾ
//				{	
//					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//					VgusModule0.RegNum=5+2*122;//����1+��ַ2+����2*N+CRC2
//					VgusModule0.FunCode=WRITE_VAR;//0x82
//					VgusModule0.RegAddr=RUNNO_FIRST;			//
//					VgusModule0.P_Data=&HldReg[RUNNO_FIRST];//
//					VGUS_WriteVar(&VgusModule0);
//					break;
////				}
//				else com_step ++;					
			case 26:
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+4;//����1+��ַ1+����1*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x80
				VgusModule0.RegAddr=0x0084;//�л�ҳ��	
				e16[0] = 0x5A01;
				e16[1] = HldReg[CHANGE_PAGE];
				VgusModule0.P_Data=&e16[0];//
				VGUS_WriteReg(&VgusModule0);	
				change_page_send_times ++;
				
				if(change_page_send_times >5) 
				{
					
					HldReg[CHANGE_PAGE] =0;
					change_page_temp = HldReg[CHANGE_PAGE] ;
					change_page_send_times =0;
				}
				break;
		
	
			default:
				VgusModule0.p_uart->Status=SEND_EXCHANGE;	
				break;
		}	
		
	}	
}


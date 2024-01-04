/*
**************************************************************
		文件名：	app_comm.c
		功	 能：	实现与变频器的通信的功能函数，定义通信
					协议。


**************************************************************
*/

#include "typedef.h"
//#include "MODBUS_RTU.H"
//#include "UART_VGUS.H"
MODBUS_MODULE ModbusModule0;
MODBUS_MODULE ModbusModule1;
VGUS_MODULE VgusModule0;

u8 CommSetFlag;  //通讯设置标志  通讯设置数据时不上传设置数据
INT16U	HldReg[HLDREG_MAX_NUM];// __attribute__((at(0X2000E000)));// __attribute__ ((section ("ER_RSV")));
INT16U	SlaveHldReg[SLAVE_HLDREG_MAX_NUM];//从站

RUNDATA runData;
CURVEDATA CurveData;
//初始化通信数据
void InitComm0Data(void)
{
//	//云平台		TR100D为从站，站号可设
	ModbusModule0.ID = 1;//从机站号;
	ModbusModule0.p_uart=&UART_6;
	ModbusModule0.p_uart->Status= SEND_END;//IDLE_WAIT;
	ModbusModule0.FunCode=READ_HLD_REG;
	ModbusModule0.RegAddr=0;
	ModbusModule0.RegNum=0;
	ModbusModule0.ReTryTimeMax=5;
	ModbusModule0.OverTimeMax=200;	
//	//连接扩展控制器		TR100D为主站
//	ModbusModule0.ID=1;
//	ModbusModule0.p_uart=&UART_6;
//	ModbusModule0.p_uart->Status=IDLE_WAIT;
//	ModbusModule0.FunCode=READ_HLD_REG;
//	ModbusModule0.RegAddr=0;
//	ModbusModule0.RegNum=0;
//	ModbusModule0.ReTryTimeMax=5;
//	ModbusModule0.OverTimeMax=20;	
	//扩展模块		TR100D为主站
//	ModbusModule1.ID=1;//
//	ModbusModule1.p_uart=&UART_4;
//	ModbusModule1.p_uart->Status=IDLE_WAIT;
//	ModbusModule1.FunCode=READ_HLD_REG;
//	ModbusModule1.RegAddr=0;
//	ModbusModule1.RegNum=0;
//	ModbusModule1.ReTryTimeMax=5;
//	ModbusModule1.OverTimeMax=40;		
	//串口屏
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
	VgusModule0.OverTimeMax=10;//5;//超出接收数据时间		
    VgusModule0.SendVarFlag = 0;		
}

//--------------------------------------------------------------------
//扩展控制器
//扩展模块是直接受主站控制
//扩展控制器是主站只发相关信息过去，阀由从站自已控制论
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
//				ModbusModule0.RegAddr=0;//写基本参数
//				ModbusModule0.RegNum=10;
//				ModbusModule0.P_Data=&HldReg[0];
//				ConstructFrame_RTUSetNHldReg(&ModbusModule0);
//				break;			
//   			case 1:
//				ModbusModule0.ID=1;
//				ModbusModule0.FunCode=READ_HLD_REG;//03
//				ModbusModule0.RegAddr=INPUT_STATE_EX_CTR;//读相关参数
//				ModbusModule0.RegNum=8;
//				ModbusModule0.P_Data=&HldReg[INPUT_STATE_C];
//				ConstructFrame_RTUReadHldReg(&ModbusModule0);
//				break;
// 			case 2://设置从机站号
//				if (HldReg[SLAVE_NUMBER_W_FLAG]	== 1)//主机发送从站地址的标志
//				{
//					ModbusModule0.ID=250;
//					ModbusModule0.FunCode=SET_1_HLD_REG;//06
//					ModbusModule0.RegAddr=SLAVE_NUMBER;//从机站号存放地址
//					ModbusModule0.RegNum=1;
//					ModbusModule0.P_Data=&HldReg[SLAVE_NUMBER_W];
//					ConstructFrame_RTUSet1HldReg(&ModbusModule0);
//					HldReg[SLAVE_NUMBER_W_FLAG]	= 0;//主机发送从站地址的标志
//				}
//				else com_step ++;
//				break;		
//			case 3://读取从机站号
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
//扩展模块
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
//扩展模块
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
u16 setColor =0xF2FF;                          //设置状态的颜色
u16 color[]= {0,0,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0,0,0,0,0,};   //非设置状态的颜色
u8 setData_flag =100; //数据有修改是才上传
//--------------------------------------------------------------------
//触摸屏通信
//--------------------------------------------------------------------
void VGUS_ComCtr(void)
{
//	uint8_t i;
	static u8 Curve_flag=0;  //间隔时间发送

	static uint8_t com_step=0,change_page_temp=0,change_page_send_times=0,send_setData =0;
	static uint8_t beep_flag=0;
	static u8 DTLED_groud_back1,DTLED_groud_back2,DTLED_groud_last;//keySET_State
	static uint16_t e16[3];
	uint16_t a16=0;
	
	
	if(VgusModule0.p_uart->RxStatus==IDLE_WAIT && VgusModule0.p_uart->Status == SEND_EXCHANGE )
	{
		if (Timer_1MS[DGUS_SEND_DELAY] == 0)
		{
			if (change_page_temp != HldReg[CHANGE_PAGE] && HldReg[CHANGE_PAGE]  >0) //需要切换页面时快速反应
				com_step = 26;
//			else if(DTLED_groud_set >0) com_step = 13;
//			else if(DTLED_groud_set_no >0) com_step = 14;   //取消设置
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
		HldReg[SET_SPEEDRCF] = HldReg[SET_RCF];//显示离心力
		runParam.SpeedRCF  = runParam.RCF;
	}
	else 
	{
		HldReg[SET_SPEEDRCF] = HldReg[SET_SPEED];
		runParam.SpeedRCF  = runParam.Speed;
	}
	//0:电机运行状态  1：启停状态  2：加热开关状态
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
		runParam.state_alarm  = runParam.alarm+10;  //报警显示图标变量从11开始
	}
	else runParam.state_alarm  = runParam.run_state;

	if(VgusModule0.p_uart->Status==IDLE_WAIT)
	{
		switch (com_step)
		{
			case 0:	
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+sizeof(runParam);//命令1+地址2+数据2*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x82
				VgusModule0.RegAddr=0x1000;			//监控参数  实际运行参数
				VgusModule0.P_Data=(INT16U *)&runParam;//
				VGUS_WriteVar(&VgusModule0);	
				VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
				break;					
					
			case 1:	
			
//				if(setData_flag ==0 && send_setData!=0){com_step++;if(send_setData >0) send_setData--;}   //有参数修改时才上传数据
//				else 
//				{
					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
					VgusModule0.RegNum=5+100*2;//命令1+地址2+数据2*N+CRC2
					VgusModule0.FunCode=WRITE_VAR;//0x82
					VgusModule0.RegAddr=0;			//断电保持参数
					VgusModule0.P_Data=&HldReg[0];//
					VGUS_WriteVar(&VgusModule0);	
					VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
					
					if(setData_flag>0)setData_flag--;
					send_setData =5;
					break;	
//				}
			case 2: 
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+sizeof(AllRunTime);//命令1+地址2+数据2*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x82
				VgusModule0.RegAddr=0x1050;			//总运行时间  累计运行时间
				VgusModule0.P_Data=(INT16U *)&AllRunTime;//
				VGUS_WriteVar(&VgusModule0);	
				VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
				break;
			
			case 3:    //曲线
				
				if(Curve_flag ==0 || sys_setRunPage!=0)
				{
					Curve_flag =4;
					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
					VgusModule0.RegNum=0x21;//命令1+地址2+数据2*N+CRC2
					VgusModule0.FunCode=WRITE_VAR;//0x82
					VgusModule0.RegAddr=0x0310;			//监控参数  实际运行参数
					VgusModule0.P_Data=(INT16U *)&CurveData;//
					VGUS_WriteReg(&VgusModule0);	
					VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
			
					break;
				}
				else 
				{
					Curve_flag--;com_step++;
				}
			case 4:	
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+sizeof(runData);//命令1+地址2+数据2*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x82
				VgusModule0.RegAddr=0x1020;			//监控参数  实际运行参数
				VgusModule0.P_Data=(INT16U *)&runData;//
				VGUS_WriteVar(&VgusModule0);	
				VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
				break;
			case 5:
				break;
//			case 13:   //设置数据改变颜色
//				DTLED_groud_set--;
//				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//				VgusModule0.RegNum=5+2;//命令1+地址2+数据2*N+CRC2
//				VgusModule0.FunCode=WRITE_VAR;//0x82
//				//VgusModule0.RegAddr=0x4023;			//总运行时间  累计运行时间
//				VgusModule0.RegAddr=0x4003+DTLED_groud_set*0x10;			//
//				VgusModule0.P_Data=(INT16U *)&setColor ;//
//				VGUS_WriteVar(&VgusModule0);	
//				VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
//				DTLED_groud_set =0;
//				break;
//			case 14:   //取消设置颜色
//				DTLED_groud_set_no--;
//				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//				VgusModule0.RegNum=5+2;//命令1+地址2+数据2*N+CRC2
//				VgusModule0.FunCode=WRITE_VAR;//0x82
//				VgusModule0.RegAddr=0x4003+DTLED_groud_set_no*0x10;			//
//				VgusModule0.P_Data=(INT16U *)&color[DTLED_groud_set_no] ;//
//				VGUS_WriteVar(&VgusModule0);	
//				DTLED_groud_set_no=0;
//				VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
//				break;
			case 24: 
				
//				if(keySET_State)
//				{
//					if(DTLED_groud_back1 != DTLED_groud )
//					{
//						
//							
//							VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//							VgusModule0.RegNum=5+2;//命令1+地址2+数据2*N+CRC2
//							VgusModule0.FunCode=WRITE_VAR;//0x82
//							//VgusModule0.RegAddr=0x4023;			//总运行时间  累计运行时间
//							VgusModule0.RegAddr=0x4003+DTLED_groud*0x10;			//
//							VgusModule0.P_Data=(INT16U *)&setColor ;//
//							VGUS_WriteVar(&VgusModule0);	
//							VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
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
//						VgusModule0.RegNum=5+2;//命令1+地址2+数据2*N+CRC2
//						VgusModule0.FunCode=WRITE_VAR;//0x82
//						//VgusModule0.RegAddr=0x4023;			//总运行时间  累计运行时间
//						VgusModule0.RegAddr=0x4003+DTLED_groud_last*0x10;			//
//						VgusModule0.P_Data=(INT16U *)&color[DTLED_groud_last] ;//
//						VGUS_WriteVar(&VgusModule0);	
//						DTLED_groud_last++;
//						VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
//					}
//				}
//				else 
//				{
//					DTLED_groud_back1 =0xFF;
//					if(DTLED_groud_last >=GROUP_ALL ) DTLED_groud_last = GROUP_FRIST;

//						
//						VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//						VgusModule0.RegNum=5+2;//命令1+地址2+数据2*N+CRC2
//						VgusModule0.FunCode=WRITE_VAR;//0x82
//						//VgusModule0.RegAddr=0x4023;			//总运行时间  累计运行时间
//						VgusModule0.RegAddr=0x4003+DTLED_groud_last*0x10;			//
//						VgusModule0.P_Data=(INT16U *)&color[DTLED_groud_last] ;//
//						VGUS_WriteVar(&VgusModule0);	
//						DTLED_groud_last++;
//						VgusModule0.BatchSendVarFlag &= 0xFE;//清除发送参数标志
//				}
//				break;
					
			case 21:
//				if (HldReg[CHANGE_PAGE] == ALARMRECORD_PAGE)	//报警记录显示
				{	
//					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//					VgusModule0.RegNum=5+2*120;//命令1+地址2+数据2*N+CRC2
//					VgusModule0.FunCode=WRITE_VAR;//0x82
//					VgusModule0.RegAddr=ALARMNO_FIRST;			//报警显示
//					VgusModule0.P_Data=&HldReg[ALARMNO_FIRST];//
//					VGUS_WriteVar(&VgusModule0);
//					break;
				}
			//	else com_step ++;	
			case 22:
//				if (HldReg[CHANGE_PAGE] == RUNRECORD_PAGE)	//运行记录显示
//				{	
//					VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
//					VgusModule0.RegNum=5+2*122;//命令1+地址2+数据2*N+CRC2
//					VgusModule0.FunCode=WRITE_VAR;//0x82
//					VgusModule0.RegAddr=RUNNO_FIRST;			//
//					VgusModule0.P_Data=&HldReg[RUNNO_FIRST];//
//					VGUS_WriteVar(&VgusModule0);
//					break;
////				}
//				else com_step ++;					
			case 26:
				VgusModule0.ID_HI=0xA5;	VgusModule0.ID_LO=0x5A;
				VgusModule0.RegNum=5+4;//命令1+地址1+数据1*N+CRC2
				VgusModule0.FunCode=WRITE_VAR;//0x80
				VgusModule0.RegAddr=0x0084;//切换页面	
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


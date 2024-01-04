/*----------------------------------------------------------------------------
File Name			:MODBUS_RTU.C
New Date		  	:2011-7-5
Author			  	:WangXinJia
Hardware			:Supervisory Unit V10
Comments		  	:
------------------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "typedef.h"
#include "bsp_uart.h"			//����IO��������
#include "UART_VGUS.H"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//--------------------------------------------------
//Modbus RTU CRC16
//INT16U GetCRC16 (__IO INT8U * ptr,INT16U len) 
//{ 
//	INT8U i; 
//    INT16U crc_data=0xFFFF; 
//    if(len==0){ 
//    	len=1; 
//   	}
//   	while(len--){   
//		crc_data^=(*ptr); 
//        for(i=0; i<8; i++){ 
//           	if(crc_data&1){ 
//				crc_data>>=1;  
//				crc_data^=0xA001; 
//			}else{ 
//				crc_data>>=1;
//			}                            
//    	} 
//        ptr++; 
//    } 
//    return(crc_data);
//}
//---------------------------------------------
//  ��ʱ�ӵ�BCD���ʽת����ʮ����
//---------------------------------------------
u8 change_bcd_dec(u8 data)
{
	u8 temp;
	temp = (data >> 4) * 10 + (data & 0x0f);
	return temp;
}
//---------------------------------------------
//  ���ڷ���0x80ָ��֡�ṹ��д�Ĵ�����
//---------------------------------------------
void  VGUS_WriteReg (VGUS_MODULE *p_vgus)  
{ 
    INT16U  i=0,j=0;
    
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 

    p_vgus->p_uart->Buf[i++] = p_vgus->RegNum; 
	p_vgus->p_uart->Buf[i++] = p_vgus->FunCode; 
    p_vgus->p_uart->Buf[i++] = WORD_HI(p_vgus->RegAddr); 
    p_vgus->p_uart->Buf[i++] = WORD_LO(p_vgus->RegAddr); 
    for(j=0;j<((p_vgus->RegNum-5)/2);j++)
	{
        p_vgus->p_uart->Buf[i++] = WORD_HI( * (p_vgus->P_Data+j) );
        p_vgus->p_uart->Buf[i++] = WORD_LO( * (p_vgus->P_Data+j) );
    }	
	CRC16(p_vgus->p_uart->Buf+3, i-3);
    p_vgus->p_uart->Buf[i++] = uchCRCHi;
    p_vgus->p_uart->Buf[i++] = uchCRCLo;	
    p_vgus->p_uart->TxLen = i;
    p_vgus->p_uart->Point = 0;
	
	p_vgus->p_uart->Status=SEND_START;
		
} 
//---------------------------------------------
//  ���ڶ�ȡ0x81ָ��֡�ṹ�����Ĵ�����
//---------------------------------------------
void  VGUS_ReadReg (VGUS_MODULE *p_vgus)  
{ 
    INT16U  i=0;//j=0,
    
#ifdef DWIN_DMG
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;	
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 
	p_vgus->RegAddr+=0x1000;   //��������ַ��0x1000��ʼ
#else
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;
#endif 
    p_vgus->p_uart->Buf[i++] = p_vgus->RegNum; 
	p_vgus->p_uart->Buf[i++] = p_vgus->FunCode; 
    p_vgus->p_uart->Buf[i++] = p_vgus->RegAddr;
	p_vgus->p_uart->Buf[i++] = p_vgus->ReadRegNum;
	CRC16(p_vgus->p_uart->Buf+3, i-3);
    p_vgus->p_uart->Buf[i++] = uchCRCHi;
    p_vgus->p_uart->Buf[i++] = uchCRCLo;	
    p_vgus->p_uart->TxLen = i;
    p_vgus->p_uart->Point = 0;
	
	p_vgus->p_uart->Status=SEND_START;
} 
//---------------------------------------------
//  ���ڷ���0x82ָ��֡�ṹ��д�����洢����Ԫ��
//---------------------------------------------
void  VGUS_WriteVar (VGUS_MODULE *p_vgus)  
{ 
    INT16U  i=0,j=0;
    
 #ifdef DWIN_DMG
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;	
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 
	p_vgus->RegAddr+=0x1000;   //��������ַ��0x1000��ʼ
#else
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;
#endif	
    p_vgus->p_uart->Buf[i++] = p_vgus->RegNum; 
	p_vgus->p_uart->Buf[i++] = p_vgus->FunCode; 
    p_vgus->p_uart->Buf[i++] = WORD_HI(p_vgus->RegAddr); 
    p_vgus->p_uart->Buf[i++] = WORD_LO(p_vgus->RegAddr); 
    for(j=0;j<((p_vgus->RegNum-5)/2);j++)
	{
        p_vgus->p_uart->Buf[i++] = WORD_HI( * (p_vgus->P_Data+j) );
        p_vgus->p_uart->Buf[i++] = WORD_LO( * (p_vgus->P_Data+j) );
    }	
	CRC16(p_vgus->p_uart->Buf+3, i-3);
    p_vgus->p_uart->Buf[i++] = uchCRCHi;
    p_vgus->p_uart->Buf[i++] = uchCRCLo;	
    p_vgus->p_uart->TxLen = i;
    p_vgus->p_uart->Point = 0;
	
	p_vgus->p_uart->Status=SEND_START;
} 
//---------------------------------------------
//  ���ڶ�ȡ0x83ָ��֡�ṹ���������洢����Ԫ��
//---------------------------------------------
void  VGUS_ReadVar (VGUS_MODULE *p_vgus)  
{ 
    INT16U  i=0;//,j=0;
    
#ifdef DWIN_DMG
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;	
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 
		p_vgus->RegAddr+=0x1000;   //��������ַ��0x1000��ʼ
#else
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_HI; 
    p_vgus->p_uart->Buf[i++] = p_vgus->ID_LO;
#endif 
    p_vgus->p_uart->Buf[i++] = p_vgus->RegNum; 
	p_vgus->p_uart->Buf[i++] = p_vgus->FunCode; 
    p_vgus->p_uart->Buf[i++] = WORD_HI(p_vgus->RegAddr); 
    p_vgus->p_uart->Buf[i++] = WORD_LO(p_vgus->RegAddr); 
	p_vgus->p_uart->Buf[i++] = p_vgus->ReadRegNum;	
	CRC16(p_vgus->p_uart->Buf+3, i-3);
    p_vgus->p_uart->Buf[i++] = uchCRCHi;
    p_vgus->p_uart->Buf[i++] = uchCRCLo;	
    p_vgus->p_uart->TxLen = i;
    p_vgus->p_uart->Point = 0;
	
	p_vgus->p_uart->Status=SEND_START;
} 

//---------------------------------------------
//    ��վ���շ���  
//  ����ֵ��    0 	OK
//              1   CRCУ�����
//              2   վ�Ų�ƥ��	
//              3   �����벻ƥ��
//              4   16��Ӧ��ַ��ƥ��
//              5   16��Ӧ����������ƥ��
//---------------------------------------------

INT16U VgusMasterFrameAnalyse (VGUS_MODULE *p_vgus,INT16U  *dest_p) 
{
    INT16U i,addr,a16;//,j;//;//,ch,ch_var;
	uint8_t *msg;
	INT16U *r_addr;
	static uint32_t a32,b32;
	
	static u8 high_set_flag;
	static u8 getInSystemSet;   //����߼���������  ���������������ź�10������  �ٵ�����ż���������ϵͳ�ͺŵȸ߼���������
	
	//uint32_t a32,b32,c32,d32;
	msg=p_vgus->p_uart->RxBuf;
	r_addr=p_vgus->R_Data;
	if(p_vgus->p_uart->RxLen<5) return 1;
	else {}


		
#ifdef DWIN_DMG
	if (p_vgus->ID_LO != (*msg) || p_vgus->ID_HI != (*(msg+1)))	return 2;	//վ��
	else {}
#else
	if (p_vgus->ID_HI != (*msg) || p_vgus->ID_LO != (*(msg+1)))	return 2;	//վ��
	else {}	
#endif
    switch (*(msg+3))
	{  
		case WRITE_REG_VGUS://0x80д�Ĵ���
			break;
		case READ_REG_VGUS:	//0x81���Ĵ���
			CRC16(p_vgus->p_uart->RxBuf+3, *(msg+2)-2);	
			if (*(msg+(*(msg+2))+1) !=  uchCRCHi || *(msg+(*(msg+2))+2) !=  uchCRCLo) return 1;					// CRC У�鲻��ȷ 
			switch (*(msg+4))
			{
				case 0x20:	//ʱ��,��ʽΪBCD��
					for (i=0; i<(*(msg+5)); i++) *(r_addr+i) = change_bcd_dec(*(msg+6+i));
					if(p_vgus->p_uart->RxLen == 24)//��ʾ���յ��˰�ť����
					{//ʱ�������ܳ���Ϊ15�����������ܳ���Ϊ9
						addr = ((*(msg+19))<<8) + *(msg+20);	
						//if (addr == KEY_RETURN_ADDR)
							*(dest_p + addr) = ((*(msg+22))<<8) + *(msg+23);//���յ�������				
					}	
					else if(p_vgus->p_uart->RxLen == 26)//��ʾ���յ��˼Ĵ�������
					{//ʱ�������ܳ���Ϊ15���Ĵ��������ܳ���Ϊ11
						CRC16(p_vgus->p_uart->RxBuf+18, *(msg+17)-2);
						if (*(msg+(*(msg+17))+1) !=  uchCRCHi || *(msg+(*(msg+17))+2) !=  uchCRCLo) return 1;// CRC У�鲻��ȷ 
						addr = ((*(msg+19))<<8) + *(msg+20);	
						*(dest_p + addr) = ((*(msg+22))<<8) + *(msg+23);//���յ�������				
					}				
					break;
				default:
					break;
			}
			break; 		
		case WRITE_VAR:       	//0x82 д�����洢��
			if ( p_vgus->p_uart->RxLen >= 19 && READ_VAR == *(msg+11)){msg +=8;}//�ظ����ݸ��������������ͬʱ���͵���� ֱ��ת������������

			else break;
    case READ_VAR:	//0x83�������洢��
			CRC16(p_vgus->p_uart->RxBuf+3, *(msg+2)-2);
			#ifdef DWIN_DMG
						addr = ((*(msg+4))<<8) + *(msg+5) - 0x1000;
			#else
						addr = ((*(msg+4))<<8) + *(msg+5);		
			#endif
			if(addr ==IN_MIMA_H ||addr ==IN_MIMA1_H ) a32 = ((u32)(*(msg+7))<<24 ) +((u32)(*(msg+8))<<16 ) + ((u32)(*(msg+9))<<8 ) + (*(msg+10));//���յ�������
			else if(addr ==IN_MIMA2_H)                b32 = ((*(msg+7))<<24 ) +((*(msg+8))<<16 ) + ((*(msg+9))<<8 ) + (*(msg+10));//���յ�������
					
			else a16 = ( (*(msg+7))<<8 ) + (*(msg+8));//���յ�������

			
			if(addr == START_KEY)   //����
			{
				getStartKey =1;getInSystemSet =0;
			}
			else if(addr == STOP_KEY)   //ֹͣ
			{
				getStopKey =1;getInSystemSet =0;
			}
			else if(addr == SHORT_KEY)   //�㶯 ����������
			{
				if(a16) 
				{
					if(runFlag.curRunFlag ==0) //getStopKey =1;
				
					{
						getStartKey =1;
						short_key_flag =1;
						runFlag.shut_run =1;  //�㶯��־  short_key_time =0;  //������Ϊ�㶯 �ΰ�Ϊ����   ???
						getInSystemSet =0;
					}
					
				}
				else 
				{
					getStopKey =1;
				}
			}
			else if(addr == STOPATART_KEY)   //����ֹͣ����
			{
				if(runFlag.curRunFlag) getStopKey =1;
				else getStartKey =1;
				getInSystemSet =0;
			}
			else if(addr == DOOR_KEY)   //����
			{
				if(runParam.Speed <30) OnDrSgn =1;
				if(getInSystemSet >=10) HldReg[CHANGE_PAGE] = HIGHSET_PAGE;
			}
			else if(addr == CLEAR_ALARM)   //�������
			{
				SetAlarmFlag(NO_ALARM);
				ClearBeep();
				getInSystemSet++;
			}	
			else if(addr == CLEAR_ALLRUNTIME)   //����ۼ�����ʱ��
			{
				InitTotalTime();
			}	
			else if(addr == RCF_KEY)   //�л�ת��������
			{
				if(g_ucfDispXg ==1) g_ucfDispXg =0;
				else g_ucfDispXg =1;
			}	
			else if(addr >= UPDATA_YEAR && addr <= UPDATA_MINUTES)//����ʱ��
			{
				switch(addr)
				{
					case UPDATA_YEAR:
						RTC_DateStruct.RTC_Year = a16-2000;
						break;
					case UPDATA_MONTH:
						RTC_DateStruct.RTC_Month =a16;
						break;
					case UPDATA_DATA:
						RTC_DateStruct.RTC_Date =a16;
						break;
					case UPDATA_HOURS:
						RTC_TimeStruct.RTC_Hours =a16;
						break;
					case UPDATA_MINUTES:
						RTC_TimeStruct.RTC_Minutes =a16;
						break;

				}
				RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
				RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
			}
      else if(addr == IN_MIMA_H)
			{
				if(a32 == 884588 /*|| a32==((HldReg[SAVE_MIMA_H]<<16) +HldReg[SAVE_MIMA_L])*/)  //����������ȷ �������ý���
				{
					HldReg[CHANGE_PAGE] = 10;
				}
				a32 =0;
			}
				
			else 	if(addr ==IN_MIMA2_H)
			{
				if(a32 ==b32)  //���������������ͬ ��������
				{
					HldReg[CHANGE_PAGE] = 10;
					HldReg[SAVE_MIMA_H] = a32>>16;
					HldReg[SAVE_MIMA_L] = a32;
				}
				a32 =b32 =0;
			}		
			else 
			{
				getInSystemSet =0;
				if(runFlag.curRunFlag ==1 && (addr == PROGRAM || addr == ROTOR)){}  //�����в��޸ĳ���ź�ת�Ӻ�
				else
				{
					for (i=0; i<(*(msg+6)); i++) *(dest_p + addr + i) = ( (*(msg+i*2+7))<<8 ) + (*(msg+i*2+8));
				}
			}	
			if(addr == IN_MIMA_H || addr == IN_MIMA1_H || addr == IN_MIMA2_H){}
			else if (addr < 200)	//��������
			{
				if(addr == PROGRAM && runFlag.curRunFlag ==0 )   //���ó����  ȡ����������
				{
					if(a16 > MAX_PROGRAM) HldReg[PROGRAM] =MAX_PROGRAM;   //�����������
					read_eepromOneProgData(HldReg[PROGRAM]);    //��ȡEEPROM����
					get_programData(&OneProgramData);
					getvalue.BPHROTOR=1;
				}
				else if(addr == ROTOR && runFlag.curRunFlag ==0 )   //����ת�Ӻź�  ȡ�����ٵ�λ����ת�Ӷ�Ӧ��ת�ٵȣ�
				{
					if(a16 > RotorMaxNum ) HldReg[ROTOR] =RotorMaxNum;   //�����������
					else if(a16 <1) HldReg[ROTOR] =1;   //�����������
					get_RotorProgranData();
					getvalue.BPHROTOR=1;
				}
				else if(addr == SET_SPEED)   //����ת��  �Զ��л�������
				{
					if(a16 > Speed_max[HldReg[ROTOR]]) HldReg[SET_SPEED] =Speed_max[HldReg[ROTOR]];   //�������ת��
					else if(a16 < 100)  HldReg[SET_SPEED] = 100;
					HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED], HldReg[ROTOR]);
				}
				else if(addr == SET_RCF)   //����ת��  �Զ��л�������
				{
					if(a16 > Force_max[HldReg[ROTOR]]) HldReg[SET_RCF] =Force_max[HldReg[ROTOR]];   //�������ת��
					else if(a16 < 10)  HldReg[SET_RCF] = 10;
					HldReg[SET_SPEED] = Force_to_Speed(HldReg[SET_RCF], HldReg[ROTOR]);
				}
				else if(addr == SET_SPEEDRCF)   //����ת�ٻ�������
				{
					if(g_ucfDispXg)  
					{
						HldReg[SET_RCF] = HldReg[SET_SPEEDRCF];//��ʾ������
						if(HldReg[SET_RCF] > Force_max[HldReg[ROTOR]]) HldReg[SET_RCF] =Force_max[HldReg[ROTOR]];   //�������ת��
						else if(HldReg[SET_RCF] < 10)  HldReg[SET_RCF] = 10;
						HldReg[SET_SPEED] = Force_to_Speed(HldReg[SET_RCF], HldReg[ROTOR]);
					}
					else
					{
						HldReg[SET_SPEED] = HldReg[SET_SPEEDRCF];
						if(HldReg[SET_SPEED] > Speed_max[HldReg[ROTOR]]) HldReg[SET_SPEED] =Speed_max[HldReg[ROTOR]];   //�������ת��
						else if(HldReg[SET_SPEED] < 100)  HldReg[SET_SPEED] = 100;
						HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED], HldReg[ROTOR]);
					}
				}
				else if(addr == SYS_TYPE)   //�޸��ͺ� �Զ��޸Ļ�Ƶ�͵�����ʼֵ
				{
					if(HldReg[SYS_TYPE] <1 || HldReg[SYS_TYPE] >= VER_MAX_NUM)  HldReg[SYS_TYPE]  = INIT_SYS_TYPE;
						get_rotor_info(HldReg[SYS_TYPE],Speed_max,Force_max,&RotorMaxNum);
					if( HldReg[SYS_TYPE] >  LOW_TYPE)  //Ĭ��Ϊ���ٵ���Ļ�Ƶ������ʼֵ
					{
						HldReg[BASE_F]  = INIT_BASE_F_HIGH;
						HldReg[RATED_CURRENT]  = INTT_RATED_CURRENT_HIGH;
					}
					else 
					{
						HldReg[BASE_F]  = INIT_BASE_F_LOW;
						HldReg[RATED_CURRENT]  = INIT_RATED_CURRENT_LOW;
					}
				}
				Maxmin_Param(1);//	
				if(addr != PROGRAM) set_programData(&OneProgramData); //�������������
				if(addr != ROTOR)   set_RotorProgranData();           //����ת�Ӷ�Ӧ����������
				p_vgus->SendVarFlag = addr;//�÷��Ͳ�����־	
				writeDataFlag =1;  //  �������
			}
			break;
		default:
			break;
    }//end switch
    return 0;
}
//---------------------------------------------
//	VgusMasterRun
//	�人����VGUS��
//	˫��ͨ��
//---------------------------------------------
void VgusMasterRun (VGUS_MODULE *p_vgus)
{

	switch ( p_vgus->p_uart->Status )
	{
		case IDLE_WAIT	://0
			break;
		case SEND_START	://3
			p_vgus->p_uart->Point=0;						
			StartTxUART_8Bit(p_vgus->p_uart);
			p_vgus->p_uart->Status=SEND_ING;
			break;
		case SEND_ING: 	//6
			break;	
		case SEND_END	://4			
			p_vgus->p_uart->Status = SEND_EXCHANGE;				
			break;;
		case SEND_EXCHANGE	://5
			break;
		default	:
			break;
	}
	switch ( p_vgus->p_uart->RxStatus )//����
	{
		case IDLE_WAIT	://0
			break;
		case RECE_START	://1
			if (p_vgus->p_uart->TimeoutReg>=p_vgus->OverTimeMax)//**msδ�յ����ݣ���Ϊ���ݽ������
			{		//����֡����
				p_vgus->p_uart->RxStatus = RECE_END;	
				//p_vgus->p_uart->RxPoint = 0;	
			}
			else {}			
			break;
		case RECE_END	://2						
			if(VgusMasterFrameAnalyse(p_vgus,HldReg)==0) p_vgus->p_uart->RxStatus=IDLE_WAIT;//У���������ÿ���״̬
			else//���ݽ��ղ�����
			{
				p_vgus->p_uart->RxStatus =IDLE_WAIT;
			}							
      			
			break;
		case SEND_EXCHANGE	://5
			break;
		default	:
			break;
	}
}


/******** (C) COPYRIGHT 2010 LongChang Electronics CO.,ltd *****END OF FILE****/
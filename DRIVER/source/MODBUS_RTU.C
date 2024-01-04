/*----------------------------------------------------------------------------
File Name			:MODBUS_RTU.C
New Date		  	:2011-7-5
Author			  	:WangXinJia
Hardware			:Supervisory Unit V10
Comments		  	:
------------------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "typedef.h"
#include "bsp_uart.h"			//串口IO配置驱动
#include "MODBUS_RTU.H"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//MODBUS_MODULE ModbusModule0;
//--------------------------------------------------
//Modbus RTU CRC16
INT16U GetCRC16 (__IO uint8_t * ptr,INT16U len) 
{ 
	uint8_t i; 
    INT16U crc_data=0xFFFF; 
    if(len==0){ 
    	len=1; 
   	}
   	while(len--){   
		crc_data^=(*ptr); 
        for(i=0; i<8; i++){ 
           	if(crc_data&1){ 
				crc_data>>=1;  
				crc_data^=0xA001; 
			}else{ 
				crc_data>>=1;
			}                            
    	} 
        ptr++; 
    } 
    return(crc_data);
}
//---------------------------------------------
//  RTU Read Hold Register
//  CMD=03  READ_HLD_REG
//  Constructe Frame 
//---------------------------------------------
void  ConstructFrame_RTUReadHldReg (MODBUS_MODULE *p_modbus)  
{ 
    INT16U  i=0,j=0;
    
    p_modbus->p_uart->Buf[i++] = p_modbus->ID; 
    p_modbus->p_uart->Buf[i++] = p_modbus->FunCode; 
    p_modbus->p_uart->Buf[i++] = WORD_HI(p_modbus->RegAddr); 
    p_modbus->p_uart->Buf[i++] = WORD_LO(p_modbus->RegAddr); 
    p_modbus->p_uart->Buf[i++] = WORD_HI(p_modbus->RegNum); 
    p_modbus->p_uart->Buf[i++] = WORD_LO(p_modbus->RegNum); 
    j=GetCRC16(p_modbus->p_uart->Buf,i);
    p_modbus->p_uart->Buf[i++] = WORD_LO(j);
    p_modbus->p_uart->Buf[i++] = WORD_HI(j);
    p_modbus->p_uart->TxLen    = i;
    p_modbus->p_uart->Point    = 0;
	
	p_modbus->p_uart->Status=SEND_START;
} 

//---------------------------------------------
//  RTU Set N Hold Register
//  CMD=06  SET_1_HLD_REG
//  Constructe Frame 
//---------------------------------------------
void  ConstructFrame_RTUSet1HldReg (MODBUS_MODULE *p_modbus)  
{ 
    INT16U  i=0,j=0;
 
    p_modbus->p_uart->Buf[i++] = p_modbus->ID;
    p_modbus->p_uart->Buf[i++] = p_modbus->FunCode;  
    p_modbus->p_uart->Buf[i++] = WORD_HI(p_modbus->RegAddr); 
    p_modbus->p_uart->Buf[i++] = WORD_LO(p_modbus->RegAddr); 
    p_modbus->p_uart->Buf[i++] = WORD_HI(* (p_modbus->P_Data)); 
    p_modbus->p_uart->Buf[i++] = WORD_LO(* (p_modbus->P_Data)); 
    j=GetCRC16(p_modbus->p_uart->Buf,i);
    p_modbus->p_uart->Buf[i++] = WORD_LO(j);
    p_modbus->p_uart->Buf[i++] = WORD_HI(j);
	
//	CRC16(p_modbus->p_uart->Buf, i);
//    p_modbus->p_uart->Buf[i++] = uchCRCHi;
//    p_modbus->p_uart->Buf[i++] = uchCRCLo;
	
    p_modbus->p_uart->TxLen    = i;
    p_modbus->p_uart->Point    = 0;
	p_modbus->p_uart->Status=SEND_START;
}
//---------------------------------------------
//  RTU Set N Hold Register
//  CMD=16  SET_N_HLD_REG
//  Constructe Frame 
//---------------------------------------------
void  ConstructFrame_RTUSetNHldReg (MODBUS_MODULE *p_modbus)  
{ 
    INT16U  i=0,j=0;
 
    p_modbus->p_uart->Buf[i++] = p_modbus->ID;
    p_modbus->p_uart->Buf[i++] = p_modbus->FunCode;  
    p_modbus->p_uart->Buf[i++] = WORD_HI(p_modbus->RegAddr); 
    p_modbus->p_uart->Buf[i++] = WORD_LO(p_modbus->RegAddr); 
    p_modbus->p_uart->Buf[i++] = WORD_HI(p_modbus->RegNum); 
    p_modbus->p_uart->Buf[i++] = WORD_LO(p_modbus->RegNum); 
    p_modbus->p_uart->Buf[i++] = p_modbus->RegNum<<1;
    for(j=0;j<p_modbus->RegNum;j++){
        p_modbus->p_uart->Buf[i++] = WORD_HI( * (p_modbus->P_Data+j) );
        p_modbus->p_uart->Buf[i++] = WORD_LO( * (p_modbus->P_Data+j) );
    }
    j=GetCRC16(p_modbus->p_uart->Buf,i);
    p_modbus->p_uart->Buf[i++] = WORD_LO(j);
    p_modbus->p_uart->Buf[i++] = WORD_HI(j);
    p_modbus->p_uart->TxLen    = i;
    p_modbus->p_uart->Point    = 0;
	
	p_modbus->p_uart->Status=SEND_START;
}

//---------------------------------------------
//  RTU  主站接收分析  
//  3   READ_HLD_REG
//  16  SET_N_HLD_REG
//  返回值：    0 	OK
//              1   CRC校验错误
//              2   站号不匹配	
//              3   功能码不匹配
//              4   16回应地址不匹配
//              5   16回应数据字数不匹配
//---------------------------------------------
INT16U RTUMasterFrameAnalyse (MODBUS_MODULE *p_modbus,INT16U  *dest_p) 
{
    INT16U i,j;
    INT16U crc_result, crc_tmp;
    INT16U RegAddr,RegNum,RegData;
	INT16U rx_byte_num;
	uint8_t *msg;
	
	msg=p_modbus->p_uart->Buf;
	if(p_modbus->p_uart->RxLen<5) return 1;
	else {}
	
	j=0;
	while(j<p_modbus->p_uart->RxLen)
	{
		if( ( *(msg+j)==p_modbus->ID ) && ( *(msg+j+1)==p_modbus->FunCode) ) break;
		else {}
		j++;
	}
	if(j>=p_modbus->p_uart->RxLen)	return 2;
	else {}
	msg+=j;
	p_modbus->p_uart->RxLen-=j;
	
    crc_tmp  = (INT16U)(*(msg+(p_modbus->p_uart->RxLen)-1))<<8;
    crc_tmp += (INT16U)(*(msg+(p_modbus->p_uart->RxLen)-2));
	crc_result=GetCRC16(msg,p_modbus->p_uart->RxLen-2);
    if ( crc_tmp != crc_result ) 	return 1;					// CRC 校验正确 
    else {}
	if ( p_modbus->ID != (*msg) )	return 2;	//站号
	else {}
	if ( p_modbus->FunCode != *(msg+1) ) return 3;	//功能码不匹配
	else {}

    switch (p_modbus->FunCode)
	{  
		case READ_N_COIL:	//01
			rx_byte_num=(*(msg+2));
			for ( i=0; i<rx_byte_num; i+=2)	*(dest_p + i/2) = ( (*(msg+i+4))<<8 ) + (*(msg+i+3));
    
			if(rx_byte_num&BIT(0))
			{
				*(dest_p + i/2) &=0xFF00;
				*(dest_p + i/2) |=(*(msg+i+3))&0xFF;	//low 8 bit renew
			}
			break; 		    			
        case READ_HLD_REG:	//03
//			if ( p_modbus->ID != 250 || (p_modbus->ID == 250)
//				&& ModbusModule0.SendVarFlag[0] == 0 && ModbusModule0.SendVarFlag[1] == 0 && ModbusModule0.SendVarFlag[2] == 0) )//触摸屏上改变了数据时，暂不从模块更新数据上来
//			if ( p_modbus->ID <= 250)	
//				for ( i=0; i<(*(msg+2)); i+=2) *(dest_p + i/2) = ( (*(msg+i+3))<<8 ) + (*(msg+i+4));
//			else if ( p_modbus->ID <= 251)	
				for ( i=0; i<(*(msg+2)); i+=2) *(dest_p + i/2) = ( (*(msg+i+3))<<8 ) + (*(msg+i+4));			
			break;
		case SET_1_HLD_REG://06
			RegAddr = ( (*(msg+2))<<8) + (*(msg+3));
            RegData = ( (*(msg+4))<<8) + (*(msg+5));	
            if ( RegAddr != p_modbus->RegAddr )  return 4;
			else {}
            if ( RegData  != *(p_modbus->P_Data) ) return 5;
			else {}Timer_1MS[EXTEND_COM_TIMER] = 1000;
			if ((*msg) == 0x01)//站号1数据
			{	
				if (RegAddr == 0) Timer_1MS[EXTEND_COM_TIMER] = 1000;//地址0为主机发送给扩展模块的输出信号
			}
			break;
		case SET_N_COIL:	//15
        case SET_N_HLD_REG:	//16
            RegAddr= ( (*(msg+2))<<8) + (*(msg+3));
            RegNum = ( (*(msg+4))<<8) + (*(msg+5));
            if ( RegAddr != p_modbus->RegAddr )  return 4;
			else {}
            if ( RegNum  != p_modbus->RegNum )  return 5;
        	else {}						
            break;
    }//end switch
    return 0;
}
//---------------------------------------------
//	ModbusRTUMasterRun
//	主战先发起通讯，发送16（写），然后接收完毕，解析后发送3（读）然后接收完毕，解析（对从站1操作）
//	对从站2进行类似操作。然后循环。
//
//---------------------------------------------
void ModbusRTUMasterRun (MODBUS_MODULE *p_modbus)
{
//	INT16U i,j;
//	INT16U temp1,temp2;
//	u8 ucLen;

	switch ( p_modbus->p_uart->Status )
	{
		case IDLE_WAIT	://0
			break;
		case SEND_START	://3
			p_modbus->p_uart->Point=0;						
			p_modbus->p_uart->TimeoutReg=0;
			/*if(p_modbus->p_uart==&UART_2 )StartTxUART_9Bit(p_modbus->p_uart);
			else*/ StartTxUART_8Bit(p_modbus->p_uart);
			p_modbus->p_uart->Status=SEND_ING;   //状态切换
			break;
		case SEND_ING: 	//6
			break;	
		case SEND_END	://4			
			if(p_modbus->ID==0x00 && p_modbus->p_uart->TimeoutReg>=15) p_modbus->p_uart->Status=SEND_EXCHANGE;//广播，不用等待回复数据
			else {}
			if (p_modbus->p_uart->TimeoutReg>=p_modbus->OverTimeMax)
			{	//超时（无回应）
				p_modbus->ReTryTime++;//重试次数加1
				p_modbus->p_uart->TimeoutReg=0;
//				p_modbus->p_uart->Status = IDLE_WAIT;
				p_modbus->p_uart->Status = SEND_EXCHANGE;				
			}
			break;
		case RECE_START	://1
			if (p_modbus->p_uart->TimeoutReg>=10)//10ms未收到数据，认为数据接收完成
			{		//接收帧结束
				p_modbus->p_uart->Status = RECE_END;				
			}
			else {}	
			break;
		case RECE_END	://2				
//			if(RTUMasterFrameAnalyse(p_modbus,p_modbus->P_Data)==0) p_modbus->p_uart->Status=SEND_EXCHANGE;//校验正常，发送下一个数据
//			else//数据接收不正常，重发
//			{
//				p_modbus->ReTryTime++;
//				p_modbus->p_uart->TimeoutReg=0;
////				p_modbus->p_uart->Status = IDLE_WAIT;
//				p_modbus->p_uart->Status = SEND_EXCHANGE;
//			}			
p_modbus->p_uart->Status = SEND_EXCHANGE;		
			break;
		case SEND_EXCHANGE	://5
			break;
		default	:
			break;
	}
	if( p_modbus->ReTryTime > p_modbus->ReTryTimeMax)//重发次数到达，重启串口
	{
//		StartRxUART_8Bit(p_modbus->p_uart);
//		p_modbus->ReTryTime=0;						
//		UART_Init(p_modbus->p_uart);
//		p_modbus->p_uart->Status=SEND_EXCHANGE;
	}
	else {}
}
//---------------------------------------------
//	RTU Set N Hold Register
//	CMD=16	SET_N_HLD_REG
//	Constructe Answer Frame 
//---------------------------------------------
void  ModbusSlaveSetNHldRegAnswer (volatile UART_MODULE *UART,uint8_t board_adr,INT16U start_address,INT16U length)  
{
	uint8_t	i=0;
	uint16_t j=0;

    UART->Buf[i++] = board_adr; 
    UART->Buf[i++] = SET_N_HLD_REG; 
    UART->Buf[i++] = WORD_HI(start_address); 
    UART->Buf[i++] = WORD_LO(start_address); 
    UART->Buf[i++] = WORD_HI(length); 
    UART->Buf[i++] = WORD_LO(length); 
	j=GetCRC16(UART->Buf,i);
	UART->Buf[i++] = WORD_LO(j);
	UART->Buf[i++] = WORD_HI(j);
	UART->TxLen    = i;
	UART->Point	  = 0;
}
//---------------------------------------------
//	RTU Read Hold Register
//	CMD=05	SET_1_COIL
//	Constructe Answer Frame 
//---------------------------------------------
void  ModbusSlaveSet1CoilAnswer (volatile UART_MODULE *UART,uint8_t board_adr,INT16U bit_address,INT16U data)
{

	uint8_t	i=0;
	INT16U	j=0;
 
    UART->Buf[i++] = board_adr; 
    UART->Buf[i++] = SET_1_COIL; 
	UART->Buf[i++] = WORD_HI(bit_address); 
    UART->Buf[i++] = WORD_LO(bit_address); 
    UART->Buf[i++] = WORD_HI(data); 
    UART->Buf[i++] = WORD_LO(data); 
	j=GetCRC16(UART->Buf,i);
	UART->Buf[i++] = WORD_LO(j);
	UART->Buf[i++] = WORD_HI(j);
	UART->TxLen    = i;
	
//	UART->TxLen    = 8;
	UART->Point	  = 0;
	
}
void  ModbusSlaveSet1HldRegAnswer (volatile UART_MODULE *UART,uint8_t board_adr,INT16U address,INT16U data)
{
	uint8_t	i=0;
	INT16U	j=0;
 
    UART->Buf[i++] = board_adr; 
    UART->Buf[i++] = SET_1_HLD_REG; 
	UART->Buf[i++] = WORD_HI(address); 
    UART->Buf[i++] = WORD_LO(address); 
    UART->Buf[i++] = WORD_HI(data); 
    UART->Buf[i++] = WORD_LO(data); 
	j=GetCRC16(UART->Buf,i);
	UART->Buf[i++] = WORD_LO(j);
	UART->Buf[i++] = WORD_HI(j);
	UART->TxLen    = i;
	
	UART->Point	  = 0;
}

//---------------------------------------------
//	RTU Read Hold Register
//	CMD=01	READ_N_COIL
//	Constructe Answer Frame 
//---------------------------------------------
void  ModbusSlaveReadNCoilAnswer (volatile UART_MODULE *UART,uint8_t board_adr,INT16U *com_buf,INT16U bit_address,INT16U length)
{
	uint8_t	i=0,byte_num;
	INT16U	j=0,m=0,*p_data;
 
    UART->Buf[i++] = board_adr; 
    UART->Buf[i++] = READ_N_COIL; 
	byte_num	   = (length+7)>>3;
//	word_num	   = byte_num>>1;
	UART->Buf[i++] = byte_num;
	p_data		   = com_buf;	
	for(j=0;j<length;j++){
		if( ( *(p_data) )&BIT(bit_address++) ){
			UART->Buf[i]|= BIT(m++);
		}else{
			UART->Buf[i]&=~BIT(m++);
		}
		if(bit_address>15){
			p_data++;
			bit_address=0;
		}
		if(m>7){
			i++;
			m=0;
		}
	}
	if(length&0x07){
		i++;
	}
	j=GetCRC16(UART->Buf,i);
	UART->Buf[i++] = WORD_LO(j);
	UART->Buf[i++] = WORD_HI(j);
	UART->TxLen    = i;
	UART->Point	  = 0;
}
//---------------------------------------------
//	RTU Read Hold Register
//	CMD=02	READ_N_DI
//	Constructe Answer Frame 
//---------------------------------------------
void  ModbusSlaveReadNDIAnswer (volatile UART_MODULE *UART,uint8_t board_adr,INT16U *com_buf,INT16U bit_address,INT16U length)
{
	uint8_t	i=0,byte_num;
	INT16U	j=0,m=0,*p_data;
 
    UART->Buf[i++] = board_adr; 
    UART->Buf[i++] = READ_N_DI; 
	byte_num	   = (length+7)>>3;
//	word_num	   = byte_num>>1;
	UART->Buf[i++] = byte_num;	
	p_data=com_buf;	
	for(j=0;j<length;j++){
		if( ( *(p_data) )&BIT(bit_address++) ){
			UART->Buf[i]|= BIT(m++);
		}else{
			UART->Buf[i]&=~BIT(m++);
		}
		if(bit_address>15){
			p_data++;
			bit_address=0;
		}
		if(m>7){
			i++;
			m=0;
		}
	}
	if(length&0x07){
		i++;
	}
	j=GetCRC16(UART->Buf,i);
	UART->Buf[i++] = WORD_LO(j);
	UART->Buf[i++] = WORD_HI(j);
	UART->TxLen    = i;
	UART->Point	  = 0;
}
//---------------------------------------------
//	RTU Read Hold Register
//	CMD=03	READ_HLD_REG
//	Constructe Answer Frame 
//---------------------------------------------
void  ModbusSlaveReadHldRegAnswer (volatile UART_MODULE *UART,uint8_t board_adr,INT16U *com_buf,INT16U lenth)
{
	uint8_t	i=0;
	INT16U	j=0;
 
    UART->Buf[i++] = board_adr; 
    UART->Buf[i++] = READ_HLD_REG; 
	UART->Buf[i++] = lenth<<1;
	for(j=0;j<lenth;j++){
		UART->Buf[i++] = WORD_HI( *(com_buf+j) );
		UART->Buf[i++] = WORD_LO( *(com_buf+j) );
	}
	j=GetCRC16(UART->Buf,i);
	UART->Buf[i++] = WORD_LO(j);
	UART->Buf[i++] = WORD_HI(j);
	UART->TxLen = i;
	UART->Point	= 0;
	
}
//---------------------------------------------
//	RTU  从站接收分析  
//	3	READ_HLD_REG
//	16	SET_N_HLD_REG
//	返回值：	0 	OK
//				1	CRC校验错误
//				2	站号不匹配	
//				3	命令码不匹配
//				4	16写数据字数不匹配
//				5	不识别的命令
//---------------------------------------------

uint8_t RTUSlaveFrameAnalyse (MODBUS_MODULE *p_modbus_rtu,volatile UART_MODULE *UART,volatile uint8_t *msg,uint8_t len) 
{
	uint8_t i;
	INT16U crc_result, crc_tmp;
	INT16U RegAddr,RegNum,RegData;
	INT16U  *dest_p;

	crc_tmp  = (INT16U)(*(msg+len-1))<<8;
    crc_tmp += (INT16U)(*(msg+len-2));
	crc_result=GetCRC16(msg,len-2);
    if (crc_tmp != crc_result) 						// CRC 校验正确 
       	return 1;     
	if (p_modbus_rtu->ID != (*msg) && (*msg) != 254)//从站站号为254时设置站号
		return 2;
//	if ( ModbusModule0.FunCode != *(msg+1) )
//		return 3;
	p_modbus_rtu->FunCode=(*(msg+1));
	switch (p_modbus_rtu->FunCode)
	{ 
//		case READ_N_COIL:	//1
//			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
//    		RegNum = ((*(msg+4))<<8) + (*(msg+5));
//			if( RegAddr>COILREG_MAX_ADR ){
//				return 4;
//			}
//			if(RegNum>COILREG_MAX_ADR){
//				return 5;
//			}
//			if( (RegAddr+RegNum)>COILREG_MAX_ADR){
//				return 6;
//			}			
//			ModbusSlaveReadNCoilAnswer(UART,p_modbus_rtu->ID,&CoilReg[RegAddr>>4],RegAddr&0x000F,RegNum );
//			break;
//		case READ_N_DI:		//2		
//			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
//    		RegNum = ((*(msg+4))<<8) + (*(msg+5));	
//			if( RegAddr>INREG_MAX_ADR ){
//				return 4;
//			}
//			if(RegNum>INREG_MAX_ADR){
//				return 5;
//			}
//			if( (RegAddr+RegNum)>INREG_MAX_ADR){
//				return 6;
//			}	
//			ModbusSlaveReadNDIAnswer(UART,p_modbus_rtu->ID,&InReg[RegAddr>>4],RegAddr&0x000F,RegNum );
//			break;			
		case READ_HLD_REG:	//03
		case READ_AI:		//04
			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
    		RegNum = ((*(msg+4))<<8) + (*(msg+5));
			//if( RegAddr>SLAVE_HLDREG_MAX_ADR ) return 4;//主站发送的数据超出了定义的范围
			//if( RegNum>SLAVE_HLDREG_MAX_ADR || RegNum>125) return 5;
			//if( (RegAddr+RegNum)>SLAVE_HLDREG_MAX_ADR) return 6;	
			if(RegAddr >= 0x1000) ModbusSlaveReadHldRegAnswer(UART,p_modbus_rtu->ID,(INT16U *)&runParam+(RegAddr - 0x1000),RegNum );
			else //设置参数	
				ModbusSlaveReadHldRegAnswer(UART,p_modbus_rtu->ID,&HldReg[RegAddr],RegNum );////////////////////////////数组入口地址////////////////////			
			break;
//		case SET_1_COIL:	//05	写位状态
//			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
//    		RegData= ((*(msg+4))<<8) + (*(msg+5));	
//			if( RegAddr>COILREG_MAX_ADR ){
//				return 4;
//			}	
//			if(RegData==0xFF00){
//				CoilReg[RegAddr/16]|= BIT(RegAddr%16);
//			}else if (RegData==0x0000){
//				CoilReg[RegAddr/16]&=~BIT(RegAddr%16);
//			}else{
//				return 5;
//			}	
//			ModbusSlaveSet1CoilAnswer(UART,p_modbus_rtu->ID,RegAddr,RegData);
			break;
		case SET_1_HLD_REG:	//6
			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
    	RegData= ((*(msg+4))<<8) + (*(msg+5));	
			if(RegAddr == START_KEY && RegData ==1)   //启动
			{
				getStartKey =1;
			}
			else if(RegAddr == STOP_KEY&& RegData ==1)   //停止
			{
				getStopKey =1;
			}
			else if(RegAddr == DOOR_KEY&& RegData ==1)   //开门
			{
				OnDrSgn =1;
			}					
			else 
			{
				if(runFlag.curRunFlag ==1 && (RegAddr == PROGRAM || RegAddr == ROTOR)){}  //运行中不修改程序号和转子号
				else
				{

					HldReg[RegAddr] =RegData;
				}
			}	
			if (RegAddr < 200)	//参数设置
			{
				if(RegAddr == PROGRAM && runFlag.curRunFlag ==0 )   //设置程序号  取程序组数据
				{
					if(RegData > MAX_PROGRAM) HldReg[PROGRAM] =MAX_PROGRAM;   //限制最大程序号
					read_eepromOneProgData(HldReg[PROGRAM]);    //读取EEPROM数据
					get_programData(&OneProgramData);
					getvalue.BPHROTOR=1;
				}
				else if(RegAddr == ROTOR && runFlag.curRunFlag ==0 )   //设置转子号号  取升降速档位（或转子对应的转速等）
				{
					if(RegData > RotorMaxNum ) HldReg[ROTOR] =RotorMaxNum;   //限制最大程序号
					else if(RegData <1) HldReg[ROTOR] =1;   //限制最大程序号
					get_RotorProgranData();
					getvalue.BPHROTOR=1;
				}
				else if(RegAddr == SET_SPEED)   //设置转速  自动切换离心力
				{
					if(RegData > Speed_max[HldReg[ROTOR]]) HldReg[SET_SPEED] =Speed_max[HldReg[ROTOR]];   //限制最大转速
					else if(RegData < 100)  HldReg[SET_SPEED] = 100;
					HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_SPEED], HldReg[ROTOR]);
				}
				else if(RegAddr == SET_RCF)   //设置转速  自动切换离心力
				{
					if(RegData > Force_max[HldReg[ROTOR]]) HldReg[SET_RCF] =Force_max[HldReg[ROTOR]];   //限制最大转速
					else if(RegData < 10)  HldReg[SET_RCF] = 10;
					HldReg[SET_SPEED] = Force_to_Speed(HldReg[SET_RCF], HldReg[ROTOR]);
				}
				else if(RegAddr == SET_SPEEDRCF)   //设置转速或离心力
				{
					if(g_ucfDispXg)  
					{
						HldReg[SET_RCF] = HldReg[SET_SPEEDRCF];//显示离心力
						if(HldReg[SET_RCF] > Force_max[HldReg[ROTOR]]) HldReg[SET_RCF] =Force_max[HldReg[ROTOR]];   //限制最大转速
						else if(HldReg[SET_RCF] < 10)  HldReg[SET_SPEED] = 10;
						HldReg[SET_SPEED] = Force_to_Speed(HldReg[SET_RCF], HldReg[ROTOR]);
					}
					else
					{
						HldReg[SET_SPEED] = HldReg[SET_SPEEDRCF];
						if(HldReg[SET_SPEED] > Speed_max[HldReg[ROTOR]]) HldReg[SET_SPEED] =Speed_max[HldReg[ROTOR]];   //限制最大转速
						else if(HldReg[SET_SPEED] < 100)  HldReg[SET_SPEED] = 100;
						HldReg[SET_RCF] = Speed_to_Force(HldReg[SET_RCF], HldReg[ROTOR]);
					}
				}
				else if(RegAddr == SYS_TYPE)   //修改型号 自动修改基频和电流初始值
				{
					if(HldReg[SYS_TYPE] <1 || HldReg[SYS_TYPE] >= VER_MAX_NUM)  HldReg[SYS_TYPE]  = INIT_SYS_TYPE;
						get_rotor_info(HldReg[SYS_TYPE],Speed_max,Force_max,&RotorMaxNum);
					if( HldReg[SYS_TYPE] >  LOW_TYPE)  //默认为高速电机的基频电流初始值
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
				if(RegAddr != PROGRAM) set_programData(&OneProgramData); //保存程序组数据
				if(RegAddr != ROTOR)   set_RotorProgranData();           //保存转子对应的运行数据

				writeDataFlag =1;  //  保存参数
				
//			if( RegAddr>SLAVE_HLDREG_MAX_ADR ) return 4;///////////////////////////////////
//			SlaveHldReg[RegAddr]=RegData;
				
			}
	ModbusSlaveSet1HldRegAnswer(UART,p_modbus_rtu->ID,RegAddr,RegData);
			break;	
//		case SET_N_COIL:	//15
//			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
//    		RegNum = ((*(msg+4))<<8) + (*(msg+5));	
//			if ( (RegNum<<1) != (*(msg+6)) ){	//字节长度是否匹配
//				return 4;
//			}
//			return 5;
////			dest_p=&CoilReg[RegAddr];
////			for ( i=0; i<UART->Buf[6]; i+=2){ 
////				*(dest_p + i/2) = ( UART->Buf[i+7]<<8 ) + UART->Buf[i+8];
////	     		}
////      	ModbusSlaveSetNHldRegAnswer(ModbusModule0.ID,RegAddr,RegNum);				
//			break;		
		case SET_N_HLD_REG:	//0x10
			RegAddr= ((*(msg+2))<<8) + (*(msg+3));
    		RegNum = ((*(msg+4))<<8) + (*(msg+5));	
			if( RegAddr>HLDREG_MAX_ADR ) return 4;
			if(RegNum>HLDREG_MAX_ADR ||  RegNum>125 ) return 5;
			if( (RegAddr+RegNum)>HLDREG_MAX_ADR) return 6;
			if ( (RegNum<<1) != (*(msg+6)) ) return 7;	//字节长度是否匹配
			if( (RegAddr+RegNum)<=HLDREG_MAX_NUM && RegNum<125 )
			{		
				dest_p=&SlaveHldReg[RegAddr];
				for (i=0; i<(*(msg+6)); i+=2)
					*(dest_p + i/2) = ( (*(msg+i+7))<<8 ) + (*(msg+i+8));
			}
			else
				return 8;
      		ModbusSlaveSetNHldRegAnswer(UART,p_modbus_rtu->ID,RegAddr,RegNum);				
			break;
		default:
			return 9;	
	}//end switch
	return 0;
}

//---------------------------------------------
//	ModbusRTUSlaveRun
//	通讯由主站发起，从站初始化为接收，并相应的做出回应。
//	站号在初始化中有设置，以后不再更改。
//---------------------------------------------
void ModbusRTUSlaveRun (MODBUS_MODULE *p_modbus_rtu) 
{
	uint8_t i,j;
	switch (p_modbus_rtu->p_uart->Status)
	{
		case IDLE_WAIT	://0
			break;
		case SEND_START	://3
			break;
		case SEND_END	://4
			break;
		case RECE_START	://1
			if (p_modbus_rtu->p_uart->TimeoutReg>=5)//接收帧结束
			{		
				p_modbus_rtu->p_uart->Status = RECE_END;
			}
			break;
		case RECE_END	://2		
			if( (p_modbus_rtu->p_uart->RxLen)<5 )
			{
				p_modbus_rtu->p_uart->Status = SEND_END;
				break;
			}
			i=p_modbus_rtu->p_uart->RxLen-5;
			do{	
				if(p_modbus_rtu->p_uart->Buf[i]==p_modbus_rtu->ID || p_modbus_rtu->p_uart->Buf[i]== 254)//从站站号为254时设置站号
				{
					j=p_modbus_rtu->p_uart->Buf[i+1];
					if(j==0x01 || j==0x02 || j==0x03 || j==0x04 || 
					   j==0x05 || j==0x06 || j==0x0F || j==0x10 )
					{
						if ( RTUSlaveFrameAnalyse(p_modbus_rtu,p_modbus_rtu->p_uart,&(p_modbus_rtu->p_uart->Buf[i]),(p_modbus_rtu->p_uart->RxLen-i))==0 ){//帧解析正确
							p_modbus_rtu->p_uart->Status = SEND_START;
							p_modbus_rtu->p_uart->TimeoutReg=0;
							StartTxUART_8Bit(p_modbus_rtu->p_uart);
							//HldReg[COM_ERR] = 0;//通信错误标志
							break;
						}
					}
				}
			}while(i--);
			if(p_modbus_rtu->p_uart->Status != SEND_START) 
				p_modbus_rtu->p_uart->Status =SEND_END;
			break;
	}

	if ( p_modbus_rtu->p_uart->TimeoutReg >= 2000)//断网
	{		//超时30ms
//		p_modbus_rtu->ReTryTime++;
//		p_modbus_rtu->p_uart->TimeoutReg=0;
//		HldReg[COM_ERR] = 1;//通信错误标志
//		HldReg[UART_FLAG] = 0;//通信正常指示灯
//		StartRxUART_8Bit(p_modbus_rtu->p_uart);
//		UART_Init(p_modbus_rtu->p_uart);
////		CLR_UART1_RDE();
//		UART1Init();
	}

}

/******** (C) COPYRIGHT 2010 LongChang Electronics CO.,ltd *****END OF FILE****/

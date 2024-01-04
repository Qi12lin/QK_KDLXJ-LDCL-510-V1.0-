/*----------------------------------------------------------------------------
File Name			:MODBUS_ASCII_Slaver.H
New Date		  	:2010-7-11
Author			  	:WangXinJia
Hardware			:Supervisory Unit V10
Comments		  	:
------------------------------------------------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODBUS_ASCII_H
#define MODBUS_ASCII_H
/* Includes ------------------------------------------------------------------*/
//#include "CustomType.h"
/* Exported types ------------------------------------------------------------*/
typedef struct	{
	uint8_t	ID					;		//ID
	uint8_t	FunCode				;		//CMD
	INT16U	RegAddr				;		//(write start ddress)
	uint8_t	RegNum				;		//(write word number)
	uint8_t	ReTryTime			;		//communication retry time
	uint8_t	ReTryTimeMax		;		//max retry time
	INT16U	OverTimeMax			;		//max overtime  
	uint8_t	SlaverIDMax			;		//max slaver id 
	UART_MODULE *p_uart			;	
	INT16U  *P_Data				;	
	uint8_t   RegSubstance		;//读取的寄存器内容
	INT16U   SendVarFlag[4];//发送参数标志	
	INT16U   SendVarFlag2[4];//发送参数标志	
}MODBUS_MODULE;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/ 
//--------------MODBUS CMD--------------------------
#define READ_N_COIL    		0x01 
#define READ_N_DI      		0x02 
#define READ_HLD_REG  		0x03 
#define READ_AI       		0x04 
#define SET_1_COIL     		0x05 
#define SET_1_HLD_REG  		0x06 
#define SET_N_COIL     		0x0F
#define SET_N_HLD_REG		0x10
#define VFD_STATE				0x00
#define VFD_OUTPUT_FRE			0x01
 
/* Exported variables ------------------------------------------------------- */
extern MODBUS_MODULE ModbusModule0;
extern MODBUS_MODULE ModbusModule1;
/* Exported functions ------------------------------------------------------- */
extern void ModbusRTUMasterRun(MODBUS_MODULE *p_modbus);
extern void ConstructFrame_RTUReadHldReg(MODBUS_MODULE *p_modbus);
extern void ConstructFrame_RTUSet1HldReg(MODBUS_MODULE *p_modbus);
extern void ConstructFrame_RTUSetNHldReg(MODBUS_MODULE *p_modbus);
extern void ModbusRTUSlaveRun (MODBUS_MODULE *p_modbus_rtu);
/*----- High layer function -----*/
/*----- Low layer function -----*/
#endif /* BASIC_H */
/******** (C) COPYRIGHT 2010 LongChang Electronics CO.,ltd *****END OF FILE****/

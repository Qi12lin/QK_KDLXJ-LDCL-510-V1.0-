/*----------------------------------------------------------------------------
File Name			:MODBUS_ASCII_Slaver.H
New Date		  	:2010-7-11
Author			  	:WangXinJia
Hardware			:Supervisory Unit V10
Comments		  	:
------------------------------------------------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_VGUS_H
#define UART_VGUS_H

#define DWIN_DMG    1 	//����-DWGϵ�У�T5L

/* Includes ------------------------------------------------------------------*/
//#include "CustomType.h"
/* Exported types ------------------------------------------------------------*/
typedef struct	{
	uint8_t	ID_HI					;		//ID
	uint8_t	ID_LO					;		//ID
	uint8_t	RegNum				;		//(write word number)	
	uint8_t	FunCode				;		//CMD
	INT16U	RegAddr				;		//(write start ddress)
	uint8_t	ReadRegNum				;		//(write word number)	
	uint8_t	ReTryTime			;		//communication retry time
	uint8_t	ReTryTimeMax		;		//max retry time
	INT16U	OverTimeMax			;		//max overtime  
//	INT8U	SlaverIDMax			;		//max slaver id 
	UART_MODULE *p_uart			;	
	INT16U  *P_Data				;
	INT16U  *R_Data				;//��ȡ���ݵ�ָ��
	uint8_t   RegSubstance		;//��ȡ�ļĴ�������
	INT16U   SendVarFlag;//���Ͳ�����־
	uint8_t   BatchSendVarFlag;//�������Ͳ�����־
}VGUS_MODULE;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/ 
//--------------VGUS CMD--------------------------
#define WRITE_REG_VGUS      		0x80 //д�Ĵ���
#define READ_REG_VGUS    		0x81 //���Ĵ���
#define WRITE_VAR       	0x82 //д�����洢��
#define READ_VAR  			0x83 //�������洢��

 
/* Exported variables ------------------------------------------------------- */
extern VGUS_MODULE VgusModule0;
/* Exported functions ------------------------------------------------------- */
extern void  VGUS_WriteReg (VGUS_MODULE *p_vgus);  
extern void  VGUS_ReadReg (VGUS_MODULE *p_vgus);
extern void  VGUS_WriteVar (VGUS_MODULE *p_vgus); 
extern void  VGUS_ReadVar (VGUS_MODULE *p_vgus);
extern void VgusMasterRun (VGUS_MODULE *p_modbus);
//extern void ModbusRTUMasterRun(MODBUS_MODULE *p_modbus);
//extern void ConstructFrame_RTUReadHldReg(MODBUS_MODULE *p_modbus);
//extern void ConstructFrame_RTUSet1HldReg(MODBUS_MODULE *p_modbus);
//extern void ConstructFrame_RTUSetNHldReg(MODBUS_MODULE *p_modbus);

/*----- High layer function -----*/
/*----- Low layer function -----*/
#endif /* BASIC_H */
/******** (C) COPYRIGHT 2010 LongChang Electronics CO.,ltd *****END OF FILE****/
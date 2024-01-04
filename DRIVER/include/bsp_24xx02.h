/*
*********************************************************************************************************
*	                                  
*	ģ������ : ����EEPROM 24xx02����ģ��
*	�ļ����� : bsp_24xx02.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-08-21 armfly  ST�̼���������V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_24C02_H
#define _BSP_24C02_H

#include <inttypes.h>

#define EE_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
#define EE_PAGE_SIZE		8			/* 24xx02��ҳ���С */
#define EE_SIZE				8192			/* 24xx02������ */



extern uint8_t readFlag;// �����ݱ�־  �������ʱ��д����

extern vu8 writeDataFlag;

extern uint8_t ee_CheckOk(void);
extern uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
extern uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
extern uint8_t ee_Read_Byte(uint8_t *_pReadBuf, uint16_t _usAddress);
extern void ee_Write_Byte(uint8_t _pWriteBuf, uint16_t _usAddress);

#endif

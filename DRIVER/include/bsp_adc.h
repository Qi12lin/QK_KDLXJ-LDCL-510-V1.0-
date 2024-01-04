/*
*********************************************************************************************************
*	                                  
*	ģ������ : ADC����ģ��
*	�ļ����� : bsp_adc.h
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

#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include <inttypes.h>

extern uint16_t g_usAdcValue;
extern uint16_t TEMP_VALUE,TEMP_VALUE_COLD,TEMP_VALUE_display;
extern uint16_t roomtempoffset,coldoffset;
extern void ADC_Configuration(void);
extern void AdcPro(void);
extern void Temperature_Calculate(void);
extern uint16_t GetADC(void);
#endif



/*
*********************************************************************************************************
*	                                  
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.h
*	版    本 : V1.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库升级到V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
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



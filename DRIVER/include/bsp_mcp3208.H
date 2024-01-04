/**********************************************************************************************************
*	                                  
*	模块名称 : LED指示灯驱动模块    
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v0.1    2009-12-27 armfly  创建该文件，ST固件库版本为V3.1.2
*		v1.0    2011-01-11 armfly  ST固件库升级到V3.4.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
**********************************************************************************************************/
#include "typedef.h"
#ifndef __BSP_MCP3208_H
#define __BSP_MCP3208_H

#define MCP3208_DOUT_ON()  GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define MCP3208_DOUT_OFF() GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define MCP3208_CS_ON()  GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define MCP3208_CS_OFF() GPIO_ResetBits(GPIOA, GPIO_Pin_15)

#define MCP3208_CLK_ON()  GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define MCP3208_CLK_OFF() GPIO_ResetBits(GPIOB, GPIO_Pin_3)

#define  MCP3208_DIN_READ()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)


/*----- High layer function -----*/
extern	void 	MCP3208_Init(void);
extern	INT16U 	MCP3208_SAMPLE(uint8_t channels);
/*----- Low layer function -----*/
#endif /* SIEMENS_MPI_H */

/*
*********************************************************************************************************
*	                                  
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.h
*	版    本 : V1.0
*	说    明 : 头文件。
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库升级到V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "STM32F4xx.h"
//#include "stm32f10x_type.h"



#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(u8 _ucByte);
u8 i2c_ReadByte(void);
u8 i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
void i2c_CfgGpio(void);
u8 i2c_CheckDevice(u8 _Address);

#endif

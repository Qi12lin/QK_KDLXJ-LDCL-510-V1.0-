/*
*********************************************************************************************************
*	                                  
*	模块名称 : 显示灯驱动模块    
*	文件名称 : bsp_gpio.h
*	版    本 : V2.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "typedef.h"
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#define INPUT_NUMBER 		10//开关量输入的路数
#define INPUT_FILTER_TIME 	10//开关量输入滤波时间，单位ms

#define X0 		0//VK_KEY10//Filter_Data[0]
#define X1 		VK_KEY4//Filter_Data[1]
//#define X2 		Filter_Data[2]
//#define X3 		Filter_Data[3]
//#define X4 		Filter_Data[4]
//#define X5 		Filter_Data[5]
//#define X6 		Filter_Data[6]
//#define X7 		Filter_Data[7]
//#define X8 		Filter_Data[8]
//#define X9 		Filter_Data[9]

//#define Y0			GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_15)
//#define Y0_ON 		GPIO_SetBits(GPIOD, GPIO_Pin_15)
//#define Y0_OFF 		GPIO_ResetBits(GPIOD, GPIO_Pin_15)
//#define Y1			GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_14)
//#define Y1_ON 		GPIO_SetBits(GPIOD, GPIO_Pin_14)
//#define Y1_OFF 		GPIO_ResetBits(GPIOD, GPIO_Pin_14)
//#define Y2			GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13)
//#define Y2_ON 		GPIO_SetBits(GPIOD, GPIO_Pin_13)
//#define Y2_OFF 		GPIO_ResetBits(GPIOD, GPIO_Pin_13)
//#define Y3			GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12)
//#define Y3_ON 		GPIO_SetBits(GPIOD, GPIO_Pin_12)
//#define Y3_OFF 		GPIO_ResetBits(GPIOD, GPIO_Pin_12)
//#define Y4			GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_11)
//#define Y4_ON 		GPIO_SetBits(GPIOD, GPIO_Pin_11)
//#define Y4_OFF 		GPIO_ResetBits(GPIOD, GPIO_Pin_11)
//#define Y5			GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_10)
//#define Y5_ON 		GPIO_SetBits(GPIOD, GPIO_Pin_10)
//#define Y5_OFF 		GPIO_ResetBits(GPIOD, GPIO_Pin_10)

#define Y6			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15)
#define Y6_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_15)
#define Y6_OFF 		GPIO_ResetBits(GPIOE, GPIO_Pin_15)
#define Y7			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14)
#define Y7_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_14)
#define Y7_OFF 		GPIO_ResetBits(GPIOE, GPIO_Pin_14)
#define Y8			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_13)
#define Y8_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_13)
#define Y8_OFF 		GPIO_ResetBits(GPIOE, GPIO_Pin_13)
#define Y9			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_12)
#define Y9_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_12)
#define Y9_OFF 		GPIO_ResetBits(GPIOE, GPIO_Pin_12)
#define Y10			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_11)
#define Y10_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_11)
#define Y10_OFF 	GPIO_ResetBits(GPIOE, GPIO_Pin_11)
#define Y11			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_10)
#define Y11_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_10)
#define Y11_OFF 	GPIO_ResetBits(GPIOE, GPIO_Pin_10)
#define Y12			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_9)
#define Y12_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_9)
#define Y12_OFF 	GPIO_ResetBits(GPIOE, GPIO_Pin_9)

//端口上没有Y14输出，所以所有输出顺延一位
//#define Y14			GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_7)
//#define Y14_ON 		GPIO_SetBits(GPIOE, GPIO_Pin_7)
//#define Y14_OFF 	GPIO_ResetBits(GPIOE, GPIO_Pin_7)

#define Y14			GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2)
#define Y14_ON 		GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define Y14_OFF 	GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define Y15			GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1)
#define Y15_ON 		GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define Y15_OFF 	GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define Y16			GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0)
#define Y16_ON 		GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define Y16_OFF 	GPIO_ResetBits(GPIOB, GPIO_Pin_0)

//#define Y17			GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)
//#define Y17_ON 		GPIO_SetBits(GPIOC, GPIO_Pin_5)
//#define Y17_OFF 	GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define Y18			GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)
#define Y18_ON 		GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define Y18_OFF 	GPIO_ResetBits(GPIOC, GPIO_Pin_4)

#define Y19			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7)
#define Y19_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define Y19_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define Y20			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_6)
#define Y20_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define Y20_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define Y21			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5)
#define Y21_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define Y21_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define Y22			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4)
#define Y22_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define Y22_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define Y23			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3)
#define Y23_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define Y23_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define Y24			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2)
#define Y24_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define Y24_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define Y25			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)
#define Y25_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define Y25_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define Y26			GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)
#define Y26_ON 		GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define Y26_OFF 	GPIO_ResetBits(GPIOA, GPIO_Pin_0)


#define Y0M			(HldReg[MAIN_OUTPUT0] & (1<<0))
#define Y0M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<0))
#define Y0M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<0))
#define Y1M			(HldReg[MAIN_OUTPUT0] & (1<<1)) >> 1
#define Y1M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<1))
#define Y1M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<1))
#define Y2M			(HldReg[MAIN_OUTPUT0] & (1<<2)) >> 2
#define Y2M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<2))
#define Y2M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<2))
#define Y3M			(HldReg[MAIN_OUTPUT0] & (1<<3)) >> 3
#define Y3M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<3))
#define Y3M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<3))
#define Y4M			(HldReg[MAIN_OUTPUT0] & (1<<4)) >> 4
#define Y4M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<4))
#define Y4M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<4))
#define Y5M			(HldReg[MAIN_OUTPUT0] & (1<<5)) >> 5
#define Y5M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<5))
#define Y5M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<5))
#define Y6M			(HldReg[MAIN_OUTPUT0] & (1<<6)) >> 6
#define Y6M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<6))
#define Y6M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<6))
#define Y7M			(HldReg[MAIN_OUTPUT0] & (1<<7)) >> 7
#define Y7M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<7))
#define Y7M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<7))
#define Y8M			(HldReg[MAIN_OUTPUT0] & (1<<8)) >> 8
#define Y8M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<8))
#define Y8M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<8))
#define Y9M			(HldReg[MAIN_OUTPUT0] & (1<<9)) >> 9
#define Y9M_ON 		(HldReg[MAIN_OUTPUT0] |= (1<<9))
#define Y9M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<9))
#define Y10M		(HldReg[MAIN_OUTPUT0] & (1<<10)) >> 10
#define Y10M_ON 	(HldReg[MAIN_OUTPUT0] |= (1<<10))
#define Y10M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<10))
#define Y11M		(HldReg[MAIN_OUTPUT0] & (1<<11)) >> 11
#define Y11M_ON 	(HldReg[MAIN_OUTPUT0] |= (1<<11))
#define Y11M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<11))
#define Y12M		(HldReg[MAIN_OUTPUT0] & (1<<12)) >> 12
#define Y12M_ON 	(HldReg[MAIN_OUTPUT0] |= (1<<12))
#define Y12M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<12))
#define Y13M		(HldReg[MAIN_OUTPUT0] & (1<<13)) >> 13
#define Y13M_ON 	(HldReg[MAIN_OUTPUT0] |= (1<<13))
#define Y13M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<13))
#define Y14M		(HldReg[MAIN_OUTPUT0] & (1<<14)) >> 14
#define Y14M_ON 	(HldReg[MAIN_OUTPUT0] |= (1<<14))
#define Y14M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<14))
#define Y15M		(HldReg[MAIN_OUTPUT0] & (1<<15)) >> 15
#define Y15M_ON 	(HldReg[MAIN_OUTPUT0] |= (1<<15))
#define Y15M_OFF 	(HldReg[MAIN_OUTPUT0] &= ~(1<<15))

extern uint8_t Filter_Data[INPUT_NUMBER];
void bsp_InitInput(void);
void bsp_InitOutput(void);
void bsp_InputFilter(void);

#endif

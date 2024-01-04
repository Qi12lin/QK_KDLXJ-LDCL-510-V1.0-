/*
*********************************************************************************************************
*	                                  
*	ģ������ : IO������ģ��    
*	�ļ����� : bsp_gpio.c
*	��    �� : V1.0
*	˵    �� : ʵ��IO�ڵĳ�ʼ��
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v0.1    2009-12-27 armfly  �������ļ���ST�̼���汾ΪV3.1.2

*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

//#include "stm32f10x.h"
#include "typedef.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitInput
*	����˵��: ��ʼ������������Ӳ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitInput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ��GPIO������ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	/* ��I7(PD6)��I8(PD7)��I9(PD8)����Ϊ��������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������ģʽ	GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	/* ��I0(PC12)����Ϊ��������ģʽ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������ģʽ	GPIO_PuPd_UP;//����
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	/* ��I1(PD0)��I2(PD1)��I3(PD2)��I4(PD3)��I5(PD4)��I6(PD7)����Ϊ��������ģʽ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������ģʽ	GPIO_PuPd_UP;//����
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitOutput
*	����˵��: ��ʼ�����������Ӳ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOB��GPIOC��GPIOF��GPIOG������ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

	/* ��Q27(PA0),Q26(PA1),Q25(PA2),Q24(PA3),Q23(PA4),Q22(PA5),Q21(PA6),Q20(PA7)����Ϊ�������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	/* ��Q17(PB0),Q16(PB1),Q15(PB2)����Ϊ�������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ��Q19(PC4)��Q18(PC5)����Ϊ�������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	/* ��Q5(PD10)��Q4(PD11)��Q3(PD12)��Q2(PD13)��Q1(PD14)��Q0(PD15)����Ϊ�������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	

	/* ��Q14(PE7),Q13(PE8),Q12(PE9),Q11(PE10),Q10(PE11),Q9(PE12),Q8(PE13),Q7(PE14),Q6(PE15)����Ϊ�������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InputFilter
*	����˵��: �����������˲�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t Filter_Data[INPUT_NUMBER];
uint8_t Input_Data[INPUT_NUMBER];
uint8_t Input_DataTemp[INPUT_NUMBER];
uint8_t Input_FilterCount[INPUT_NUMBER];
uint8_t j;

void bsp_InputFilter(void)
{
	Input_Data[0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
	Input_Data[1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
	Input_Data[2] = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	Input_Data[3] = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	Input_Data[4] = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
	Input_Data[5] = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4);//X5
	Input_Data[6] = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7);	
	Input_Data[7] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	Input_Data[8] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	Input_Data[9] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);	
	
	for(j = 0; j < INPUT_NUMBER; j++)/* ����ڸ��� */
	{
		if(Input_Data[j] == Input_DataTemp[j]) 
		{
			Input_FilterCount[j]++;
			
			if(Input_FilterCount[j] >= INPUT_FILTER_TIME)				
				Filter_Data[j] = Input_Data[j];
			else{}
		}
		else 
			Input_FilterCount[j] = 0;

		Input_DataTemp[j] = Input_Data[j];
	}
}

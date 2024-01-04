/*
*********************************************************************************************************
*	                                  
*	ģ������ : ADC����ģ��
*	�ļ����� : bsp_adc.c
*	��    �� : V1.0
*	˵    �� : ʵ��adc�����ò�����
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-12-31   ����  ST�̼���������V3.5.0�汾��
*
*	Copyright (C), 2012-2013,��ɳ���Ƶ���
*
*********************************************************************************************************
*/
#include "bsp_adc.h"
#include "typedef.h"
#define SAMP_COUNT	32		/* ������������ʾ��λʱ���ڵĲ���������ƽ��ֵ */

/*
*********************************************************************************************************
*	�� �� ��: ADC_Configuration
*	����˵��: ����ADC, PC4ģʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

  /* ʹ��ADC1��GPIOCʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��	
	
  //�ȳ�ʼ��ADC1ͨ��10 IO��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PC0 ͨ��10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
}
/*
*********************************************************************************************************
*	�� �� ��: GetADC
*	����˵��: ��ȡADC������ƽ��ֵ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t GetADC(void)
{
	uint16_t ret;

	/* ��Ϊ	g_AdcValue ������systick�ж��и�д��Ϊ�˱��������������ʱ���жϳ�����ҵ������ݴ��������Ҫ
	�ر��жϽ��б��� */

	/* �����ٽ����������ر��ж� */
	__set_PRIMASK(1);  /* ���ж� */

	ret = g_usAdcValue;

	__set_PRIMASK(0);  /* ���ж� */

	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: AdcPro
*	����˵��: ADC����������ʱ���е���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t g_usAdcValue;	/* ADC ����ֵ��ƽ��ֵ */
uint16_t roomtempoffset,coldoffset;
void AdcPro(void)
{
	static uint16_t buf[SAMP_COUNT+5];
	static uint8_t write = 0;
	uint32_t sum = 0;
	uint8_t i;

	buf[write] = ADC_GetConversionValue(ADC1);
	
	if((write + 1) >= SAMP_COUNT)
	{
	    write = 0;
	   	sum = 0;
		for (i = 0; i < SAMP_COUNT; i++)
		{
			sum += buf[i];
		}
		g_usAdcValue = sum / SAMP_COUNT;	/* ADC����ֵ�����ɴβ���ֵƽ�� */
	}
	else
	{
		++write;
	}
	ADC_SoftwareStartConv(ADC1);	/* ��������´�ADCת�� */
}

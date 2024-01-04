//#include "stm8s_lib.h"
#include "typedef.h"
//#include "bsp_ds1302.h"	
//#include "MODBUS_RTU.H"
//#include "app_temp.h"
//#include "PID.h"
void RCC_Configuration(void)
{   
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
}

//RTC��ʼ��
//extern void RTC_Init(void);

//��Դ��λ��ʼ��
void PwrInit(void)
{
//	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//	 PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
//	 PWR_PVDCmd(ENABLE);
}
//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	
	IWDG_SetPrescaler(prer); //����IWDG��Ƶϵ��

	IWDG_SetReload(rlr);   //����IWDGװ��ֵ

	IWDG_ReloadCounter(); //reload
	
//	IWDG_Enable();       //ʹ�ܿ��Ź�
}

void ExtiInit(void)//�ⲿ�жϳ�ʼ������
{
// 	EXTI_InitTypeDef EXTI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��GPIOA������ʱ�� */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��

	
	/* ��I3(PG0)����Ϊ��������ģʽ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOG, &GPIO_InitStructure);	
//	
//    //GPIOA.0 �ж����Լ��жϳ�ʼ������
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource0);

//  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//������	EXTI_Trigger_Falling;//�½��ش���
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���	
}

/*********************************
���ƣ�DriverInit
���ܣ���ʼ�����е�����
��ڲ�������
���ز�������
*********************************/
void DriverInit(void)
{
//	RCC_Configuration();
//	PwrInit();
	IWDG_Init(4,3000); //���Ƶ��Ϊ64,����ֵΪ1000,���ʱ��Ϊ2s��	
	BeepInit();//������  
  MPU_Init();// ��ʼ��MPU6050
 	//bsp_InitInput();
	bsp_InitOutput();	
//	ExtiInit();//�ⲿ�ж�
	bsp_InitTimer();	
	ee_CheckOk();//EEPROMIO_Init();
	ADC_Configuration();//ADC_Init
//	UART_Init(&UART_1);//UART1Init();	
//	UART_Init(&UART_2);//UART2Init();
	UART_Init(&UART_3);//UART3Init();	
	UART_Init(&UART_4);//UART4Init();
	UART_Init(&UART_6);//UART4Init();	
	
	W25QXX_Init();//SPI FLASH��ʼ��
	MCP3208_Init();
	NVIC_Configuration();
	
//	TM1638_IO_Init();
	keyInit();
//	DS1302_Init();	
My_RTC_Init();		 		//��ʼ��RTC
}

//ϵͳ�жϹ���
void NVIC_Configuration(void)
{ 
  	NVIC_InitTypeDef NVIC_InitStructure;

//  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   	

	/* Configure the NVIC Preemption Priority Bits */  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		// 1bit(2preemption):3bit(8sub)
	  
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ���ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1�� 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�0
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure); 

//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	

//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	
	
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}



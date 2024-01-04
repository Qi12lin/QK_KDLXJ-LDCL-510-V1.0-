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

//RTC初始化
//extern void RTC_Init(void);

//电源复位初始化
void PwrInit(void)
{
//	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//	 PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
//	 PWR_PVDCmd(ENABLE);
}
//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//rlr:自动重装载值,0~0XFFF.
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
	
	IWDG_SetPrescaler(prer); //设置IWDG分频系数

	IWDG_SetReload(rlr);   //设置IWDG装载值

	IWDG_ReloadCounter(); //reload
	
//	IWDG_Enable();       //使能看门狗
}

void ExtiInit(void)//外部中断初始化函数
{
// 	EXTI_InitTypeDef EXTI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 打开GPIOA部件的时钟 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);//外部中断，需要使能AFIO时钟

	
	/* 将I3(PG0)配置为浮动输入模式 */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOG, &GPIO_InitStructure);	
//	
//    //GPIOA.0 中断线以及中断初始化配置
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource0);

//  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿	EXTI_Trigger_Falling;//下降沿触发
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
}

/*********************************
名称：DriverInit
功能：初始化所有的驱动
入口参数：无
返回参数：无
*********************************/
void DriverInit(void)
{
//	RCC_Configuration();
//	PwrInit();
	IWDG_Init(4,3000); //与分频数为64,重载值为1000,溢出时间为2s。	
	BeepInit();//蜂鸣器  
  MPU_Init();// 初始化MPU6050
 	//bsp_InitInput();
	bsp_InitOutput();	
//	ExtiInit();//外部中断
	bsp_InitTimer();	
	ee_CheckOk();//EEPROMIO_Init();
	ADC_Configuration();//ADC_Init
//	UART_Init(&UART_1);//UART1Init();	
//	UART_Init(&UART_2);//UART2Init();
	UART_Init(&UART_3);//UART3Init();	
	UART_Init(&UART_4);//UART4Init();
	UART_Init(&UART_6);//UART4Init();	
	
	W25QXX_Init();//SPI FLASH初始化
	MCP3208_Init();
	NVIC_Configuration();
	
//	TM1638_IO_Init();
	keyInit();
//	DS1302_Init();	
My_RTC_Init();		 		//初始化RTC
}

//系统中断管理
void NVIC_Configuration(void)
{ 
  	NVIC_InitTypeDef NVIC_InitStructure;

//  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   	

	/* Configure the NVIC Preemption Priority Bits */  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		// 1bit(2preemption):3bit(8sub)
	  
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级1， 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级0
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
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



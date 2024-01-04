#include "typedef.h"
//#include "bsp_timer.h"
//#include "app_start_proc.h"
//#include "app_alarm_record.h"
//#include "PID.h"
/////////////////////////////////////////////
__IO int32_t g_iRunTime = 0;

/* 目前是空操作，用户可以定义让CPU进入IDLE状态的函数和喂狗函数 */
#define CPU_IDLE()		 
TMR_10MS  time_10ms[NO_10MS];
TMR_100MS time_100ms[NO_100MS];
TMR_1S    time_1s[NO_1S];// __attribute__((at(0X2000DF00)));
uint8_t flag_10ms,flag_100ms, flag_1s;
uint8_t flag_20ms,flag_200ms, flag_2s;
INT16U	Timer_1MS[TIMER_1MS_MAX_NUM];
INT16U	Timer_100MS[TIMER_100MS_MAX_NUM];
INT16U	Timer_1S[TIMER_1S_MAX_NUM];
uint32_t Timer_1MS_32[TIMER_1MS_32_MAX_NUM];
extern  u16 T_display1,T_display;
u16 T_flicker;
u8 short_key_time,short_key_flag;

u32 bsp_GetRunTime(void)
{
	int runtime; 

	__set_PRIMASK(1);  		/* 关中断 */
	
	runtime = g_iRunTime;	/* 由于在Systick中断被改写，因此关中断进行保护 */
		
	__set_PRIMASK(0);  		/* 开中断 */

	return runtime;
}
/**********************************************************************************************************
*	函 数 名: Delay_us(u32 nus)
*	功能说明: us延时
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void Delay_us(u32 nus)
{
	u8 j;
	u32 i;
	for (i = 0; i < nus; i++)
		for (j = 0; j < 29; j++);
}
/**********************************************************************************************************
*	函 数 名: Delay_ms(u32 nus)
*	功能说明: us延时
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void Delay_ms(u32 nms)
{
	u16 j;
	u32 i;
	for (i = 0; i < nms; i++)
		for (j = 0; j < 29000; j++);
}
//--------------------------------------------------------------------
//定时器2初始化
//--------------------------------------------------------------------
void TIM2_Init(void)//分段点胶用，周期500us
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
	TIM_TimeBaseStructure.TIM_Period = (500-1);//

	// 这个就是预分频系数，当由于为0时表示不分频所以要减1
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);		  //1ms 

	// 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
	// 使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //向上计数
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//初始化定时器2
	
	/* Clear TIMx update pending flag[清除TIM5溢出中断标志] */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	/* TIMx IT enable */ //打开溢出中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* TIMx enable counter */
	TIM_Cmd(TIM2, ENABLE);  //计数器使能，开始工作
}
//--------------------------------------------------------------------
//定时器3初始化
//--------------------------------------------------------------------
void TIM3_Init(void)	//脉冲输出
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructureCP1;	
//	TIM_DeInit(TIM3);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//	/* 配置(TIM3CH4、PB1)为复用功能推挽输出模式 */		   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	
//	//这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
////	// 这个就是预分频系数，当由于为0时表示不分频所以要减1
//	TIM_TimeBaseStructure.TIM_Period = (1000);//
//	// 这个就是预分频系数，当由于为0时表示不分频所以要减1
////	TIM_TimeBaseStructure.TIM_Prescaler = (720 - 1);		  //1ms 100次
//	TIM_TimeBaseStructure.TIM_Prescaler = (360 - 1);		  //1ms 200次
//	// 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
//	// 使用的采样频率之间的分频比例
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //向上计数
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	//初始化定时器3
//	
//	TIM_OCInitStructureCP1.TIM_OCMode = TIM_OCMode_PWM1;	
//	TIM_OCInitStructureCP1.TIM_Pulse = 500;
//	TIM_OCInitStructureCP1.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low;//
//	TIM_OC4Init(TIM3, &TIM_OCInitStructureCP1);
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM3, ENABLE);	
//	
//	/* Clear TIM5 update pending flag[清除TIM5溢出中断标志] */
//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

//	/* TIM IT enable */ //打开溢出中断
////	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	/* TIM3 enable counter */
////	TIM_Cmd(TIM3, ENABLE);  //计数器使能，开始工作	
}
//--------------------------------------------------------------------
//定时器4初始化
//--------------------------------------------------------------------
void TIM4_Init(void)//计数用，25us
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
	TIM_TimeBaseStructure.TIM_Period = (25-1);//40k  (166 - 1);//6k

	// 这个就是预分频系数，当由于为0时表示不分频所以要减1
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);		  //1ms 10次

	// 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
	// 使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //向上计数
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//初始化定时器
	
	/* Clear TIMx update pending flag[清除TIMx溢出中断标志] */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	
	/* TIMx IT enable */ //打开溢出中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/* TIMx enable counter */
	TIM_Cmd(TIM4, ENABLE);  //计数器使能，开始工作
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: ms级延迟，延迟精度为正负1ms
*	形    参：n : 延迟长度，单位1 ms。 n 应大于2
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(uint16_t len)
{
	Timer_1MS[0] = len;
	while (Timer_1MS[0])
	{
		CPU_IDLE();	/* 此处是空操作。用户可以定义，让CPU进入IDLE状态，以降低功耗；或实现喂狗 */
	}
}
/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔1ms进入1次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
u16 disp_delay,X3_counter=0;
u16 glue_run_pulse_c1=0,glue_stop_pulse_c1=0,glue_run_pulse_c2=0,glue_stop_pulse_c2=0;
u8 glue_run_flag1=0,glue_run_flag2=0;
uint16_t count_10ms,count_100ms, count_1s;
uint16_t count_20ms,count_200ms, count_2s;
uint8_t glue1_count=0,glue2_count=0;
u8 sys_startrun =0;
u8 sys_setRunPage =5;
extern int set5;  //控制温度
void SysTick_ISR(void)
{
	u8 i;
//	INT32U a32;

	g_iRunTime++;	/* 全局运行时间每1ms增1 */	
	if (g_iRunTime >= 0x7FFFFFFF) g_iRunTime = 0;
	else {}	
	
	for(i=0;i<TIMER_1MS_MAX_NUM;i++)//1ms定时
	{	
		if(Timer_1MS[i]>0) Timer_1MS[i]--;
		else {}
	}
//	for(i=0;i<TIMER_1MS_32_MAX_NUM;i++)//1ms定时
//	{	
//		if(Timer_1MS_32[i]>0) Timer_1MS_32[i]--;
//		else {}
//	}	
//	if(ENTERKey_delay >0) ENTERKey_delay--;
	disp_delay ++;	
	if(count_10ms >= 10){count_10ms = 0;flag_10ms = 1;}/* 10ms定时 */
	else count_10ms ++;	
	if(count_20ms >= 20){count_20ms = 0;flag_20ms = 1;}/* 20ms定时 */
	else count_20ms ++;		
	if(count_100ms >= 100)
	{	
		count_100ms = 0;flag_100ms = 1;
		if(Y4 ==0 )		
		{
			if(Y4_OFF_time < 60000) Y4_OFF_time++;		//20
		}
		else if(Y4_ON_time <60000) Y4_ON_time++;
		
//		if(short_key_flag)
//		{
//			if(short_key_time <100) short_key_time++;
//			if(short_key_time >30) runFlag.shut_run =1;  //点动标志
//		}
//		else short_key_time =0;
		//if(Y4 ==0 && temper.realTemp >(set5-20) )		 Y4_OFF_time++;		//20  -LXY
}/* 100ms定时 */
	else count_100ms ++;	
	if(count_200ms >= 200){	count_200ms = 0;flag_200ms = 1;}/* 200ms定时 */
	else count_200ms ++;		
	if(count_1s >= 1000)
	{
		count_1s = 0;flag_1s = 1;
		
	if(sys_setRunPage >0) sys_setRunPage--;
	if(sys_setRunPage <3 && sys_setRunPage>0 && HldReg[SET_STATRPAGE] ==2) HldReg[CHANGE_PAGE] = 1;

	}/* 1s定时 */
	else count_1s ++;	
	if(count_2s >= 2000)
	{
		count_2s = 0;flag_2s = 1;sys_startrun =1;
	}/* 2s定时 */
	else count_2s ++;	
	
	Counter_200ms();
	AdcPro();
	key_filter();
//------------------------------------------
// 功能控制
//------------------------------------------
	if (T_flicker >= FLICKER) T_flicker = 0;
	else T_flicker ++;	
//	UART_1.TimeoutReg++;
//	UART_2.TimeoutReg++;	
	UART_3.TimeoutReg++;//RS485通信
	UART_4.TimeoutReg++;	
	UART_6.TimeoutReg++;		
	Beep_RecordTimer();	 //蜂鸣器计时器
//	bsp_InputFilter();	//开关量输入滤波
	runTimeContor();
	
}


//定时器中断服务程序
void TIME2_ISR(void)//1000us执行一次
{
	 
}
//定时器初始化
void bsp_InitTimer(void)
{
//	TIM2_Init();
//	TIM3_Init();//PUR机才初始化
//	TIM4_Init();
	SysTick_Config(SystemCoreClock / 1000);
}
//定时器中断服务程序
uint32_t SSR8_ration_pulse;
void TIME3_ISR(void)
{

}
//定时器中断服务程序
void TIME4_ISR(void)
{

}
/**********************************************************************************************************
*	函 数 名: Bsp_Time_10ms
*	功能说明: 
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
**********************************************************************************************************/
static void Bsp_Time_10ms(TMR_10MS *_tmr)
{
	if(_tmr->enable == 1)
	{	
		if(_tmr->count >= _tmr->number) 
			_tmr->flag = 1;
		else  
			_tmr->count++;
	}
	else
	{
		_tmr->count = 0;
		_tmr->flag = 0;
	}	
}
/**********************************************************************************************************
*	函 数 名: Bsp_Control_10ms
*	功能说明: 10ms控制函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_Control_10ms(void)
{
	uint8_t i;
	if(flag_10ms == 1)
	{
		flag_10ms = 0;

		for (i = 0; i < NO_10MS; i++)
			Bsp_Time_10ms(&time_10ms[i]);


	}
	else {}
}
/**********************************************************************************************************
*	函 数 名: Bsp_Time_100ms
*	功能说明: 
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void Bsp_Time_100ms(TMR_100MS *_tmr)
{
	if(_tmr->enable == 1)
	{	
		if(_tmr->count >= _tmr->number) 
			_tmr->flag = 1;
		else  
			_tmr->count++;
	}
	else
	{
		_tmr->count = 0;
		_tmr->flag = 0;
	}	
}
/**********************************************************************************************************
*	函 数 名: Bsp_Control_100ms
*	功能说明: 100ms控制函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_Control_100ms(void)
{
	uint8_t i;

	if(flag_100ms == 1)
	{
		flag_100ms = 0;

		for (i = 0; i < NO_100MS; i++)
			Bsp_Time_100ms(&time_100ms[i]);
		for(i=0;i<TIMER_100MS_MAX_NUM;i++)//递减定时器
		{
			if(Timer_100MS[i]>0) Timer_100MS[i]--;
			else {}
		}			
	}
	else{}
}
/**********************************************************************************************************
*	函 数 名: Bsp_Control_500ms
*	功能说明: 500ms控制函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_Control_200ms(void)
{

	static u8 flag_R =1;
	if(flag_200ms == 1)
	{
		flag_200ms = 0;
			if(readFlag ==0)   
			{
				Save_eepromData();
			}
			//读取时钟
			if(flag_R)
			{
				RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
				RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
		//	flag_R=0;
			}
	}
	else{}
}
/**********************************************************************************************************
*	函 数 名: Bsp_Time_1s
*	功能说明: 
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
**********************************************************************************************************/
static void Bsp_Time_1s(TMR_1S *_tmr)
{
	if(_tmr->enable == 1)
	{	
		if(_tmr->count >= _tmr->number) 
			_tmr->flag = 1;
		else  
			_tmr->count++;
	}
	else
	{
		_tmr->count = 0;
		_tmr->flag = 0;
	}	
}

/**********************************************************************************************************
*	函 数 名: Bsp_Control_1s
*	功能说明: 100ms控制函数
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/

void Bsp_Control_1s(void)
{
	u16 i;
//	INT16U PM;
	u8 j=0,k=0;	
	static u16 AnalogDisplay[4][8]={0};
	uint16_t PM,sum=0;
//	static uint16_t roomtempoffsettime = 0;//
	
	if(flag_1s == 1)
	{
		flag_1s = 0;
		for (i = 0; i < NO_1S; i++) Bsp_Time_1s(&time_1s[i]);//带标志定时器
		for(i=0;i<TIMER_1S_MAX_NUM;i++)//递减定时器
		{
			if(Timer_1S[i]>0) Timer_1S[i]--;
			else {}
		}		
			
//		ds1302_read_time();//读取当前时钟
//		read_time_flag = 1;//读取触摸屏时间
//		Save_Date();//保存当前日期
		
	
	}
	else{}
}
/**********************************************************************************************************
*	函 数 名: Bsp_Control_2s
*	功能说明: 2s控制函数
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/

//void Bsp_Control_2s(void)
//{	
//	if(flag_2s == 1)
//	{
//		flag_2s = 0;			
//	}
//	else{}
//}

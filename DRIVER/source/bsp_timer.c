#include "typedef.h"
//#include "bsp_timer.h"
//#include "app_start_proc.h"
//#include "app_alarm_record.h"
//#include "PID.h"
/////////////////////////////////////////////
__IO int32_t g_iRunTime = 0;

/* Ŀǰ�ǿղ������û����Զ�����CPU����IDLE״̬�ĺ�����ι������ */
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

	__set_PRIMASK(1);  		/* ���ж� */
	
	runtime = g_iRunTime;	/* ������Systick�жϱ���д����˹��жϽ��б��� */
		
	__set_PRIMASK(0);  		/* ���ж� */

	return runtime;
}
/**********************************************************************************************************
*	�� �� ��: Delay_us(u32 nus)
*	����˵��: us��ʱ
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/
void Delay_us(u32 nus)
{
	u8 j;
	u32 i;
	for (i = 0; i < nus; i++)
		for (j = 0; j < 29; j++);
}
/**********************************************************************************************************
*	�� �� ��: Delay_ms(u32 nus)
*	����˵��: us��ʱ
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/
void Delay_ms(u32 nms)
{
	u16 j;
	u32 i;
	for (i = 0; i < nms; i++)
		for (j = 0; j < 29000; j++);
}
//--------------------------------------------------------------------
//��ʱ��2��ʼ��
//--------------------------------------------------------------------
void TIM2_Init(void)//�ֶε㽺�ã�����500us
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999
	TIM_TimeBaseStructure.TIM_Period = (500-1);//

	// �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);		  //1ms 

	// �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //���ϼ���
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//��ʼ����ʱ��2
	
	/* Clear TIMx update pending flag[���TIM5����жϱ�־] */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	/* TIMx IT enable */ //������ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* TIMx enable counter */
	TIM_Cmd(TIM2, ENABLE);  //������ʹ�ܣ���ʼ����
}
//--------------------------------------------------------------------
//��ʱ��3��ʼ��
//--------------------------------------------------------------------
void TIM3_Init(void)	//�������
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructureCP1;	
//	TIM_DeInit(TIM3);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//	/* ����(TIM3CH4��PB1)Ϊ���ù����������ģʽ */		   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	
//	//��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999
////	// �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
//	TIM_TimeBaseStructure.TIM_Period = (1000);//
//	// �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
////	TIM_TimeBaseStructure.TIM_Prescaler = (720 - 1);		  //1ms 100��
//	TIM_TimeBaseStructure.TIM_Prescaler = (360 - 1);		  //1ms 200��
//	// �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
//	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //���ϼ���
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	//��ʼ����ʱ��3
//	
//	TIM_OCInitStructureCP1.TIM_OCMode = TIM_OCMode_PWM1;	
//	TIM_OCInitStructureCP1.TIM_Pulse = 500;
//	TIM_OCInitStructureCP1.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low;//
//	TIM_OC4Init(TIM3, &TIM_OCInitStructureCP1);
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM3, ENABLE);	
//	
//	/* Clear TIM5 update pending flag[���TIM5����жϱ�־] */
//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

//	/* TIM IT enable */ //������ж�
////	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	/* TIM3 enable counter */
////	TIM_Cmd(TIM3, ENABLE);  //������ʹ�ܣ���ʼ����	
}
//--------------------------------------------------------------------
//��ʱ��4��ʼ��
//--------------------------------------------------------------------
void TIM4_Init(void)//�����ã�25us
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999
	TIM_TimeBaseStructure.TIM_Period = (25-1);//40k  (166 - 1);//6k

	// �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);		  //1ms 10��

	// �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //���ϼ���
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//��ʼ����ʱ��
	
	/* Clear TIMx update pending flag[���TIMx����жϱ�־] */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	
	/* TIMx IT enable */ //������ж�
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/* TIMx enable counter */
	TIM_Cmd(TIM4, ENABLE);  //������ʹ�ܣ���ʼ����
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayMS
*	����˵��: ms���ӳ٣��ӳپ���Ϊ����1ms
*	��    �Σ�n : �ӳٳ��ȣ���λ1 ms�� n Ӧ����2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayMS(uint16_t len)
{
	Timer_1MS[0] = len;
	while (Timer_1MS[0])
	{
		CPU_IDLE();	/* �˴��ǿղ������û����Զ��壬��CPU����IDLE״̬���Խ��͹��ģ���ʵ��ι�� */
	}
}
/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��1ms����1��
*	��    �Σ���
*	�� �� ֵ: ��
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
extern int set5;  //�����¶�
void SysTick_ISR(void)
{
	u8 i;
//	INT32U a32;

	g_iRunTime++;	/* ȫ������ʱ��ÿ1ms��1 */	
	if (g_iRunTime >= 0x7FFFFFFF) g_iRunTime = 0;
	else {}	
	
	for(i=0;i<TIMER_1MS_MAX_NUM;i++)//1ms��ʱ
	{	
		if(Timer_1MS[i]>0) Timer_1MS[i]--;
		else {}
	}
//	for(i=0;i<TIMER_1MS_32_MAX_NUM;i++)//1ms��ʱ
//	{	
//		if(Timer_1MS_32[i]>0) Timer_1MS_32[i]--;
//		else {}
//	}	
//	if(ENTERKey_delay >0) ENTERKey_delay--;
	disp_delay ++;	
	if(count_10ms >= 10){count_10ms = 0;flag_10ms = 1;}/* 10ms��ʱ */
	else count_10ms ++;	
	if(count_20ms >= 20){count_20ms = 0;flag_20ms = 1;}/* 20ms��ʱ */
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
//			if(short_key_time >30) runFlag.shut_run =1;  //�㶯��־
//		}
//		else short_key_time =0;
		//if(Y4 ==0 && temper.realTemp >(set5-20) )		 Y4_OFF_time++;		//20  -LXY
}/* 100ms��ʱ */
	else count_100ms ++;	
	if(count_200ms >= 200){	count_200ms = 0;flag_200ms = 1;}/* 200ms��ʱ */
	else count_200ms ++;		
	if(count_1s >= 1000)
	{
		count_1s = 0;flag_1s = 1;
		
	if(sys_setRunPage >0) sys_setRunPage--;
	if(sys_setRunPage <3 && sys_setRunPage>0 && HldReg[SET_STATRPAGE] ==2) HldReg[CHANGE_PAGE] = 1;

	}/* 1s��ʱ */
	else count_1s ++;	
	if(count_2s >= 2000)
	{
		count_2s = 0;flag_2s = 1;sys_startrun =1;
	}/* 2s��ʱ */
	else count_2s ++;	
	
	Counter_200ms();
	AdcPro();
	key_filter();
//------------------------------------------
// ���ܿ���
//------------------------------------------
	if (T_flicker >= FLICKER) T_flicker = 0;
	else T_flicker ++;	
//	UART_1.TimeoutReg++;
//	UART_2.TimeoutReg++;	
	UART_3.TimeoutReg++;//RS485ͨ��
	UART_4.TimeoutReg++;	
	UART_6.TimeoutReg++;		
	Beep_RecordTimer();	 //��������ʱ��
//	bsp_InputFilter();	//�����������˲�
	runTimeContor();
	
}


//��ʱ���жϷ������
void TIME2_ISR(void)//1000usִ��һ��
{
	 
}
//��ʱ����ʼ��
void bsp_InitTimer(void)
{
//	TIM2_Init();
//	TIM3_Init();//PUR���ų�ʼ��
//	TIM4_Init();
	SysTick_Config(SystemCoreClock / 1000);
}
//��ʱ���жϷ������
uint32_t SSR8_ration_pulse;
void TIME3_ISR(void)
{

}
//��ʱ���жϷ������
void TIME4_ISR(void)
{

}
/**********************************************************************************************************
*	�� �� ��: Bsp_Time_10ms
*	����˵��: 
*	��    �Σ�_tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: Bsp_Control_10ms
*	����˵��: 10ms���ƺ���
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: Bsp_Time_100ms
*	����˵��: 
*	��    �Σ�_tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: Bsp_Control_100ms
*	����˵��: 100ms���ƺ���
*	��    �Σ���
*	�� �� ֵ: ��
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
		for(i=0;i<TIMER_100MS_MAX_NUM;i++)//�ݼ���ʱ��
		{
			if(Timer_100MS[i]>0) Timer_100MS[i]--;
			else {}
		}			
	}
	else{}
}
/**********************************************************************************************************
*	�� �� ��: Bsp_Control_500ms
*	����˵��: 500ms���ƺ���
*	��    �Σ���
*	�� �� ֵ: ��
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
			//��ȡʱ��
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
*	�� �� ��: Bsp_Time_1s
*	����˵��: 
*	��    �Σ�_tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: Bsp_Control_1s
*	����˵��: 100ms���ƺ���
*	��    �Σ���
*	�� �� ֵ: ��
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
		for (i = 0; i < NO_1S; i++) Bsp_Time_1s(&time_1s[i]);//����־��ʱ��
		for(i=0;i<TIMER_1S_MAX_NUM;i++)//�ݼ���ʱ��
		{
			if(Timer_1S[i]>0) Timer_1S[i]--;
			else {}
		}		
			
//		ds1302_read_time();//��ȡ��ǰʱ��
//		read_time_flag = 1;//��ȡ������ʱ��
//		Save_Date();//���浱ǰ����
		
	
	}
	else{}
}
/**********************************************************************************************************
*	�� �� ��: Bsp_Control_2s
*	����˵��: 2s���ƺ���
*	��    �Σ���
*	�� �� ֵ: ��
**********************************************************************************************************/

//void Bsp_Control_2s(void)
//{	
//	if(flag_2s == 1)
//	{
//		flag_2s = 0;			
//	}
//	else{}
//}

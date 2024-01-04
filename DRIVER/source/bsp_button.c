#include "typedef.h"

//独立按键
//键盘IO端口

#define KEY0_PORT     GPIOB
#define KEY1_PORT  	  GPIOB



//键盘IO引脚
#define KEY0_PIN      GPIO_Pin_0
#define KEY1_PIN      GPIO_Pin_1


#define GPIO_ReadInputPin GPIO_ReadInputDataBit

vu32 keyValueTab=0,keyValueTab1=0; //按键信息表
static Key_Scan keyScanInfo;

/////////////////////////////////////////////////
//按键初始化
void keyInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	安富莱STM32F103ZE-EK 按键口线分配：
	USER键     : PG8  (低电平表示按下)
	TAMPEER键  : PC13 (低电平表示按下)
	WKUP键     : PA0  (!!!高电平表示按下)
	摇杆UP键   : PG15 (低电平表示按下)
	摇杆DOWN键 : PD3  (低电平表示按下)
	摇杆LEFT键 : PG14 (低电平表示按下)
	摇杆RIGHT键: PG13 (低电平表示按下)
	摇杆OK键   : PG7 (低电平表示按下)
	*/
			
	/* 第1步：打开GPIOA GPIOC GPIOD GPIOF GPIOG的时钟
	   注意：这个地方可以一次性全打开
	*/
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = KEY0_PIN;
	GPIO_Init(KEY0_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_Init(KEY1_PORT, &GPIO_InitStructure);	
	

//	GPIO_InitStructure.GPIO_Pin = KEY8_PIN;
//	GPIO_Init(KEY8_PORT, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = KEY9_PIN;
//	GPIO_Init(KEY9_PORT, &GPIO_InitStructure);							
	keyScanInfo.key_msg = 0;
	keyScanInfo.old_key_msg = keyScanInfo.key_msg;
}


/*---------------------------------------
名    称：kbhit
功    能：判断是否有键按下
入口参数：无
返回参数：非0 有键按下 | 为0 没有键按下
备    注：还可以获取组合键值
-----------------------------------------*/
u32 kbhit(void)
{
	return (keyValueTab);
}

/*---------------------------------------
名    称：getKeyState
功    能：判断某个按键的状态
入口参数：vkKey 的值为VK_KeyType类型
		  按键对应的虚拟键码
返回参数：相应按键状态：1 按下 | 0 没有
备    注：还可以获取组合键值
-----------------------------------------*/
//u32 getKeyState(u32 vkKey)
//{
//	if(vkKey == VK_ALL) return ((keyValueTab1 & vkKey)||(keyValueTab & vkKey));
//	else if(vkKey < 0x10)
//	return (keyValueTab & vkKey);
//	else return ((keyValueTab1 & (vkKey>>4)) <<4);
//}


/*---------------------------------------
名    称：getKey
功    能：获取按下的键值
入口参数：无
返回参数：虚拟键值VK_KeyType
备    注：阻塞方式获取按键（只有获取到键值才能退出）
-----------------------------------------*/
u32 getKey(void)
{
	u16 i;
	for( i = 0;  ; )
	{
		if(keyValueTab & (0x01 << i))
		{
			return (0x01 << i);
		}
		i++;
		if(i == KEY_NUM) i = 0;
	}
}



//获取按键相应IO状态
//矩阵按键和独立按键复用
static void getKeyIOState(void)
{
	keyScanInfo.key_msg = 0;
	if(GPIO_ReadInputPin(KEY0_PORT,KEY0_PIN) == 0)
	{
		keyScanInfo.key_msg |= (0x01 << 0);
	}
	if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN) == 0)
	{
		keyScanInfo.key_msg |= (0x01 << 1);
	}

	
//	if(GPIO_ReadInputPin(KEY8_PORT,KEY8_PIN) == 0)
//	{
//		keyScanInfo.key_msg |= (0x01 << 8);
//	}
//	if(GPIO_ReadInputPin(KEY9_PORT,KEY9_PIN) == 0)
//	{
//		keyScanInfo.key_msg |= (0x01 << 9);
//	}
}

//------------------------------------------
// 输入/按键 信号滤波
//------------------------------------------
void key_filter(void)
{
	static u8 keyScanTime;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	getKeyIOState(); //获取按键IO状态
	if(keyScanTime <20)
	{
		if(keyScanInfo.key_msg == keyScanInfo.old_key_msg)
		{
			if(keyScanInfo.cnt < 10) //独立按键滤波10ms
			{
				keyScanInfo.cnt++;
			}
			else if(keyScanInfo.cnt == 10)
			{
				keyValueTab = keyScanInfo.key_msg;//保存按键信息
			}
			else
			{}
		}
		else
		{
			keyScanInfo.cnt = 0;
		}
		keyScanInfo.old_key_msg = keyScanInfo.key_msg;
	}
	
	
	if(keyScanTime <20) keyScanTime++;
	else keyScanTime =0;
}



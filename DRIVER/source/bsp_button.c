#include "typedef.h"

//��������
//����IO�˿�

#define KEY0_PORT     GPIOB
#define KEY1_PORT  	  GPIOB



//����IO����
#define KEY0_PIN      GPIO_Pin_0
#define KEY1_PIN      GPIO_Pin_1


#define GPIO_ReadInputPin GPIO_ReadInputDataBit

vu32 keyValueTab=0,keyValueTab1=0; //������Ϣ��
static Key_Scan keyScanInfo;

/////////////////////////////////////////////////
//������ʼ��
void keyInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	������STM32F103ZE-EK �������߷��䣺
	USER��     : PG8  (�͵�ƽ��ʾ����)
	TAMPEER��  : PC13 (�͵�ƽ��ʾ����)
	WKUP��     : PA0  (!!!�ߵ�ƽ��ʾ����)
	ҡ��UP��   : PG15 (�͵�ƽ��ʾ����)
	ҡ��DOWN�� : PD3  (�͵�ƽ��ʾ����)
	ҡ��LEFT�� : PG14 (�͵�ƽ��ʾ����)
	ҡ��RIGHT��: PG13 (�͵�ƽ��ʾ����)
	ҡ��OK��   : PG7 (�͵�ƽ��ʾ����)
	*/
			
	/* ��1������GPIOA GPIOC GPIOD GPIOF GPIOG��ʱ��
	   ע�⣺����ط�����һ����ȫ��
	*/
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */

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
��    �ƣ�kbhit
��    �ܣ��ж��Ƿ��м�����
��ڲ�������
���ز�������0 �м����� | Ϊ0 û�м�����
��    ע�������Ի�ȡ��ϼ�ֵ
-----------------------------------------*/
u32 kbhit(void)
{
	return (keyValueTab);
}

/*---------------------------------------
��    �ƣ�getKeyState
��    �ܣ��ж�ĳ��������״̬
��ڲ�����vkKey ��ֵΪVK_KeyType����
		  ������Ӧ���������
���ز�������Ӧ����״̬��1 ���� | 0 û��
��    ע�������Ի�ȡ��ϼ�ֵ
-----------------------------------------*/
//u32 getKeyState(u32 vkKey)
//{
//	if(vkKey == VK_ALL) return ((keyValueTab1 & vkKey)||(keyValueTab & vkKey));
//	else if(vkKey < 0x10)
//	return (keyValueTab & vkKey);
//	else return ((keyValueTab1 & (vkKey>>4)) <<4);
//}


/*---------------------------------------
��    �ƣ�getKey
��    �ܣ���ȡ���µļ�ֵ
��ڲ�������
���ز����������ֵVK_KeyType
��    ע��������ʽ��ȡ������ֻ�л�ȡ����ֵ�����˳���
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



//��ȡ������ӦIO״̬
//���󰴼��Ͷ�����������
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
// ����/���� �ź��˲�
//------------------------------------------
void key_filter(void)
{
	static u8 keyScanTime;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	getKeyIOState(); //��ȡ����IO״̬
	if(keyScanTime <20)
	{
		if(keyScanInfo.key_msg == keyScanInfo.old_key_msg)
		{
			if(keyScanInfo.cnt < 10) //���������˲�10ms
			{
				keyScanInfo.cnt++;
			}
			else if(keyScanInfo.cnt == 10)
			{
				keyValueTab = keyScanInfo.key_msg;//���水����Ϣ
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



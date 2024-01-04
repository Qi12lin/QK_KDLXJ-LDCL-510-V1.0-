#include "typedef.h"

uint8_t uchCRCHi, uchCRCLo;
//volatile UART_MODULE UART_1	;
//volatile UART_MODULE UART_2	;
volatile UART_MODULE UART_3	;
volatile UART_MODULE UART_4	;
//volatile UART_MODULE UART_5	;
volatile UART_MODULE UART_6	;
/*
*********************************************************************************************************
*	CRC ��λ�ֽ�ֵ��
*********************************************************************************************************
*/
uint8_t auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/*
*********************************************************************************************************
*	CRC��λ�ֽ�ֵ��
*********************************************************************************************************
*/
uint8_t auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
/*
*********************************************************************************************************
*	�� �� ��: pgm_read_byte
*	����˵��: ��һ���ֽ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t pgm_read_byte(uint8_t *pt)
{
	return(*pt);
}
/*
*********************************************************************************************************
*	�� �� ��: CRC16
*	����˵��: CRCУ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CRC16(uint8_t *puchMsg, uint8_t usDataLen)
{
	uint8_t uIndex;
	uint8_t *point;
	u8 temp;

	uchCRCHi = 0xFF;
	uchCRCLo = 0xFF;

	while(usDataLen--) 
	{
		uIndex = uchCRCHi ^ *puchMsg++;
		point = auchCRCHi;
		temp = pgm_read_byte(point + uIndex);
		uchCRCHi = uchCRCLo ^ temp;
		point = auchCRCLo;
		uchCRCLo = pgm_read_byte(point + uIndex);		
	}
}



//���ջ�����
Rece_data receBuf; 

//�ӻ������ж�ȡһ���ֽ�����
//�������ֵ��*dat��ȶ�����0xffʱ���򷵻�ֵΪ0xff
u8 get_buf_byte(u8 *dat)
{
	if(receBuf.head == receBuf.tail) 
	{
		return 0;
	}
	*dat = receBuf.Buf[receBuf.head];
	receBuf.head = (receBuf.head + 1)%BUF_SIZE;
	return 1;
}

void InitUartBuf(void)
{
	receBuf.head = 0;
	receBuf.tail = 0;
}

//����һ���ֽ����ݵ�������
void save_buf_byte(u8 dat)
{
	receBuf.Buf[receBuf.tail] = dat;
	receBuf.tail = (receBuf.tail + 1)%BUF_SIZE;
}
//����uart1��ʼ��
void UART1Init(void)
{
}
//����uart2��ʼ��
void UART2Init(void)
{
//	INT8U i;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//	for( i = 0;i < UART_BUF_NUM ;i++ )UART_2.Buf[i] = 0;
//    UART_2.TxLen 	= 0;
//    UART_2.RxLen 	= 0; 
//    UART_2.Point 	= 0;
//	UART_2.TimeoutReg= 0;
//	UART_2.Status 	= IDLE_WAIT;		
//	USART_DeInit(USART2);

//	/* ��1������GPIO��USART������ʱ�� */
//	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);//| RCC_APB2Periph_AFIO
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	//����1��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_USART1); //GPIOA9����ΪUSART1
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_USART1); //GPIOA10����ΪUSART1
//	
//	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����	
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
//		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
//		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���	*/


//	/* ��4��������USART����
//	    - ������   = 9600 baud
//	    - ���ݳ��� = 8 Bits
//	    - 1��ֹͣλ
//	    - ��У��
//	    - ��ֹӲ������(����ֹRTS��CTS)
//	    - ʹ�ܽ��պͷ���
//	*/
//	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//9b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART2, &USART_InitStructure);
///* 
//	CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
//	�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
// 	�巢����ɱ�־��Transmission Complete flag 
//*/
//	USART_ClearFlag(USART2, USART_FLAG_TC); 
//	USART_ClearFlag(USART2, USART_FLAG_RXNE);	

//	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	/* ��5����ʹ�� USART�� ������� */
//	USART_Cmd(USART2, ENABLE);
}
//--------------------------------------------------------------------
//����uart3��ʼ��
//--------------------------------------------------------------------
void UART3Init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	for( i = 0;i < UART_BUF_NUM ;i++ )UART_3.Buf[i] = 0;
    UART_3.TxLen 	= 0;
    UART_3.RxLen 	= 0; 
    UART_3.Point 	= 0;
	UART_3.TimeoutReg= 0;
	UART_3.Status 	= IDLE_WAIT;		
	USART_DeInit(USART3);

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);//| RCC_APB2Periph_AFIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOD8����ΪUSART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOD9����ΪUSART3
	
	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* ��3��������USART����
	    - ������   = 38400 baud
	    - ���ݳ��� = 8 Bits
	    - 2��ֹͣλ
	    - ��У��
	    - ��ֹӲ������(����ֹRTS��CTS)
	    - ʹ�ܽ��պͷ���
	*/
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

/* 
	CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
 	�巢����ɱ�־��Transmission Complete flag 
*/
	USART_ClearFlag(USART3, USART_FLAG_TC); 
	USART_ClearFlag(USART3, USART_FLAG_RXNE);	

	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART3, ENABLE);
}
//--------------------------------------------------------------------
//����4��ʼ��
//--------------------------------------------------------------------
void UART4Init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		
	USART_DeInit(UART4);

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);//| RCC_APB2Periph_AFIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //����ΪUSART4
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //����ΪUSART4
	
	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ��3��������USART����
	    - ������   = 38400 baud
	    - ���ݳ��� = 8 Bits
	    - 2��ֹͣλ
	    - ��У��
	    - ��ֹӲ������(����ֹRTS��CTS)
	    - ʹ�ܽ��պͷ���
	*/
		USART_InitStructure.USART_BaudRate = 2400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

/* 
	CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
 	�巢����ɱ�־��Transmission Complete flag 
*/
	USART_ClearFlag(UART4, USART_FLAG_TC); 
	USART_ClearFlag(UART4, USART_FLAG_RXNE);	

	USART_ITConfig(UART4, USART_IT_TC, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(UART4, ENABLE);
}
//--------------------------------------------------------------------
//����6��ʼ��
//--------------------------------------------------------------------
void UART6Init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	for( i = 0;i < UART_BUF_NUM ;i++ )UART_6.Buf[i] = 0;
    UART_6.TxLen 	= 0;
    UART_6.RxLen 	= 0; 
    UART_6.Point 	= 0;
	UART_6.TimeoutReg= 0;
	UART_6.Status 	= IDLE_WAIT;		
	USART_DeInit(USART6);

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);//| RCC_APB2Periph_AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //����ΪUSART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //����ΪUSART6
	
	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ��3��������USART����
	    - ������   = 38400 baud
	    - ���ݳ��� = 8 Bits
	    - 2��ֹͣλ
	    - ��У��
	    - ��ֹӲ������(����ֹRTS��CTS)
	    - ʹ�ܽ��պͷ���
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	/* ��485�����ݷ�������Ϊ�������ģʽ����485��ʼ��Ϊ����״̬ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	USART6_REV;//��ʼ����״̬
/* 
	CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
 	�巢����ɱ�־��Transmission Complete flag 
*/
	USART_ClearFlag(USART6, USART_FLAG_TC); 
	USART_ClearFlag(USART6, USART_FLAG_RXNE);	

	USART_ITConfig(USART6, USART_IT_TC, ENABLE);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART6, ENABLE);
}
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void StartTxUART_8Bit(volatile UART_MODULE * p_uart)
{
	/*if( p_uart==&UART_1)
	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//��ֹ�����ж�
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);			
		USART_SendData(USART1, (INT16U)UART_1.Buf[UART_1.Point++] );
		UART_1.TxLen--;
	}		
	else if( p_uart==&UART_2 )
	{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//��ֹ�����ж�
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);			
		USART_SendData(USART2, (INT16U)UART_2.Buf[UART_2.Point++] );
		UART_2.TxLen--;
	}	
	else*/ if( p_uart==&UART_3 )
	{
//		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//��ֹ�����ж�
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		USART_ITConfig(USART3, USART_IT_TC, ENABLE);			
		USART_SendData(USART3, (INT16U)UART_3.Buf[UART_3.Point++] );
		UART_3.TxLen--;
	}
	else if( p_uart==&UART_4 )
	{
//		USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//��ֹ�����ж�
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		USART_ITConfig(UART4, USART_IT_TC, ENABLE);			
		USART_SendData(UART4, (INT16U)UART_4.Buf[UART_4.Point++] );
		UART_4.TxLen--;
	}	
	else if( p_uart==&UART_6 )
	{
		USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);//��ֹ�����ж�
		
		USART6_SEND;		
		USART_ClearITPendingBit(USART6, USART_IT_TC);
		USART_ITConfig(USART6, USART_IT_TC, ENABLE);			
		USART_SendData(USART6, (INT16U)UART_6.Buf[UART_6.Point++] );
		//USART_SendData(USART6, 0xAA);
		UART_6.TxLen--;
	}	
	else {}
							  
}

void StartRxUART_8Bit(volatile UART_MODULE * p_uart)
{
/*	if( p_uart==&UART_1 )
	{
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
//		USART1_REV;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
	}	
	else if( p_uart==&UART_2 )
	{
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
//		USART2_REV;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		
	}
	else*/ if( p_uart==&UART_3 )
	{
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);
//		USART3_REV;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		
	}
	else if( p_uart==&UART_4 )
	{
//		USART_ITConfig(UART4, USART_IT_TC, DISABLE);
//		USART4_REV;
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);		
	}	
	else if( p_uart==&UART_6 )
	{
		USART_ITConfig(USART6, USART_IT_TC, DISABLE);
		USART6_REV;
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);		
	}	
	else {}						  
}

void StartTxUART_9Bit(volatile UART_MODULE * p_uart)
{
//	if( p_uart==&UART_2 ){
//		USART_SendData(USART2, (INT16U)UART_2.Buf[UART_2.Point++]|0x0100 );
//		UART_2.TxLen--;
//	}						  
}

void UART_Init (volatile UART_MODULE * p_uart)
{
	/*if( p_uart==&UART_1) UART1Init();			
	else if( p_uart==&UART_2) UART2Init();			
	else*/ if (p_uart==&UART_3) UART3Init();			
	else if( p_uart==&UART_4) UART4Init();			
	//else if( p_uart==&UART_5) UART5Init();			
	else if( p_uart==&UART_6) UART6Init();	
	else {}
}



//����һ���ֽ�����
void uart_Send_Byte(u8 byte)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	
	USART_SendData(USART3, (uint16_t)byte);
	
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

//����һ���ֽڵ�����
u8 uart_Rece_Byte(u8 *type)
{
	return get_buf_byte(type);
}

//���ڽ��������Ƿ�Ϊ��
u8 uartReceNoEmpty(void)
{
	if(receBuf.head != receBuf.tail)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//�����ַ���
u8 uart_send_str(u8 *_ucStr, u16 _usLen)
{
	u8 *ucpStr = _ucStr;
	if(ucpStr == NULL)
	{
		return 0;
	}

	for(  ; ucpStr < (_ucStr + _usLen); ucpStr++ )
	{
		 uart_Send_Byte(*ucpStr);
	}
	return 1;
}

void uartTest(void)
{
		/*
	if(uart_Rece_Byte(&ucData))
	{
		switch(ucData)
		{
		case '0':
			SetLedState(LED1, 1);
		break;
		case '1':
			SetLedState(LED2, 1);
		break;
		case '3':
			SetLedState(LED2, 0);
			SetLedState(LED1, 0);
		break;
		default:
		break;
		}
	}
	*/
}

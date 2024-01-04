#include "typedef.h"

#ifndef _UART_H
#define _UART_H (1)

#define USART6_SEND 	GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define USART6_REV 		GPIO_SetBits(GPIOC, GPIO_Pin_8)//GPIO_ResetBits(GPIOC, GPIO_Pin_8)GPIO_SetBits

//------------UART_MODULE.Status---------------
#define IDLE_WAIT     		0x00        // ����̬���ȴ���ʼλ
#define RECE_START      	0x01        // �յ���ʼλ���ȴ�����λ
#define RECE_END       		0x02        // �յ�����λ���ȴ�����
#define SEND_START     		0x03        // ������ʼλ
#define SEND_END      		0x04        // �������
#define SEND_EXCHANGE		0x05		// ���������л�����/д��վ1/վ2��
#define	SEND_ING			0x06		//������

#define	UART_BUF_NUM	255
typedef struct {
	INT16U	TimeoutReg			;		//Timeout register(ms)
	uint8_t Status				;		//Sci status
	uint8_t RxStatus			;		//Sci status
	uint8_t	Point				;		//Data process point
	uint8_t	RxPoint				;		//Data process point	
	uint8_t	Buf[UART_BUF_NUM]	;		//Data buffer Zone		Useful Data 100 word
	uint8_t	RxBuf[UART_BUF_NUM]	;		//Data buffer Zone		Useful Data 100 word	
	uint8_t	RxLen				;		//Receive Data length
	uint8_t	TxLen				;		//Send Data length
} UART_MODULE;

//extern volatile UART_MODULE UART_1;
//extern volatile UART_MODULE UART_2;
extern volatile UART_MODULE UART_3;
extern volatile UART_MODULE UART_4;
extern volatile UART_MODULE UART_6;
extern uint8_t uchCRCHi, uchCRCLo;
extern	void 	UART_Init(volatile UART_MODULE * p_uart);
extern	void 	StartTxUART_8Bit(volatile UART_MODULE * p_uart);
extern	void 	StartRxUART_8Bit(volatile UART_MODULE * p_uart);
extern	void 	StartTxUART_9Bit(volatile UART_MODULE * p_uart);
extern void CRC16(uint8_t *puchMsg, uint8_t usDataLen);
//extern void UART3Init(void);



/*	ÿ�����ڶ�Ӧ1��ȫ�ֵĽṹ�����
	���Ա������ʵ�����ݷ��ͽ���״̬�������
*/
//typedef struct
//{
//	uint16_t 	SendCount;		/* ���ڷ��������ֽڼ��� */
//	uint16_t 	RevCount;		/* ���ڽ��������ֽڼ��� */
//	uint8_t 	SendEnd;		/* ���ڷ�����ɱ�־ */
//	uint8_t 	RevEnd; 		/* ���ڽ�����ɱ�־ */
//	uint16_t 	SendNumber;		/* ���ڷ��������ܵ��ֽ��� */
//	uint8_t 	SendData[200];/* ���ڷ������� */ 
//	uint8_t 	RevData[200]; 	/* ���ڽ������� */
//	uint16_t 	SendTemp;		/* ���ڷ��������ݴ��� */
//	uint16_t 	SendTemp1;		/* ���ڷ��������ݴ��� */
//	uint8_t 	RevTemp;		/* ���ڽ��������ݴ��� */
//	uint16_t 	RegData[200];	/* ModBus�Ĵ������� */
//	uint8_t 	BitData[200];	/* ModBusλ״̬���� */
//	uint16_t 	RevTime;		/* ���ڽ���ָ�븴λʱ�� */
//	uint16_t 	SendTime;		/* ���ڷ������ݼ��ʱ�� */
//	uint16_t 	RegDataMin[200];	/* ModBus�Ĵ���������Сֵ */
//	uint16_t 	RegDataMax[200];	/* ModBus�Ĵ����������ֵ */
//}MODBUS_T;


#define BUF_SIZE 20

//���ջ�����
typedef struct
{
	u8 Buf[BUF_SIZE];	
	u8 head;
	u8 tail;
}Rece_data;

extern u8 get_buf_byte(u8 *dat); //�ӻ������ж�ȡһ���ֽ�����
extern void save_buf_byte(u8 dat);//����һ���ֽ����ݵ�������
extern void uartInit(void);//���ڳ�ʼ��
extern void uart_Send_Byte(u8 byte);//����һ���ֽ�
extern u8 uart_Rece_Byte(u8 *type);//����һ���ֽ�
extern void InitUartBuf(void);
extern u8 uartReceNoEmpty(void); //�жϽ��������Ƿ�Ϊ��
//�����ַ���
extern u8 uart_send_str(u8 *_ucStr, u16 _usLen);
//����uart3��ʼ��

#endif



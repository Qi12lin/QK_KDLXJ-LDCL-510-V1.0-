#include "typedef.h"

#ifndef _UART_H
#define _UART_H (1)

#define USART6_SEND 	GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define USART6_REV 		GPIO_SetBits(GPIOC, GPIO_Pin_8)//GPIO_ResetBits(GPIOC, GPIO_Pin_8)GPIO_SetBits

//------------UART_MODULE.Status---------------
#define IDLE_WAIT     		0x00        // 空闲态，等待起始位
#define RECE_START      	0x01        // 收到起始位，等待结束位
#define RECE_END       		0x02        // 收到结束位，等待发送
#define SEND_START     		0x03        // 发送起始位
#define SEND_END      		0x04        // 发送完毕
#define SEND_EXCHANGE		0x05		// 发送内容切换（读/写，站1/站2）
#define	SEND_ING			0x06		//发送中

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



/*	每个串口对应1个全局的结构体变量
	其成员变量是实现数据发送接收状态所必须的
*/
//typedef struct
//{
//	uint16_t 	SendCount;		/* 串口发送数据字节计数 */
//	uint16_t 	RevCount;		/* 串口接收数据字节计数 */
//	uint8_t 	SendEnd;		/* 串口发送完成标志 */
//	uint8_t 	RevEnd; 		/* 串口接收完成标志 */
//	uint16_t 	SendNumber;		/* 串口发送数据总的字节数 */
//	uint8_t 	SendData[200];/* 串口发送数据 */ 
//	uint8_t 	RevData[200]; 	/* 串口接收数据 */
//	uint16_t 	SendTemp;		/* 串口发送数据暂存量 */
//	uint16_t 	SendTemp1;		/* 串口发送数据暂存量 */
//	uint8_t 	RevTemp;		/* 串口接收数据暂存量 */
//	uint16_t 	RegData[200];	/* ModBus寄存器数据 */
//	uint8_t 	BitData[200];	/* ModBus位状态数据 */
//	uint16_t 	RevTime;		/* 串口接收指针复位时间 */
//	uint16_t 	SendTime;		/* 串口发送数据间隔时间 */
//	uint16_t 	RegDataMin[200];	/* ModBus寄存器数据最小值 */
//	uint16_t 	RegDataMax[200];	/* ModBus寄存器数据最大值 */
//}MODBUS_T;


#define BUF_SIZE 20

//接收缓冲区
typedef struct
{
	u8 Buf[BUF_SIZE];	
	u8 head;
	u8 tail;
}Rece_data;

extern u8 get_buf_byte(u8 *dat); //从缓冲区中读取一个字节数据
extern void save_buf_byte(u8 dat);//保存一个字节数据到缓冲区
extern void uartInit(void);//串口初始化
extern void uart_Send_Byte(u8 byte);//发送一个字节
extern u8 uart_Rece_Byte(u8 *type);//接收一个字节
extern void InitUartBuf(void);
extern u8 uartReceNoEmpty(void); //判断接收数据是否为空
//发送字符串
extern u8 uart_send_str(u8 *_ucStr, u16 _usLen);
//串口uart3初始化

#endif



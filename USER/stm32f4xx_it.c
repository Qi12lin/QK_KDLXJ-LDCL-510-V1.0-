#include "typedef.h"
#include "bsp_uart.h"
/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


/**********************************************************************************************************
*	函 数 名: SysTick_Handler
*	功能说明: 系统嘀嗒定时器中断服务程序。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void SysTick_Handler(void)
{
	SysTick_ISR(); 
}
/**********************************************************************************************************
*	函 数 名: USART3_IRQHandler
*	功能说明: 串口3发送接收中断服务程序
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void USART3_IRQHandler(void)
{
	INT16U tmp;
  	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		tmp= USART_ReceiveData(USART3);
		switch( UART_3.RxStatus )	//全双工通信
		{
			case IDLE_WAIT:						//4
				UART_3.RxStatus = RECE_START;
				UART_3.RxPoint=0;
			case RECE_START:					//1
				UART_3.RxBuf[UART_3.RxPoint] = tmp;
				UART_3.RxPoint++;
				UART_3.RxLen = UART_3.RxPoint;		
				break;
		}
		UART_3.TimeoutReg=0;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  	}

	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		if( UART_3.TxLen > 0 )
		{
		     USART_SendData(USART3, UART_3.Buf[UART_3.Point++]);
		     UART_3.TxLen--;
		}
		else //发送完毕
		{ 
			UART_3.Status = SEND_END;   //状态切换
			UART_3.Point=0;
		}
//		UART_3.TimeoutReg=0;
  	  	USART_ClearITPendingBit(USART3, USART_IT_TC);		
	}	 
}
/**********************************************************************************************************
*	函 数 名: UART4_IRQHandler
*	功能说明: 串口4发送接收中断服务程序
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void UART4_IRQHandler(void)
{
	INT16U tmp;
  	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		tmp= USART_ReceiveData(UART4);
		save_buf_byte(tmp);
		Comm_Rece_Block();	
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
  	}

	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
			if(sendPos < tranStr[1]+3)
		 {
//			uart_Send_Byte();
			USART_SendData(UART4, (uint16_t)((tranStr[sendPos])));
			sendPos++;
		 }
		 else
		 {
		 	sendPos = 0;
		 }
  	  	USART_ClearITPendingBit(UART4, USART_IT_TC);		
	} 	

}
/**********************************************************************************************************
*	函 数 名: USART6_IRQHandler
*	功能说明: 串口6发送接收中断服务程序
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void USART6_IRQHandler(void)
{
	INT16U tmp;
  	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		tmp= USART_ReceiveData(USART6);

		switch( UART_6.Status )		//半双工通信
		{
			case SEND_END:						//4
				UART_6.Status = RECE_START;
				UART_6.Point=0;
			case RECE_START:					//1
				UART_6.Buf[UART_6.Point] = tmp;
				UART_6.Point++;
				UART_6.RxLen = UART_6.Point;	
				break;
		}
		UART_6.TimeoutReg=0;
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
  	}

	if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)
	{
		if( UART_6.TxLen > 0 )
		{
		     USART_SendData(USART6, UART_6.Buf[UART_6.Point++]);
			//USART_SendData(USART6,0xAA);
		     UART_6.TxLen--;
		}
		else //发送完毕
		{ 
			StartRxUART_8Bit(&UART_6);//切换成接收
			UART_6.Status = SEND_END;   //状态切换
			UART_6.Point=0;
		}
		UART_6.TimeoutReg=0;
  	  	USART_ClearITPendingBit(USART6, USART_IT_TC);		
	}	 
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

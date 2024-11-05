/**
  ******************************************************************************
  * @file           : drv_uart.c\h
  * @brief          : 
  * @note           : finish 2022-2-12 12:24:28
  ******************************************************************************
  */

#include "drv_uart.h"
#include "config_uart.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

uint8_t usart1_dma_rxbuf[USART1_RX_BUF_LEN];
uint8_t usart2_dma_rxbuf[2][USART2_RX_BUF_LEN];
uint8_t usart3_dma_rxbuf[USART3_RX_BUF_LEN];
uint8_t usart4_dma_rxbuf[USART4_RX_BUF_LEN];
uint8_t usart5_dma_rxbuf[USART5_RX_BUF_LEN];

void USART1_Init(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除USART1串口的空闲标志位
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//启用USART1串口的空闲中断
	
	//使能DMA接收
	SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);	
	
	DMA_Start(huart1.hdmarx, \
			  (uint32_t)&huart1.Instance->DR, \
			  (uint32_t)usart1_dma_rxbuf,  \
			  USART1_RX_BUF_LEN);
}

void USART2_Init(void){
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	
	//使能DMA接收
	SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);
	

}

void USART3_Init(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	
	//使能DMA接收
	SET_BIT(huart3.Instance->CR3, USART_CR3_DMAR);	
	
	DMA_Start(huart3.hdmarx, \
			  (uint32_t)&huart3.Instance->DR, \
			  (uint32_t)usart3_dma_rxbuf, \
			  USART3_RX_BUF_LEN);
}

void USART4_Init(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart4);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	
	//使能DMA接收
	SET_BIT(huart4.Instance->CR3, USART_CR3_DMAR);	
	
	DMA_Start(huart4.hdmarx, \
			  (uint32_t)&huart4.Instance->DR, \
			  (uint32_t)usart4_dma_rxbuf, \
			  USART4_RX_BUF_LEN);
}

void USART5_Init(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
	
	//使能DMA接收
	SET_BIT(huart5.Instance->CR3, USART_CR3_DMAR);	
	
	DMA_Start(huart5.hdmarx, \
			  (uint32_t)&huart5.Instance->DR, \
			  (uint32_t)usart5_dma_rxbuf, \
			  USART5_RX_BUF_LEN);
}



void DRV_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    // 判断是否为空闲中断
	if( __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&		//IDLE中断标志
		__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))		//IDLE中断使能
	{
		uart_rx_idle_callback(huart);
	}
}


/**
 * @brief DMA_Start
 * 
 * @param hdma 串口接收DMA句柄，用于控制DMA接收操作的参数和状态
 * @param SrcAddress 表示接收数据的来源
 * @param DstAddress 存储接收数据的目标地址
 * @param DataLength 收缓冲区的长度，表示DMA将接收多少数据后自动停止传输
 * 
 * @retval DMA处于就绪->HAL_OK | DMA处于忙碌->HAL_BUSY
 */
static HAL_StatusTypeDef DMA_Start(DMA_HandleTypeDef *hdma, \
                            uint32_t SrcAddress, \
                            uint32_t DstAddress, \
                            uint32_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	/* Process locked */
	__HAL_LOCK(hdma);
	if(HAL_DMA_STATE_READY == hdma->State)
	{
		/* Change DMA peripheral state */
		hdma->State = HAL_DMA_STATE_BUSY;

		/* Initialize the error code */
		hdma->ErrorCode = HAL_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		/* Clear DBM bit */
//		hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

		/* Configure DMA Stream data length */
		hdma->Instance->CNDTR = DataLength;

		/* Memory to Peripheral */
		if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
		{
			/* Configure DMA Stream destination address */
			hdma->Instance->CPAR = DstAddress;//外设地址

			/* Configure DMA Stream source address */
			hdma->Instance->CMAR = SrcAddress;//储存器
		}
		/* Peripheral to Memory */
		else
		{
			/* Configure DMA Stream source address */
			hdma->Instance->CPAR = SrcAddress;

			/* Configure DMA Stream destination address */
			hdma->Instance->CMAR = DstAddress;
		}

		/* Enable the Peripheral */
		__HAL_DMA_ENABLE(hdma);
	}
	else
	{
		/* Process unlocked */
		__HAL_UNLOCK(hdma);

		/* Return error status */
		status = HAL_BUSY;
	} 
	return status; 	
}



static void uart_rx_idle_callback(UART_HandleTypeDef* huart)
{
	/* clear idle it flag avoid idle interrupt all the time */
	__HAL_UART_CLEAR_IDLEFLAG(huart);	
	/* handle received data in idle interrupt */
	if (huart == &huart1)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);
		/* handle dbus data dbus_buf from DMA */
		USART1_rxDataHandler(usart1_dma_rxbuf);
		memset(usart1_dma_rxbuf, 0, USART1_RX_BUF_LEN);
		/* restart dma transmission */	  
		__HAL_DMA_ENABLE(huart->hdmarx);
	}
//	else if (huart == &huart2)
//	{
//		/* clear DMA transfer complete flag */
//		__HAL_DMA_DISABLE(huart->hdmarx);

//		/* handle dbus data dbus_buf from DMA */
//		//uint32_t status = taskENTER_CRITICAL_FROM_ISR();
//		if ((USART2_RX_BUF_LEN - huart->hdmarx->Instance->NDTR) == USART2_RX_DATA_FRAME_LEN)
//		{
//			if(huart->hdmarx->Instance->CR & DMA_SxCR_CT)
//				huart->hdmarx->XferM1CpltCallback(huart->hdmarx);
//			else
//				huart->hdmarx->XferCpltCallback(huart->hdmarx);
//		}
////		taskEXIT_CRITICAL_FROM_ISR(status);

//		/* restart dma transmission */
//		__HAL_DMA_SET_COUNTER(huart->hdmarx, USART2_RX_BUF_LEN);
//		__HAL_DMA_ENABLE(huart->hdmarx);
//	}
	else if (huart == &huart3)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);
		/* handle dbus data dbus_buf from DMA */
		USART3_rxDataHandler(usart3_dma_rxbuf);
		memset(usart3_dma_rxbuf, 0, USART3_RX_BUF_LEN);
		/* restart dma transmission */
		huart->hdmarx->Instance->CNDTR=USART3_RX_BUF_LEN;	  
		__HAL_DMA_ENABLE(huart->hdmarx);
	}
	else if (huart == &huart4)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);
		/* handle dbus data dbus_buf from DMA */
		USART4_rxDataHandler(usart4_dma_rxbuf);
		memset(usart4_dma_rxbuf, 0, USART4_RX_BUF_LEN);
		/* restart dma transmission */	  
		__HAL_DMA_ENABLE(huart->hdmarx);		
	}
	else if (huart == &huart5)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);
		/* handle dbus data dbus_buf from DMA */
		USART5_rxDataHandler(usart5_dma_rxbuf);
		memset(usart5_dma_rxbuf, 0, USART5_RX_BUF_LEN);
		/* restart dma transmission */	  
		__HAL_DMA_ENABLE(huart->hdmarx);
	}

}

/**
  ******************************************************************************
  * @file           : drv_uart.c\h
  * @brief          : 
  * @note           : finish 2022-2-12 12:24:28
  ******************************************************************************
  */

#ifndef DRV_UART_H
#define DRV_UART_H

#include "main.h"

void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);
void USART4_Init(void);
void USART5_Init(void);
void USART6_Init(void);

void DRV_UART_IRQHandler(UART_HandleTypeDef *huart);


static void uart_rx_idle_callback(UART_HandleTypeDef* huart);

static HAL_StatusTypeDef DMA_Start(DMA_HandleTypeDef *hdma, \
                            uint32_t SrcAddress, \
                            uint32_t DstAddress, \
                            uint32_t DataLength);

#endif

/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

#include <cmsis_os.h>
#include "lwrb.h"
#include "semphr.h"

static SemaphoreHandle_t readSemaphore;
static osSemaphoreId writeSemaphore;




#define TX_DMA_BUFFER_SIZE 16
__aligned(32) uint8_t TX_DMA_buffer[TX_DMA_BUFFER_SIZE];

#define RX_DMA_BUFFER_SIZE 16
__aligned(32) uint8_t RX_DMA_buffer[RX_DMA_BUFFER_SIZE];

lwrb_t rx_buffer;
uint8_t rx_buffer_container[255];

lwrb_t tx_buffer;
uint8_t tx_buffer_container[255];

void initialize_buffers(void) {
	osSemaphoreDef(WRITESEM);
	writeSemaphore = osSemaphoreCreate(osSemaphore(WRITESEM), 1);

	vSemaphoreCreateBinary(readSemaphore);
	if (readSemaphore == NULL) {
		Error_Handler();
	}

	if (lwrb_init(&rx_buffer, rx_buffer_container, sizeof(rx_buffer_container)) != 1){
		Error_Handler();
	}
	if (lwrb_init(&tx_buffer, tx_buffer_container, sizeof(tx_buffer_container)) != 1){
		Error_Handler();
	}
}

/* USER CODE END 0 */

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;


void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */
	initialize_buffers();
  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RX_DMA_buffer, RX_DMA_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
  /* USER CODE END USART2_Init 2 */

}


/* USER CODE BEGIN 1 */

static int tx_next_chunk(void) {
	int number_of_items_in_tx_buffer = lwrb_read(&tx_buffer, TX_DMA_buffer, TX_DMA_BUFFER_SIZE);
	if (number_of_items_in_tx_buffer > 0) {
		if (HAL_UART_Transmit_DMA(&huart2, TX_DMA_buffer,
				number_of_items_in_tx_buffer) != HAL_OK) {
			assert(0);
		}
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
	return number_of_items_in_tx_buffer;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART2) {
		if (lwrb_write(&rx_buffer,  RX_DMA_buffer, Size) != Size ){
			//buffer overrun
		}

		HAL_UARTEx_ReceiveToIdle_DMA(huart, RX_DMA_buffer, RX_DMA_BUFFER_SIZE);
		BaseType_t xHigherPriorityTaskWoken;
		xSemaphoreGiveFromISR(readSemaphore,&xHigherPriorityTaskWoken);
	}
}

int get_rx_data(uint8_t *buffer, size_t buffer_length, uint32_t timeout) {
	xSemaphoreTake(readSemaphore,pdMS_TO_TICKS(timeout ));
	return lwrb_read(&rx_buffer, buffer, buffer_length);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {
		tx_next_chunk();
	}
}

void put_tx_data_with_wait(uint8_t *buffer, size_t buffer_length) {
	int retries = 1000;
	while (retries > 0) {
		int pushed_bytes = put_tx_data(buffer, buffer_length);
		buffer_length -= pushed_bytes;
		buffer += pushed_bytes;
		if (buffer_length <= 0) {
			break;
		}
		osDelay(1);
		retries--;
	}
}

int put_tx_data(uint8_t *buffer, size_t buffer_length) {
	int ret = 0;
	if (osSemaphoreWait(writeSemaphore, osWaitForever) == osOK) {
		ret = lwrb_write(&tx_buffer, buffer, buffer_length);
		osSemaphoreRelease(writeSemaphore);
	}

	if (huart2.gState == HAL_UART_STATE_READY) {
		tx_next_chunk();
	}
	return ret;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

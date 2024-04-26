/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "freeRTOS.h"
#include "task.h"
#include <semphr.h>
#include <SEGGER_SYSVIEW.h>
#include <stm32f4xx_hal.h>
#include "fatfs.h"
#include "Uart4Setup.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define STACK_SIZE 128
void GreenTaskA( void * argument);
void BlueTaskB( void* argumet );
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// Create storage for a pointer to a semaphore

SemaphoreHandle_t semPtr = NULL;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;

/* Definitions for defaultTask */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	BaseType_t status;

	BaseType_t retVal;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FATFS_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  SEGGER_SYSVIEW_Conf();

 // NVIC_SetPriorityGrouping(0);

  // Ensure proper priority-grouping for FreeRTOS
  	NVIC_SetPriorityGrouping(0);

    		//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);	//ensure proper priority grouping for freeRTOS

    		// Create a semaphore using the FreeRTOS Heap
    	//	semPtr = xSemaphoreCreateBinary();
    	    // Ensure the pointer is valid (semaphore created successfully)
    //		assert_param(semPtr != NULL);

    		// Create TaskA as a higher priority than TaskB.  In this example, this isn't strictly necessary since the tasks
    		// spend nearly all of their time blocked
    	//	status=xTaskCreate(GreenTaskA, "GreenTaskA", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
    //		assert_param(status == pdPASS);
//
    		// Using an assert to ensure proper task creation
    //		status=xTaskCreate(BlueTaskB, "BlueTaskB", STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
   // 		assert_param( status == pdPASS);

    // Setup a timer to kick off UART traffic (flowing out of UART4 TX line
      // and into USART2 RX line) 5 seconds after the scheduler starts.
      // The transmission needs to start after the receiver is ready for data.
  	TimerHandle_t oneShotHandle =
  	xTimerCreate(	"startUart4Traffic",
  					5000 /portTICK_PERIOD_MS,
  					pdFALSE,
  					NULL,
  					startUart4Traffic);
  	assert_param(oneShotHandle != NULL);
  	xTimerStart(oneShotHandle, 0);

      // Create the semaphore
  	rxDone = xSemaphoreCreateBinary();
  	assert_param(rxDone != NULL);

      // Setup tasks, making sure they have been properly created before moving on
  	retVal = xTaskCreate(uartPrintOutTask, "uartPrint", STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
  	assert_param(retVal == pdPASS);


    		// Start the scheduler - shouldn't return unless there's a problem
    		vTaskStartScheduler();

    		// If you've wound up here, there is likely an issue with over-running the freeRTOS heap
    		while(1)
    		{
    		}

  }
/**
 setupUSART2DMA():
 * Enable the clock for the DMA1 controller
 * Enable the IRQ for DMA1 Stream5
 * Configure and initialize DMA stream
   * Configure the struct that specifies the DMA stream
   * Calls HAL_DMA_Init() to initialize the DMA stream
     *  HAL_DMA_Init() first calls __HAL_DMA_DISABLE()
 * Enable the DMA transfer-complete interrupt
 * Enable USART2's DMA-mode, for reception

 */
void setupUSART2DMA( void )
{
	HAL_StatusTypeDef retVal;

	// __HAL_RCC_DMA1_CLK_ENABLE : AHB1 Peripheral Clock Enable
	// * Prior to calling HAL_DMA_Init() the clock must be enabled for DMA through this macro.
	__HAL_RCC_DMA1_CLK_ENABLE();

	NVIC_SetPriority(DMA1_Stream5_IRQn, 6);
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	// The DMA stream is configured by filling out usart2DmaRx
	memset(&usart2DmaRx, 0, sizeof(usart2DmaRx));

	// Specify stream 5
    usart2DmaRx.Instance = DMA1_Stream5;
    // Specify channel 4
    usart2DmaRx.Init.Channel = DMA_CHANNEL_4;
    // Specify the data-transfer is from the peripheral to memory
    usart2DmaRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    // Specify not to use the stream’s FIFO queue.
    // * This is referred to as “direct mode”.
    // * After each single data transfer from the peripheral to the
    //   FIFO, the data is immediately stored into the memory
    //   destination.
    usart2DmaRx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    // Specify the stream’s priority, relative to other streams
    // in the DMA controller.
    usart2DmaRx.Init.Priority = DMA_PRIORITY_HIGH;
    // Specifies the DMA mode.
    // * Normal mode specifies the data-transfer occurs once
    //   (vs circular mode).
    // * Normal mode also specifies the DMA flow-controller is used
    //   (vs the peripheral flow-controller).
    usart2DmaRx.Init.Mode = DMA_NORMAL;

    //
    // Peripheral-transfer configuration
    //

    // DMA Peripheral data size.
    // * Specifies one byte.
    usart2DmaRx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // Specify that the peripheral address is not incremented
    // after each transfer.
    usart2DmaRx.Init.PeriphInc = DMA_PINC_DISABLE;
    // Specify the DMA controller should generate a “single transfer”.
    // * Each DMA request generates a data transfer of a byte.
    // * Since direct-mode is used, “single transfer” must be used.
    usart2DmaRx.Init.PeriphBurst = DMA_PBURST_SINGLE;

    //
    // Memory-transfer configuration
    //

    // DMA memory data-size
    // * Specifies one byte.
    usart2DmaRx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    // Enable memory incrementing.
    // * The DMA controller will increment the memory address
    //   after a transfer
    usart2DmaRx.Init.MemInc = DMA_MINC_ENABLE;
    // Specify the DMA controller should generate a “single transfer”.
    // * Each DMA request generates a data transfer of a byte.
    // * Since direct-mode is used, “single transfer” must be used.
    usart2DmaRx.Init.MemBurst = DMA_MBURST_SINGLE;


	// HAL_DMA_Init : Initialize the DMA stream specified in usart2DmaRx, and create the
    // associated handle.
	retVal = HAL_DMA_Init(&usart2DmaRx);
	assert_param(retVal == HAL_OK);

	// DMA stream x configuration register (DMA_SxCR)
	// * TCIE: transfer-complete interrupt enable
	DMA1_Stream5->CR |= DMA_SxCR_TCIE;

    // Control register 3 (USART_CR3)
    // * DMAR: DMA enable receiver
    // * 1: DMA mode is enabled for reception
    USART2->CR3 |= USART_CR3_DMAR_Msk;

}

/**
 * Start an interrupt-driven receive.
 */
int32_t startReceiveDMA( volatile uint8_t * Buffer, uint_fast16_t Len )
{
	if(!rxInProgress)
	{
		rxInProgress = true;

		// __HAL_DMA_DISABLE : Disables the specified DMA Stream.
		__HAL_DMA_DISABLE(&usart2DmaRx);
		// Wait for the EN bit to be set to 0.
		// * Described in ST's “Using the . . . STM32F7 Series DMA controller” (AN4031)
		while((DMA1_Stream5->CR & DMA_SxCR_EN) != 0);

		// setupUSART2DMA():
		// * Configure and initialize DMA stream
		// * Enable the DMA transfer-complete interrupt
		setupUSART2DMA();

        // Enable DMA for USART2-reception
        // * Get the DMA peripheral ready to receive data immediately before enabling UART
        //   so there is no chance of overrun.
        // * The DMA stream-enable bit must be toggled before a transfer will properly restart.

		// HAL_DMA_Start : Starts the DMA Transfer
		// * Enables the DMA stream (sets the EN bit in the DMA_SxCR register).
		// * Parameter USART2->RDR : USART Receive Data register
		if(HAL_DMA_Start(&usart2DmaRx, (uint32_t)&(USART2->RDR), (uint32_t) Buffer, Len) != HAL_OK)
		{
		    test_startReceiveDMA_halDmaStartFailed++;
			return -1;
		}


		// Clear error flags.
		// Needed for reception to resume after being paused in the debugger.
		USART2->ICR |= (USART_ICR_FECF | USART_ICR_PECF |
		            	USART_ICR_NCF | USART_ICR_ORECF);

		// For this specific instance, we'll avoid enabling UART interrupts for errors since
		// we'll wind up with a lot of noise on the line at high baud-rates (the way the ISR is written will
		// cause a transfer to terminate if there are any errors are detected, rather than simply
		// continue with what data it can).
        //USART2->CR3 |= (USART_CR3_EIE);	//enable error interrupts
		//USART2->CR1 |= USART_CR1_RXNEIE | USART_CR1_PEIE);

		//NVIC_SetPriority(USART2_IRQn, 6);
		//NVIC_EnableIRQ(USART2_IRQn);

        // Enable the UART
        USART2->CR1 |= (USART_CR1_UE);

		return 0;
	}

	test_startReceiveDMA_rxInProgressNotExpected++;
	return -1;
}

void startUart4Traffic( TimerHandle_t xTimer )
{
	SetupUart4ExternalSim(BAUDRATE);
}

void stopReceiveDMA( void )
{
	rxInProgress = false;

	// HAL_DMA_Abort : Aborts the DMA Transfer
	HAL_DMA_Abort(&usart2DmaRx);

}

void uartPrintOutTask( void* NotUsed)
{
    uint8_t i;
    memset((void*)memoryBuffer, 0, BUFFER_LENGTH);

	// Configure and initialize the DMA stream
	setupUSART2DMA();

	// Initialize USART2, pass usart2DmaRx for use in receive
	STM_UartInit(USART2, BAUDRATE, NULL, &usart2DmaRx);

	while(1)
	{
	    // Start the DMA transfer for each iteration of the loop.
	    // * After the DMA transfer completes, it must be restarted.
		startReceiveDMA(memoryBuffer, (uint_fast16_t)(STRING_LENGTH));

		// Take the semaphore. The timeout is 100 ticks.
		if(xSemaphoreTake(rxDone, 100) == pdPASS)
		{
			// NDTR is the remaining number of data items to be transferred,
			// 0 signals completion
			if(DMA1_Stream5->NDTR == 0)
			{
			    test_uartPrintOutTask_received++;

			    // The string sent by UART4 has a null-terminator.
			    // Replace the null-terminator with a "#".
			    for(i=0;i<=(STRING_LENGTH-1);i++){
			        if (memoryBuffer[i]==0){
			            memoryBuffer[i]='#';
			            break;
			        }
			    }
			    // Add a null-terminator to the end of the string (needed by SEGGER_SYSVIEW_Print)
			    memoryBuffer[BUFFER_LENGTH - 1] = 0;

			    SEGGER_SYSVIEW_Print("received: ");
				SEGGER_SYSVIEW_Print((char*)memoryBuffer);
			}
			else
			{
			    test_uartPrintOutTask_xferNotComplete++;
			}
		}
		else
		{
		    // Stop the DMA transfer if timeout in xSemaphoreTake
	        stopReceiveDMA();
            // * UART4 is started 5 seconds after the scheduler is started.
            // * So, xSemaphoreTake will timeout 50 times before
            //   UART4 starts sending data.
	        test_uartPrintOutTask_xSemaphoreTakeTimeout++;
			SEGGER_SYSVIEW_PrintfHost("timeout");
		}
	}
}


/**
 * Given the DMA setup performed by setupUSART2DMA,
 * this ISR will only execute when a DMA transfer is complete
 */
void DMA1_Stream5_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	SEGGER_SYSVIEW_RecordEnterISR();

	// DMA high interrupt status register (DMA_HISR)
	// * TCIF[7:4]: stream x transfer-complete interrupt-flag
	if(rxInProgress && (DMA1->HISR & DMA_HISR_TCIF5))
	{
		rxInProgress = false;

		// DMA high interrupt flag clear register (DMA_HIFCR)
		// * CTCIF[7:4]: stream x clear transfer-complete interrupt-flag
		//   * Writing 1 to this bit clears the corresponding TCIFx flag in
		//     the DMA_HISR register.
		DMA1->HIFCR |= DMA_HIFCR_CTCIF5;

		xSemaphoreGiveFromISR(rxDone, &xHigherPriorityTaskWoken);
	}
	else
	{
	    test_DMA1_Stream5_IRQHandler_notExpected++;
	}

	SEGGER_SYSVIEW_RecordExitISR();
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}









void GreenTaskA( void* argument )
{
	uint_fast8_t count = 0;
	while(1)
	{
		// Every 5 times through the loop, give the semaphore
		if(++count >= 5)
		{
			count = 0;
			SEGGER_SYSVIEW_PrintfHost("GreenTaskA gives semPtr");
			xSemaphoreGive(semPtr);
		}
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		vTaskDelay(1000/portTICK_PERIOD_MS);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

/**
 * BlueTaskB waits to receive semPtr, then triple-blinks the blue LED
 */
void BlueTaskB( void* argument )
{
	while(1)
	{
		// 'take' the semaphore with no timeout.
	    // * In our system, FreeRTOSConfig.h specifies "#define INCLUDE_vTaskSuspend 1".
	    // * So, in xSemaphoreTake, portMAX_DELAY specifies an indefinite wait.
		SEGGER_SYSVIEW_PrintfHost("BlueTaskB attempts to take semPtr");
		if(xSemaphoreTake(semPtr, 500/portTICK_PERIOD_MS) == pdPASS)
		{
			SEGGER_SYSVIEW_PrintfHost("BlueTaskB received semPtr");
			// Triple-blink the Blue LED
			for(uint_fast8_t i = 0; i < 3; i++)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
				vTaskDelay(500/portTICK_PERIOD_MS);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
				vTaskDelay(500/portTICK_PERIOD_MS);
			}
		}
		else
		{
		//	This is the code that would be executed if we timed-out waiting for
		//	the semaphore to be given.
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			 vTaskDelay(250/ portTICK_PERIOD_MS);

			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

		}
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM9 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM9) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

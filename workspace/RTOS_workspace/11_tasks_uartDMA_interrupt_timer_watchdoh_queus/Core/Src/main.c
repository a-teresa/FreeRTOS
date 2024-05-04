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
#include <stm32f4xx_it.h>
#include "fatfs.h"
#include "usart.h"
#include <timer.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define STACK_SIZE 128
void GreenTaskA( void * argument);
void BlueTaskB( void* argument );
void UartTaskC( void* argument );
void vHandlingTaskTimer (void* argument);
void vTaskMonitor (void* argument);
TaskHandle_t timerTaskHandler = NULL;
TaskHandle_t watchdogTaskHandler = NULL;
TaskHandle_t ledATaskHandler = NULL;

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

IWDG_HandleTypeDef hiwdg;
TIM_HandleTypeDef htim1;


osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_IWDG_Init(void);

/* USER CODE BEGIN PFP */



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t tasksRunning;
static uint8_t taskAIsRunning=0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	BaseType_t status;
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
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_TIM1_Init();
  init_struct_timers();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */





  /* Init scheduler */
    SEGGER_SYSVIEW_Conf();


     		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);	//ensure proper priority grouping for freeRTOS

     		// Create a semaphore using the FreeRTOS Heap
     		semPtr = xSemaphoreCreateBinary();
     	    // Ensure the pointer is valid (semaphore created successfully)
     		assert_param(semPtr != NULL);


     		// Create TaskA as a higher priority than TaskB.  In this example, this isn't strictly necessary since the tasks
     		// spend nearly all of their time blocked

     		status=xTaskCreate(GreenTaskA, "GreenTaskA", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &ledATaskHandler);
     		assert_param(status == pdPASS);
     		//xTaskCreateStatic(GreenTaskA, "GreenTaskA", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, GreenTaskStack, &GreenTaskTCB);
     		// Using an assert to ensure proper task creation
     		//xTaskCreateStatic(BlueTaskB, "BlueTaskB", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, BlueTaskStack, &BlueTaskTCB);
     		status=xTaskCreate(BlueTaskB, "BlueTaskB", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
     		assert_param( status == pdPASS);

     		status=xTaskCreate(UartTaskC, "UartTaskC", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
     		assert_param( status == pdPASS);

     		status=xTaskCreate(vHandlingTaskTimer, "vHandlingTaskTimer", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &timerTaskHandler);
     		assert_param( status == pdPASS);
     		// Start the scheduler - shouldn't return unless there's a problem

			status=xTaskCreate(vTaskMonitor, "vTaskMonitor", STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, &watchdogTaskHandler);
			assert_param( status == pdPASS);




     		HAL_TIM_Base_Start_IT(&htim1);

     		vTaskStartScheduler();



     		// If you've wound up here, there is likely an issue with over-running the freeRTOS heap
     		while(1)
     		{
     		}

   }





   /**
    * GreenTaskA periodically 'gives' semaphorePtr
    * NOTES:
    * - This semaphore isn't "given" to any task specifically
    * - Giving the semaphore doesn't prevent GreenTaskA from continuing to run.
    * - Note the green LED continues to blink at all times
    */
   void GreenTaskA( void* argument )
   {
   	uint_fast8_t count = 0;
   	while(1)
   	{
   		//taskIsRunning=taskIsRunning|1;


   		// Every 5 times through the loop, give the semaphore
   		if(++count >= 5)
   		{
   			count = 0;
   			SEGGER_SYSVIEW_PrintfHost("GreenTaskA gives semPtr");
   			xSemaphoreGive(semPtr);
   		}
   		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
   		vTaskDelay(100/portTICK_PERIOD_MS);
   		//vTaskDelay(pdMS_TO_TICKS(100));
   		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
   		vTaskDelay(100/portTICK_PERIOD_MS);
   		//vTaskDelay(pdMS_TO_TICKS(100));
   		taskAIsRunning=1;
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
   				//vTaskDelay(pdMS_TO_TICKS(50));
   				vTaskDelay(50/portTICK_PERIOD_MS);
   				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
   				vTaskDelay(50/portTICK_PERIOD_MS);
   				//vTaskDelay(pdMS_TO_TICKS(500));
   			}
   		}
   		else
   		{
   		//	This is the code that would be executed if we timed-out waiting for
   		//	the semaphore to be given.
   			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
   			vTaskDelay(100/ portTICK_PERIOD_MS);
   			//vTaskDelay(pdMS_TO_TICKS(500));
   			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

   		}
   	}
   }

   void UartTaskC( void* argument )
      {
      	while(1)
      	{
      		uint8_t temp_buffer[64];
      	//	uint16_t temp_buffer[sizeof(send_esp)];

			size_t read_bytes;
			SEGGER_SYSVIEW_PrintfHost("UartTaskC started");
			read_bytes =get_rx_data(temp_buffer, sizeof(temp_buffer), 100);

			//read_bytes =get_rx_data("data to print", sizeof("data to print"), 100);
			SEGGER_SYSVIEW_PrintfHost("read");
			put_tx_data_with_wait(temp_buffer,read_bytes);

			//put_tx_data_with_wait("data to print",read_bytes);
			SEGGER_SYSVIEW_PrintfHost("write");
      	}
      }

   void vHandlingTaskTimer( void *pvParameters )
   {
   uint32_t ulInterruptStatus;

   	   while(1)
       {
   		vTaskSuspend(NULL);
   		SEGGER_SYSVIEW_PrintfHost("vHandlingTaskTimer notification call");
   		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   		SEGGER_SYSVIEW_PrintfHost("GPIO_PIN_RESET");
       }

//   		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
//    	   /* Block indefinitely (without a timeout, so no need to check the function's
//    	           return value) to wait for a notification.  NOTE!  Real applications
//    	           should not block indefinitely, but instead time out occasionally in order
//    	           to handle error conditions that may prevent the interrupt from sending
//    	           any more notifications. */
//    	           xTaskNotifyWait( 0,                  /* Wait for 0th Notificaition */
//    	                                   0,          /* Clear all bits on exit. */
//    	                                   &ulInterruptStatus, /* Receives the notification value. */
//    	                                   portMAX_DELAY );    /* Block indefinitely. */
//
//
//    	          if(( ulInterruptStatus & 0x01 ) != 0x00 ){
//    	        	  SEGGER_SYSVIEW_PrintfHost("vHandlingTaskTimer condition OK");
//    	          }
//
//
//       }


   }

   void vTaskMonitor( void *pvParameters )
      {



      	   while(1)
          {
      		 if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET){
      		 vTaskDelay(1000/ portTICK_PERIOD_MS);
      		   if(taskAIsRunning==1){
      			 taskAIsRunning=0;
      			__HAL_RCC_CLEAR_RESET_FLAGS();
      		   }else{
      			 vTaskSuspend(ledATaskHandler);
      		   }
//
      		 }
      		vTaskDelay(10/ portTICK_PERIOD_MS);
      		HAL_IWDG_Init(&hiwdg);
      		__HAL_IWDG_START(&hiwdg);
      		   int counter;
//      		   for(int i=0;i<50000;i++){
//      			   counter++;
//      		   }

      		 vTaskDelay(10/ portTICK_PERIOD_MS);
      			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
      			SEGGER_SYSVIEW_PrintfHost("WDT LED ON ");
      			vTaskDelay(10/ portTICK_PERIOD_MS);
//      		 if(taskIsRunning!=0)
//      		 {
//      			 SEGGER_SYSVIEW_PrintfHost("confirm task OK");
//      		 }

          }




      }

   /*****************************************************************************/
   /*****************************************************************************/
   void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
   //void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim){

	  // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

   	  if (htim->Instance == TIM9) {
   	    HAL_IncTick();
   	  }
   /*****************************************************************************/
   	if (htim->Instance==TIM1){ //check if the interrupt comes from TIM1

   		SEGGER_SYSVIEW_PrintfHost("Interrupt");

   		 // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
   		SEGGER_SYSVIEW_PrintfHost("GPIO_PIN_SET");
//   		BaseType_t xHigherPriorityTaskWoken;
//   		uint32_t ulStatusRegister;
//   		BaseType_t xTaskNotifyFromISR;
//   		xTaskNotifyFromISR( vHandlingTaskTimer,
//				   0,
//				   eSetBits,
//				   &xHigherPriorityTaskWoken );
////				 The macro used to do this is dependent on the port and may be called
////				     portEND_SWITCHING_ISR. */
//   			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
   		  BaseType_t checkIfYieldRequired;
   		  checkIfYieldRequired = xTaskResumeFromISR(timerTaskHandler);
   		  portYIELD_FROM_ISR(checkIfYieldRequired);

   		timer.f_10ms[0]=1;
   				timer.f_10ms[1]=1;
   				timer.f_10ms[2]=1;
   				timer.f_10ms[3]=1;
   				timer.f_10ms[4]=1;
   				timer.f_10ms[5]=1;
   				timer.f_10ms[6]=1;
   				timer.f_10ms[7]=1;

   				timer.count[0]++;
   				timer.count[1]++;
   				timer.count[2]++;




   		    // timer envent 100ms
   		    if(timer.count[0]>=10){
   		        timer.count[0]=0;
   				timer.f_100ms[0]=!timer.f_100ms[0];
   		        timer.f_100ms[1]=1;
   		        timer.f_100ms[2]=1;
   		        timer.f_100ms[3]=1;
   		        timer.f_100ms[4]=1;
   		        timer.f_100ms[5]=1;
   		     SEGGER_SYSVIEW_PrintfHost("timer event 100ms");

   						 //PutsU1("\n Test 100ms\0");
   		    }



   		    // timer envent 500ms
   		    if(timer.count[1]>=50){
   		         timer.count[1]=0;
   		         timer.f_500ms[0]=!timer.f_500ms[0];
   		         timer.f_500ms[1]=1;
   		         timer.f_500ms[2]=1;
   		         timer.f_500ms[3]=1;
   		         timer.f_500ms[4]=1;
   		         timer.f_500ms[5]=1;
   						 //PutsU1("\n Test 500ms\0");
   		      SEGGER_SYSVIEW_PrintfHost("timer event 500ms");
   		    }


   		    // timer envent 1000ms
   		    if(timer.count[2]>=100){
   		         timer.count[2]=0;
   		         timer.f_1000ms[0]=!timer.f_1000ms[0];
   		         timer.f_1000ms[1]=1;
   		         timer.f_1000ms[2]=1;
   		         timer.f_1000ms[3]=1;
   		         timer.f_1000ms[4]=1;
   		         timer.f_1000ms[5]=1;
   						// PutsU1("\n Test 1000ms\0");
   		      SEGGER_SYSVIEW_PrintfHost("timer event 1000ms");

   		   		}


   		}

   }


  /* @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 200;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 3599;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */




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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
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


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM9 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */


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

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */
typedef struct{
	uint8_t login;
	uint8_t status;
	uint8_t mode;
	float sensor_voltage;
	float sensor_current;
	float sensor_resistance;
	uint8_t master;
	uint8_t slave;
	uint8_t network;
	uint8_t n_slaves;
	uint8_t p1;
	uint8_t thc;
	uint8_t p23;
	uint16_t voltage;
	uint16_t ntc;
	uint16_t ntc_b;
	float ts1;
  float temp_bc;
  float temp_ac;
	float int_isl;
	float int_ish;
	float int_osl;
	float int_osh;
  float ext1_bc;
  float ext1_ac;
	float ext1_isl;
	float ext1_ish;
	float ext1_osl;
	float ext1_osh;
	float ext2_bc;
  float ext2_ac;
	float ext2_isl;
	float ext2_ish;
	float ext2_osl;
	float ext2_osh;
	float vacuum_bc;
	float vacuum_ac;
	float vacuum_bc_v;
  float vacuum_ac_v;
	float vac_isl;
	float vac_ish;
	float vac_osl;
	float vac_osh;
  float vacuum_v_isl;
	float vacuum_v_ish;
	float vacuum_v_osl;
	float vacuum_v_osh;
	float int_s1;
	float ext_s1;
	float ext_s2;
	int vacuum;
	uint8_t en_stp1;
	uint8_t en_stp2;
	uint8_t en_stp3;
	uint8_t en_stp4;
	uint8_t en_stp5;
	float tsp1;
	float tsp2;
	float tsp3;
	float tsp4;
	float tsp5;
	float ramp1;
	float ramp2;
	float ramp3;
	float ramp4;
  float ramp5;
	float tol1;
  float tol2;
  float tol3;
  float tol4;
  float tol5;
	uint8_t min1;
	uint8_t min2;
	uint8_t min3;
	uint8_t min4;
	uint8_t min5;
	float kp_gain;
  float ti_gain;
  float td_gain;
  uint16_t period;
	float kp_prop;
	float ti_prop;
	float kp_integral;
	float ti_integral;
	float kp_deriv;
	float ti_deriv;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t step;
	uint8_t thermostat;
	uint8_t admin;
	uint8_t user;
	float w_sp;
	float sp;
	float ramp;
	float tolerance;
	float pid_output;
	uint16_t countdown;
	float red_high;
	float red_low;
	float yellow_high;
	float yellow_low;
	uint8_t alarm_thermostat;
	char name[30];
}TO_ESP;

extern  TO_ESP send_esp;
/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void put_tx_data_with_wait(uint8_t *buffer, size_t buffer_length);
int put_tx_data(uint8_t *buffer, size_t buffer_length);
int get_rx_data(uint8_t *buffer, size_t buffer_length, uint32_t timeout);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

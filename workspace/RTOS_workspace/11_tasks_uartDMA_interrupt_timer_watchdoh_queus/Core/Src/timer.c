/* Timer.c */
/* Pedro Santos */

#include "timer.h"
#include "stm32F4xx_hal.h"
#include "stm32F4xx.h"
#include "stm32F4xx_it.h"
#include "main.h"
#include "freeRTOS.h"
#include "task.h"



extern TIM_HandleTypeDef htim2;	// Timer
mytime timer;
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/
void timer_flux_disable_interrupt(void){

		HAL_TIM_Base_Stop_IT(&htim2);
		//__HAL_TIM_SET_COUNTER(&htim1,0);
}

/*****************************************************************************/
/*****************************************************************************/
void timer_flux_enable_interrupt(void){
		//__HAL_TIM_SET_COUNTER(&htim1,0);
		HAL_TIM_Base_Start_IT(&htim2);
}



/**************************************************************************/
	void init_struct_timers(void)
	{
		timer.count[0]=0;
		timer.count[1]=0;
		timer.count[2]=0;

		timer.f_100ms[0]=0;
		timer.f_100ms[1]=0;
		timer.f_100ms[2]=0;
		timer.f_100ms[3]=0;
		timer.f_100ms[4]=0;
		timer.f_100ms[5]=0;
		timer.f_100ms[6]=0;
		timer.f_100ms[7]=0;
		timer.f_100ms[8]=0;
		timer.f_100ms[9]=0;
		timer.f_100ms[10]=0;
		timer.f_100ms[11]=0;
		timer.f_100ms[12]=0;


		timer.f_500ms[0]=0;
		timer.f_500ms[1]=0;
		timer.f_500ms[2]=0;
		timer.f_500ms[3]=0;
		timer.f_500ms[4]=0;
		timer.f_500ms[5]=0;

		timer.f_1000ms[0]=0;
		timer.f_1000ms[1]=0;
		timer.f_1000ms[2]=0;
		timer.f_1000ms[3]=0;
		timer.f_1000ms[4]=0;
		timer.f_1000ms[5]=0;
		}


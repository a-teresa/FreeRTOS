#include "tim.h"
#include "systick.h"
#include "uart.h"
#include <math.h>

#define PIN0		(1U<<0)

volatile uint32_t pwm_sig;

uint32_t g_before,g_after,g_time_taken;
double ans;

double weight  = 0.5;
double input = 0.5;
double expected_value  = 0.8;
double step_amount = 0.001;

void brute_force_learning( double input,double weight,double expected_value,double step_amount,uint32_t itr);
/*
 * How long it takes to compute the 'brute_force_learning' function as is presented in the files
 * => number_of_cycle X time_for_a_single_cycle => 1917129 *62.5x10-9 = 0.1198s = 119.9ms
*/
int main(void)
{
	uart2_tx_init();

	/*Systick Init*/
	systick_counter_init();

	/*Configure PA8 as output pin*/
	RCC->AHB1ENR  |= (1U<<0);

	GPIOA->MODER |=(1U<<16);
	GPIOA->MODER &=~(1U<<17);

	/*Enable FPU*/
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));



	g_before =  SysTick->VAL;

	brute_force_learning(input,weight,expected_value,step_amount,1500);

	g_after =  SysTick->VAL;

	g_time_taken = g_before - g_after;

	while(1)
	{

		 /*Set PA8 HIGH*/
		GPIOA->ODR |=(1U<<8);

		brute_force_learning(input,weight,expected_value,step_amount,1500);

		 /*Set PA8 LOW*/
		GPIOA->ODR &= ~(1U<<8);
	}

}


void brute_force_learning( double input,double weight,double expected_value,double step_amount,uint32_t itr)
{
   double prediction,error;
   double up_prediction,down_prediction,up_error, down_error;

	 for(int i=0;i<itr;i++){

		 prediction  = input * weight;
		 error = powf((prediction- expected_value),2);

		//printf("Error : %f    Prediction : %f \r\n",error,prediction);

		 up_prediction =  input * (weight +step_amount);
		 up_error      =   powf((up_prediction- expected_value),2);

		 down_prediction =  input * (weight - step_amount);
		 down_error      =  powf((down_prediction - expected_value),2);

		 if(down_error <  up_error)
			   weight  = weight - step_amount;
		 if(down_error >  up_error)
			   weight = weight  + step_amount;



	 }

}



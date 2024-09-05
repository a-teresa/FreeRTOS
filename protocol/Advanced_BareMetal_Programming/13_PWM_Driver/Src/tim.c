#include "tim.h"

#define GPIOAEN			(1U<<0)
#define TIM2EN			(1U<<0)
#define CR1_CEN			(1U<<0)




void tim2_pa0_pa1_pwm_init(void)
{
	/*********GPIO Configuration***************/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA0 and PA1 mode to alternate function mode*/
	/*PA0*/
	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER |= (1U<<1);

	/*PA1*/
	GPIOA->MODER &= ~(1U<<2);
	GPIOA->MODER |= (1U<<3);

	/*Set alternate function type to TIM2 (AF01)*/
	/*PA0*/
	GPIOA->AFR[0] |=(1U<<0);
	GPIOA->AFR[0] &=~(1U<<1);
	GPIOA->AFR[0] &=~(1U<<2);
	GPIOA->AFR[0] &=~(1U<<3);


	/*PA1*/
	GPIOA->AFR[0] |=(1U<<4);
	GPIOA->AFR[0] &=~(1U<<5);
	GPIOA->AFR[0] &=~(1U<<6);
	GPIOA->AFR[0] &=~(1U<<7);

	/*********TIM2 Configuration***************/

	/*Enable clock access to TIM2*/
	RCC->APB1ENR |=TIM2EN;

	/*Set timer Prescaler*/
	TIM2->PSC = 0 ;    // 16 000 000 no divider  = 16 000 000

	/*Set auto-reload valaue*/
	TIM2->ARR =  100 - 1;  // 16 000 000 / 100  =  160kHz

	/*Reset the timer count*/
	TIM2->CNT = 0;

	/*Set PA1 (ch2) and PA0 (ch1) to 110 : PWM mode 1*/

	/*ch1*/
	TIM2->CCMR1 &=~(1U<<4);
	TIM2->CCMR1 |= (1U<<5);
	TIM2->CCMR1 |= (1U<<6);

	/*ch2*/
	TIM2->CCMR1 &=~(1U<<12);
	TIM2->CCMR1 |= (1U<<13);
	TIM2->CCMR1 |= (1U<<14);

	/*Enable ch1 and ch2*/
	/*ch1*/
	TIM2->CCER |=(1U<<0);

	/*ch2*/
	TIM2->CCER |=(1U<<4);

	/*Enable timer*/
	TIM2->CR1 |=CR1_CEN;

}


void tim2_pa0_pa1_pwm_set_dutycycle(uint8_t ch, uint32_t dutycycle)
{
	/*Select channel*/
	switch(ch)
	{

	   /*Set dutycycle*/

		case 1:
			TIM2->CCR1 = dutycycle;
			break;
		case 2:
			TIM2->CCR2 = dutycycle;
		default:
			break;
	}
}



void pseudo_dly(void)
{
	for(int i = 0; i < 17000; i++){}
}

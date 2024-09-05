#include "tim.h"
#include "systick.h"

#define PIN0		(1U<<0)

volatile uint32_t pwm_sig;

int main(void)
{
	/*Init pwm at pa0 and pa1*/
	tim2_pa0_pa1_pwm_init();

	/*Systick Init*/
	systick_5kHz_interrupt();



	tim2_pa0_pa1_pwm_set_dutycycle(1,30);


	while(1)
	{



	}

}

static void systick_callback(void)
{
	pwm_sig =  (uint32_t)(GPIOA->IDR & PIN0);
}
void SysTick_Handler(void)
{
	//Do something...
	systick_callback();
}

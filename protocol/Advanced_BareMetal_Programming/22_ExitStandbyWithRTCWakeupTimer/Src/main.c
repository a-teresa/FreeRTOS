#include "led.h"
#include "uart.h"
#include "standby_mode.h"
#include "rtc.h"
#include "gpio_exti.h"
#include <stdio.h>

uint8_t g_btn_press;

static void wait_btn_press(void);


int main(void)
{
	uart2_tx_init();
	gpio_interrupt_init();
	led_init();
	rtc_wkup_timer_init();

	/*Check if systems resumed and check source*/
	 if ((PWR->CSR & PWR_CSR_SBF) != (PWR_CSR_SBF))
	 {
		 /*Run after normal reset*/
		 wait_btn_press();
		 printf("System going into StandBy......\n\r");

		 /*Enter standby with RTC WakeUp Timer enabled*/
		 enter_standby_mode_rtc_wkup();

	 }
	 else
	 {
		 /*Clear Standby flag*/
		 PWR->CR |=PWR_CR_CSBF;

		 /*Reset the internal wakeup flag*/
		 RTC->ISR &= ~RTC_ISR_WUTF;

		 printf("System back from StandBy!!!--------\n\r");

	 }

	while(1)
	{

	}

}


static void wait_btn_press(void)
{
	while(g_btn_press == 0)
	{
		GPIOA->ODR ^=(1U<<5);
		for(int i = 0; i < 90000; i++){}
	}
	GPIOA->ODR &= ~(1U<<5);
}




static void btn_callback(void)
{
	printf("--------------------BTN Pressed-----------------\n\r");
	g_btn_press = 1;
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & EXTI_IMR_IM13) == (EXTI_IMR_IM13))
	{
		 /*Clear EXTI flag*/
		 EXTI->PR = EXTI_IMR_IM13;

		 //Do something...
		 btn_callback();
	}
}



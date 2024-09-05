#include "led.h"
#include "uart.h"
#include "iwdg.h"
#include "gpio_exti.h"
#include <stdio.h>

uint8_t g_btn_press;
static void check_reset_source(void);

int main(void)
{

	uart2_tx_init();
	gpio_interrupt_init();
	led_init();
    check_reset_source();
	iwdg_init();
	while(1)
	{
         if( g_btn_press != 1)
         {
        	  /*Refresh IWDG down-counter to default value*/
        	 IWDG->KR = IWDG_KEY_RELOAD;
        	 led_blink();
         }
	}

}




static void check_reset_source(void)
{
	  if ((RCC->CSR & RCC_CSR_IWDGRSTF) == (RCC_CSR_IWDGRSTF))
	  {
		  /*Clear IWDG Reset flag*/
		  RCC->CSR = RCC_CSR_RMVF;

		  /*Turn LED On*/
		  led_on();
		  printf("RESET was caused by IWDG.....\n\r");

		  while( g_btn_press != 1)
		  {

		  }

		  g_btn_press =  0;

	  }
}

static void btn_callback(void)
{
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

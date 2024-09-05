#include "led.h"
#include "uart.h"
#include "standby_mode.h"
#include "gpio_exti.h"
#include <stdio.h>

uint8_t g_btn_press;
static void check_reset_source(void);
void clock_config(void);
static void led_blink_forever(void);

/*Press the blue push-button to enter StandBy mode*/
/**@note : PA0 is wakeup pin and it is active low
 * connect a jumper wire from PA0 to ground in normal mode,
 * pull out jumper wire and aconnect it to 3.3v to cause a change in logic which will
 * in turn trigger the WAKEUP**/


int main(void)
{
	uart2_tx_init();
	wakeup_pin_init();
	check_reset_source();
	gpio_interrupt_init();
	led_init();

	led_blink_forever();
	while(1)
	{

	}

}




static void led_blink_forever(void)
{
	while(1)
	{
		GPIOA->ODR ^=(1U<<5);
		for(int i = 0; i < 90000; i++){}
	}
}

static void check_reset_source(void)
{

	    /*Enable clock access to PWR*/
	     RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	  if ((PWR->CSR & PWR_CSR_SBF) == (PWR_CSR_SBF))
	  {
		  /*Clear Standby flag*/
		  PWR->CR |= PWR_CR_CSBF;

		  led_blink();
		  printf("System resume from Standby.....\n\r");

		  /*Wait for wakeup pin to be released*/
		  while(get_wakeup_pin_state() == 0){}

	  }

	  /*Check and Clear Wakeup flag*/
	  if((PWR->CSR & PWR_CSR_WUF) == PWR_CSR_WUF )
	  {
		  PWR->CR |= PWR_CR_CWUF;
	  }
}

static void btn_callback(void)
{
	standby_wakeup_pin_setup();
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



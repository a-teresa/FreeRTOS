#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"


#define		GPIOCEN		(1U<<2)
#define 	PIN13		(1U<<13)
#define 	BTN_PIN		PIN13

uint8_t button_state;

int main(void)
{
	/*Enable clock access to GPIOC*/
	RCC->AHB1ENR |= GPIOCEN;

	/*Set PC13 as input pin*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

	while(1)
	{
			/*Check if BTN is pressed or not*/
		    /*Push button is active low*/
			if(GPIOC->IDR &  BTN_PIN)
			{
				button_state = 0;
			}
			else
			{
				button_state = 1;
			}

			  for(int i = 0; i < 90000; i++){}
	}
}

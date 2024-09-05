#include "led.h"

void led_init(void)
{
    /*Enable clock for GPIOA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*Set PA5 as output pin*/
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

}


void led_on(void)
{
	GPIOA->ODR |= (1U<<5);
}

void led_off(void)
{
	GPIOA->ODR &= ~(1U<<5);

}


void led_blink(void)
{
	GPIOA->ODR ^= (1U<<5);
	for(int i=0;i<90000;i++){}
}

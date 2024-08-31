#include "uart.h"
void USART2_init(void){
	//1 - enable clock access to uart2
	RCC->APB1ENR |= 0x2000;

	//2 - enable clock access to port A
	RCC->AHB1ENR = 0x01;

	//3 - enable pins for alternate functions, pa2, pa3
	GPIOA->MODER &=~0x00F0;
	GPIOA->MODER |= 0x00A0;//enable alt func for PA2 and PA3

	//4 - configure type of alternate function
	GPIOA->AFR[0] &=~0xFF00;
	GPIOA->AFR[0] |=0x7700;

	// configure uart

	USART2->BRR = 0x0683;
	USART2->CR1 = 0x000C; //enable tx rx 8 bit data
	USART2->CR2 = 0x000;
	USART2->CR3 = 0x000;
	USART2->CR1 |= 0x2000; //enable uart



}

int USAR2_write(int ch){
	while(!(USART2->SR & 0x0080)){
		USART2->DR= (ch & 0xFF);

		return ch;
	}
}


int USART2_read(void){
	while(!(USART2->SR & 0x0020)){
	return USART2->DR;
	}
}



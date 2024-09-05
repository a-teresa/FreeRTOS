#include "uart.h"


#define UART2EN					(1U<<17)
#define GPIOAEN					(1U<<0)
#define CR1_TE					(1U<<3)
#define CR1_RE					(1U<<2)
#define CR1_UE					(1U<<13)
#define SR_TXE					(1U<<7)


#define UART_BAUDRATE 			115200
#define CLK 					72000000


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void usart2_write(int ch);


int __io_putchar(int ch)
{
	usart2_write(ch);
	return ch;
}



void uart2_tx_init(void)
{
	/********** Configure UART GPIO pin******************/
	/*1 - Enable clock access to GPIOA*/
	RCC -> AHB1ENR=GPIOAEN;

	/*2 - Set PA2 to alternate function mode */

	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);

	/*3 - Set PA2 to alternate function type to AF7 (UART2_TX) */

	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);


	/********** Configure UART module ******************/

	/*4 - Enable clock access to UART2 */
	RCC ->APB1ENR |= UART2EN;
	/*5 - Set baud rate*/
	uart_set_baudrate(CLK,UART_BAUDRATE);
	/*6 - Set transfer direction*/

	USART2->CR1 = CR1_TE;

	/*7 - Enable uart module*/
	USART2-> CR1 |= CR1_UE;


}

static void usart2_write(int ch)
{
	/*make shure transmit data is empty*/
	while(!(USART2->SR & SR_TXE)){};
	/*write to transmit data register*/
	USART2->DR=(ch & 0XFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART1->BRR = compute_uart_bd(periph_clk, baudrate);
}

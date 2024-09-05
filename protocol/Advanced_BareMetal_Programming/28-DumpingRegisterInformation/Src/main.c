#include "tim.h"
#include "systick.h"
#include "uart.h"
#include <math.h>


#define BUFF_SIZE		30

uint32_t PORTA_DATA_BUFF[BUFF_SIZE];

uint32_t counter;


void dump_porta_data(void)
{
	if(counter < BUFF_SIZE)
	{
		PORTA_DATA_BUFF[counter] =  GPIOA->IDR;
		counter++;
	}
}
int main(void)
{
	uart2_tx_init();


    /*Enable clock access to GPIOA*/
	RCC->AHB1ENR  |= (1U<<0);


	/*Enable FPU*/
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));




	while(1)
	{

		dump_porta_data();

	}

}




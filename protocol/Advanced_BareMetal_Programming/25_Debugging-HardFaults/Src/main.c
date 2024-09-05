#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"

#define CFSR_UFSR_DIVIDE_BY_ZERO		(1U<<25)

uint32_t exception_code;


extern uint16_t adc_raw_data[NUM_OF_CHANNELS];


uint32_t pressure_sensor, final_value;


int main(void)
{
	pressure_sensor = 0;

	uart2_tx_init();
	adc_dma_init();

	while(1)
	{


			printf("Value from sensor one : %d \n\r ",adc_raw_data[0]);
			printf("Value from sensor two : %d \n\r ",adc_raw_data[1]);

			for( int i = 0; i < 90000; i++){}

		    pressure_sensor = adc_raw_data[0];

		    final_value  =  10000 / pressure_sensor;




	}
}


void HardFault_Handler(void)
{
	exception_code  = SCB->CFSR;

	if(exception_code == CFSR_UFSR_DIVIDE_BY_ZERO)
	{
		printf("Divide By Zero Error \n\r");
	}
}


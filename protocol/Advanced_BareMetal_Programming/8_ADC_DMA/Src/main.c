#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"

extern uint16_t adc_raw_data[NUM_OF_CHANNELS];



int main(void)
{

	uart2_tx_init();
	adc_dma_init();

	while(1)
	{
			printf("Value from sensor one : %d \n\r ",adc_raw_data[0]);
			printf("Value from sensor two : %d \n\r ",adc_raw_data[1]);

			for( int i = 0; i < 90000; i++){}

	}
}



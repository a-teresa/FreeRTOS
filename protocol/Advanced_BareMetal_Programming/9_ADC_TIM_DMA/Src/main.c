#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"

extern uint16_t adc_raw_data[NUM_OF_SAMPLES];


volatile uint8_t g_transfer_cmplt;

int main(void)
{
	g_transfer_cmplt = 0;
	uart2_tx_init();
	adc_tim_dma_init();

	while(1)
	{
		if(g_transfer_cmplt)
		{
			g_transfer_cmplt = 0;

			for( int i = 0; i < NUM_OF_SAMPLES; i++)
			{
				printf("Sample number [%d]  = %d \r\n ",i,adc_raw_data[i]);
			}
			for( int i = 0; i < 90000; i++){}

		}

	}
}


void DMA2_Stream0_IRQHandler(void)
{

	/*Check if transfer complete interrupt occurred*/
	if((DMA2->LISR) & LISR_TCIF0)
	{
		g_transfer_cmplt = 1;

		/*Clear flag*/
		DMA2->LIFCR |=LIFCR_CTCIF0;
	}

}



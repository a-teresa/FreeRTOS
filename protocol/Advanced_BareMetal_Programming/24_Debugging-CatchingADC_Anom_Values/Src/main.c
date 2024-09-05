#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"

#define ANOMALY_THRESHOLD_HIGH		4000
#define ANOMALY_THRESHOLD_LOW		10

extern uint16_t adc_raw_data[NUM_OF_CHANNELS];


uint32_t pressure_sensor;


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

		    if(  pressure_sensor > ANOMALY_THRESHOLD_HIGH )
		    {
		    	__asm("BKPT #1\n");
		    }

		    else if(pressure_sensor < ANOMALY_THRESHOLD_LOW )
		    {
		    	__asm("BKPT #2\n");
		    }

	}
}



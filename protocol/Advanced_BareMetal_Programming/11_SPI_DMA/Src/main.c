#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart_dma.h"
#include "adc_dma.h"
#include "mpu9250.h"
#include <string.h>




float acc_x,acc_y,acc_z;


int main(void)
{


	/*Enable SPI*/
	spi1_dma_init();

	/*Config NCS pin*/
	mpu9250_ncs_pin_config();

	/*Enable Floating Point Unit*/
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

	/*Enable tx stream*/
	dma2_stream3_spi_tx_init();

	/*Enable rx stream*/
	dma2_stream2_spi_rx_init();

	/*Reset NCS pin*/
	mpu9250_ncs_pin_reset();

	/*Config accel*/
	mpu9250_accel_config(ACC_FULL_SCALE_2_G);

	while(1)
	{

		/*Reset NCS pin*/
		mpu9250_ncs_pin_reset();

        /*Update accel values*/
		mpu9250_accel_update();

		/*Get accel data*/
		acc_x =  mpu9250_get_x();
		acc_y =  mpu9250_get_y();
		acc_z =  mpu9250_get_z();

		/*Set NCS pin*/
		mpu9250_ncs_pin_set();
	}
}





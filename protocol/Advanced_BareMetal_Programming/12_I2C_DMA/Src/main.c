#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "adc_dma.h"
#include "mpu9250.h"
#include <string.h>




float acc_x,acc_y,acc_z;


int main(void)
{


	/*Enable I2C*/
	i2c1_init();


	/*Enable Floating Point Unit*/
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

	/*Enable tx stream*/
	dma1_stream6_i2c1_tx_init();

	/*Enable rx stream*/
	dma1_stream5_i2c1_rx_init();



	/*Config accel*/
	mpu9250_accel_config(ACC_FULL_SCALE_2_G);

	while(1)
	{



        /*Update accel values*/
		mpu9250_accel_update();

		/*Get accel data*/
		acc_x =  mpu9250_get_x();
		acc_y =  mpu9250_get_y();
		acc_z =  mpu9250_get_z();

	}
}





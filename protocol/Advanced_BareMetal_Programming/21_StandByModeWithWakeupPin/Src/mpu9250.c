#include "mpu9250.h"


/**@PINOUT

 *	PB8 :  SCL
 *	PB9 :  SDA
*/


#define mpu9250_addr		0x68

#define I2C_DATA_BUFF_LEN    2

#define USER_CTRL_I2C_IF_DIS		(1U<<4)
#define MAX_TRANSFER_LEN			6
#define READ_FLAG					0x80

#define GPIOAEN					(1U<<0)

uint8_t dummy_buff[MAX_TRANSFER_LEN + 1];
uint8_t accel_buff[MAX_TRANSFER_LEN + 1];

uint8_t i2c_data_buff[I2C_DATA_BUFF_LEN];
uint8_t g_tx_cmplt;
uint8_t g_rx_cmplt;

double g_accel_range;

void mpu9250_ncs_pin_config(void)
{
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA0 as output pin*/
	GPIOA->MODER |= (1U<<0);
	GPIOA->MODER &= ~(1U<<1);
}


void mpu9250_ncs_pin_set(void)
{
	GPIOA->ODR |=(1U<<0);
}


void mpu9250_ncs_pin_reset(void)
{
	GPIOA->ODR &= ~(1U<<0);
}


void mpu9250_accel_config(uint8_t mode)
{
	switch(mode)
	{
		case ACC_FULL_SCALE_2_G:
			g_accel_range = 2.0;
			break;

		case ACC_FULL_SCALE_4_G:
			g_accel_range = 4.0;
			break;

		case ACC_FULL_SCALE_8_G:
			g_accel_range = 8.0;
			break;

		case ACC_FULL_SCALE_16_G:
			g_accel_range = 16.0;
			break;
		default:
			break;
	}




	/*Configure the ACCEL Range*/
	i2c_data_buff[0] = MPU9250_ADDR_ACCELCONFIG;
	i2c_data_buff[1] = mode;

   /*Write mode to ACCELCONFIG register*/
	i2c_dma_write(mpu9250_addr,i2c_data_buff,I2C_DATA_BUFF_LEN);
}


void mpu9250_accel_update(void)
{

    /*Read ACCEL data*/
	i2c_dma_read(mpu9250_addr,MPU9250_ACCEL_XOUT_H,(uint8_t *)accel_buff,MAX_TRANSFER_LEN);

}
float mpu9250_accel_get(uint8_t high_idx, uint8_t low_idx)
{
	int16_t rslt;

	rslt  =  accel_buff[high_idx] << 8 | accel_buff[low_idx];
	if(rslt)
	{
		return ((float)- rslt) * g_accel_range / (float)0x8000;
	}
	else
	{
		return 0.0;
	}
}

float mpu9250_get_x(void)
{
	return mpu9250_accel_get(1,2);
}

float mpu9250_get_y(void)
{
	return mpu9250_accel_get(3,4);
}

float mpu9250_get_z(void)
{
	return mpu9250_accel_get(5,6);
}


#include "mpu9250.h"


/**@PINOUT

 *	PA5 :  SPI1_SCK
 *	PA6 :  SPI1_MISO
 *	PA7 :  SPI1_MOSI

  MPU--------------> STM32
	VCC        -       3.3V
	GND        -       GND
	NCS				 - PA0
	SCL				 - SCK/PA5
	SDI/SDA    -       MOSI/PA7
	SDO/ADO    -       MISO/PA6
*/


#define SPI_DATA_BUFF_LEN		2

#define USER_CTRL_I2C_IF_DIS		(1U<<4)
#define MAX_TRANSFER_LEN			6
#define READ_FLAG					0x80

#define GPIOAEN					(1U<<0)

uint8_t dummy_buff[MAX_TRANSFER_LEN + 1];
uint8_t accel_buff[MAX_TRANSFER_LEN + 1];

uint8_t spi_data_buff[SPI_DATA_BUFF_LEN];
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

	/*Set to SPI mode only*/
	spi_data_buff[0] = MPU9250_ADDR_USER_CTRL;
	spi_data_buff[1] = USER_CTRL_I2C_IF_DIS;

	dma2_stream3_spi_transfer((uint32_t) spi_data_buff, (uint32_t) SPI_DATA_BUFF_LEN);

	/*Wait for transfer completion*/
	while(!g_tx_cmplt){}

	/*Reset flag*/
	g_tx_cmplt = 0;


	/*Configure the ACCEL Range*/
	spi_data_buff[0] = MPU9250_ADDR_ACCELCONFIG;
	spi_data_buff[1] = mode;

	dma2_stream3_spi_transfer((uint32_t) spi_data_buff, (uint32_t) SPI_DATA_BUFF_LEN);

	/*Wait for transfer completion*/
	while(!g_tx_cmplt){}

	/*Reset flag*/
	g_tx_cmplt = 0;
}


void mpu9250_accel_update(void)
{
	dummy_buff[0] =  MPU9250_ACCEL_XOUT_H |READ_FLAG;

	dma2_stream2_spi_receive((uint32_t)accel_buff,(uint32_t)(MAX_TRANSFER_LEN + 1));

	dma2_stream3_spi_transfer((uint32_t) dummy_buff, (uint32_t)(MAX_TRANSFER_LEN + 1));


	/*Wait for reception completion*/
	while(!g_rx_cmplt){}

	/*Reset flag*/
	g_rx_cmplt = 0;


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
void DMA2_Stream3_IRQHandler(void)
{
	if((DMA2->LISR) & LISR_TCIF3)
	{
		//do something...
		g_tx_cmplt = 1;
		DMA2_Stream3->CR&=~DMA_SxCR_EN;
		//Clear the flag
		DMA2->LIFCR |=LIFCR_CTCIF3;

	}
	else if((DMA2->LISR) & LISR_TEIF3)
	{
        //do something...

		//Clear the flag
		DMA2->LIFCR |=LIFCR_CTEIF3;

	}

}

void DMA2_Stream2_IRQHandler(void)
{
	if((DMA2->LISR) & LISR_TCIF2)
	{
		//do something...
		g_rx_cmplt = 1;
		DMA2_Stream2->CR&=~DMA_SxCR_EN;
		//Clear the flag
		DMA2->LIFCR |=LIFCR_CTCIF2;

	}
	else if((DMA2->LISR) & LISR_TEIF2)
	{
        //do something...

		//Clear the flag
		DMA2->LIFCR |=LIFCR_CTEIF2;

	}

}

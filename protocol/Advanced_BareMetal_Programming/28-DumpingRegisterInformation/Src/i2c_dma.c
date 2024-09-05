#include "i2c_dma.h"


/*
DMA1_Stream5_channel 1 is I2C1_RX
DMA1_Stream6_channel 1 is I2C1_TX
PB8 : I2C1 SCL
PB9 : I2C1 SDA
*/


#define GPIOBEN				(1U<<1)
#define I2C1EN				(1U<<21)
#define CR1_SWRST			(1U<<15)
#define CR1_NOSTRETCH  		(1U<<7)
#define CR1_ENGC			(1U<<6)
#define CR2_DMAEN			(1U<<11)
#define CR2_LAST			(1U<<12)
#define CR1_PE				(1U<<0)
#define DMA1EN				(1U<<21)
#define DMA_SCR_MINC		(1U<<10)
#define DMA_SCR_TCIE		(1U<<4)
#define DMA_SCR_EN  		(1U<<0)

#define HIFCR_CTCIF6		(1U<<21)
#define HIFCR_CTCIF5		(1U<<11)


#define HIFSR_TCIF5		(1U<<11)
#define HIFSR_TCIF6		(1U<<21)

#define PERIPH_CLK			16

#define SR2_BUSY			(1U<<1)

#define I2C_100KHZ				80   // 0B 0101 0000
#define SD_MODE_MAX_RISE_TIME	17

#define CR1_START			(1U<<8)
#define SR1_SB				(1U<<0)
#define SR1_ADDR			(1U<<1)
#define SR1_TXE				(1U<<7)
#define CR1_ACK				(1U<<10)
#define SR1_BTF				(1U<<2)
#define CR1_STOP			(1U<<9)

uint8_t g_rx_cmplt;
uint8_t g_tx_cmplt;

void i2c1_init(void)
{
	/********I2C GPIO Configuration*************/
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= GPIOBEN;

	/*Set PB8 and PB9 mode to alternate function mode*/
	/*PB8*/
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);


	/*PB9*/
	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);


	/*Set PB8 and PB9 alternate function type to I2C1 (AF4)  */
	/*PB8*/
	GPIOB->AFR[1] &=~(1U<<0);
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |=(1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	/*PB9*/
	GPIOB->AFR[1] &=~(1U<<4);
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |=(1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);

	/*SCL and SDA respectively*/
	/*Set output type of PB8 and PB9 to open-drain*/
	GPIOB->OTYPER |=(1U<<8);
	GPIOB->OTYPER |=(1U<<9);

	/********I2C  Configuration*************/
	/*Enable clock access to I2C1*/
	RCC->APB1ENR |= I2C1EN;

	/*Reset I2C module*/
	I2C1->CR1 = CR1_SWRST;


	/*Release the reset*/
	I2C1->CR1 &= ~CR1_SWRST;

	/*Enable clock stretching*/
	I2C1->CR1 &=~CR1_NOSTRETCH;

	/*Disable General Call*/
	I2C1->CR1 &=~CR1_ENGC;

	/*Select to use DMA*/
	I2C1->CR2 |=CR2_DMAEN;

	/*Enable LAST*/
	I2C1->CR2 |=CR2_LAST;

	/*Set source clock speed*/
	I2C1->CR2 |=PERIPH_CLK;

	 /*Set I2C to standard mode, 100kHz clock*/
	I2C1->CCR = I2C_100KHZ; /*Based on Computation*/

	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/*Enable I2C module*/
	I2C1->CR1 |=CR1_PE;

}


void dma1_stream6_i2c1_tx_init(void)
{
	/*Enable clock access DMA*/
	RCC->AHB1ENR |=DMA1EN;

	/*Disable DMA stream*/
	DMA1_Stream6->CR = 0;

	/*Wait till DMA Stream is disabled*/
	while((DMA1_Stream6->CR & DMA_SCR_EN)){}


	/*Select DMA channel : CH1*/
	DMA1_Stream6->CR |=(1U<<25);
	DMA1_Stream6->CR &=~(1U<<26);
	DMA1_Stream6->CR &=~(1U<<27);

	/*Enable Mem Addr increment*/
	DMA1_Stream6->CR |=DMA_SCR_MINC;

	/*Enable Transfer Complete Interrupt*/
	DMA1_Stream6->CR |=DMA_SCR_TCIE;

	/*Set Transfer direction :  Mem to Periph*/
	DMA1_Stream6->CR |=(1U<<6);
	DMA1_Stream6->CR &=~(1U<<7);

	/*Enable Stream Interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

void dma1_stream5_i2c1_rx_init(void)
{
	/*Enable clock access DMA*/
	RCC->AHB1ENR |=DMA1EN;

	/*Disable DMA stream*/
	DMA1_Stream5->CR = 0;

	/*Wait till DMA Stream is disabled*/
	while((DMA1_Stream5->CR & DMA_SCR_EN)){}

	/*Select DMA channel : CH1*/
	DMA1_Stream5->CR |=(1U<<25);
	DMA1_Stream5->CR &=~(1U<<26);
	DMA1_Stream5->CR &=~(1U<<27);

	/*Enable Mem Addr increment*/
	DMA1_Stream5->CR |=DMA_SCR_MINC;

	/*Enable Transfer Complete Interrupt*/
	DMA1_Stream5->CR |=DMA_SCR_TCIE;

	/*Set Transfer direction :   Periph to Mem*/
	DMA1_Stream5->CR &=~(1U<<6);
	DMA1_Stream5->CR &=~(1U<<7);

	/*Enable Stream Interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

void dma1_stream6_i2c1_transfer(uint8_t *msg_to_send, uint32_t msg_len)
{
	if( NULL != msg_to_send )
	{

		/*Clear interrupt flags*/
		DMA1->HIFCR = HIFCR_CTCIF6;

		/*Set Peripheral address*/
		DMA1_Stream6->PAR =	(uint32_t)(&(I2C1->DR));

		/*Set Memory address*/
		DMA1_Stream6->M0AR = (uint32_t)msg_to_send;

		/*Set transfer length*/
		DMA1_Stream6->NDTR = msg_len;

		/*Enable the DMA Stream*/
		DMA1_Stream6->CR |= DMA_SCR_EN;

	}
	else
	{
		//Do something...
	}
}


void dma1_stream5_i2c1_receive(uint8_t * received_msg, uint32_t msg_len)
{
	if( NULL != received_msg )
	{

		/*Clear interrupt flags*/
		DMA1->HIFCR = HIFCR_CTCIF5;

		/*Set Peripheral address*/
		DMA1_Stream5->PAR =	(uint32_t)(&(I2C1->DR));

		/*Set Memory address*/
		DMA1_Stream5->M0AR = (uint32_t)received_msg;

		/*Set transfer length*/
		DMA1_Stream5->NDTR = msg_len;

		/*Enable the DMA Stream*/
		DMA1_Stream5->CR |= DMA_SCR_EN;

	}

	else
	{
		//Do something...
	}
}


void i2c_dma_write(uint8_t slave_addr, uint8_t *p_write_buff, uint16_t num_of_bytes)
{
	/*Wait while BUSY flag is set*/
	while((I2C1->SR2 & SR2_BUSY)){}

	/*Generate START condition*/
	I2C1->CR1 |= CR1_START;

	/*Wait until the SB flag  is set*/
	while(!(I2C1->SR1 & SR1_SB)){}

	/*Read SR1*/
	 I2C1->SR1;

	/*Send Slave Addr "Write"*/
	 I2C1->DR = (slave_addr<<1|0);

	/*Wait for Addr Flag to be set*/
	 while(!((I2C1->SR1)& SR1_ADDR)){}

	/*Call DMA transfer function*/
	 dma1_stream6_i2c1_transfer(p_write_buff,num_of_bytes);

	/*Read SR1*/
	 I2C1->SR1;

	/*Read SR2*/
	 I2C1->SR2;


}

void i2c_dma_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *p_read_buff, uint16_t num_of_bytes)
{
	/*Wait while BUSY flag is set*/
	while((I2C1->SR2 & SR2_BUSY)){}

	/*Generate START condition*/
	I2C1->CR1 |= CR1_START;

	/*Wait until the SB flag  is set*/
	while(!(I2C1->SR1 & SR1_SB)){}

	/*Read SR1*/
	 I2C1->SR1;

	/*Send Slave Addr "Write"*/
	 I2C1->DR = (slave_addr<<1|0);

	/*Wait for Addr Flag to be set*/
	 while(!((I2C1->SR1)& SR1_ADDR)){}

	/*Read SR1*/
	 I2C1->SR1;

    /*Read SR2*/
	 I2C1->SR2;

	/*Wait for TXE flag to be set*/
	while(!(I2C1->SR1 & SR1_TXE)){}

	/*Send RegAddr*/
	if(num_of_bytes >= 2)
	{
		/*Enable ACK*/
		I2C1->CR1 |=CR1_ACK;

		/*Send register address to read*/
		I2C1->DR = reg_addr;

	}
	else
	{
		/*Disable ACK*/
		I2C1->CR1 &=~CR1_ACK;

		/*Send register address to read*/
		I2C1->DR = reg_addr;
	}

	/*Wait for BTF flag*/
	while(!(I2C1->SR1 & SR1_BTF)){}

	/*Generate RESTART*/
	I2C1->CR1 |= CR1_START;

	/*Wait until the SB flag is set*/
	while(!(I2C1->SR1 & SR1_SB)){}

	/*Read SR1*/
	I2C1->SR1;

	/*Send Slave Addr with "read"*/
	 I2C1->DR = (slave_addr<<1|1);

	/*Wait for the Addr flag to be set*/
	 while(!((I2C1->SR1)& SR1_ADDR)){}

	/*Call DMA receive function*/
	 dma1_stream5_i2c1_receive(p_read_buff,num_of_bytes);

	/*Read SR1*/
	I2C1->SR1;

	/*Read SR2*/
	I2C1->SR2;

}


void DMA1_Stream6_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF6)
	{
		//do_ssomething
		g_tx_cmplt = 1;

		/*Generate STOP condition*/
		 I2C1->CR1 |= CR1_STOP;

		/*Clear the flag*/
		DMA1->HIFCR |= HIFCR_CTCIF6;
	}
}

void DMA1_Stream5_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF5)
	{

		g_rx_cmplt = 1;

		//do_ssomething

		/*Generate STOP condition*/
		I2C1->CR1 |= CR1_STOP;

		/*Clear the flag*/
		DMA1->HIFCR |= HIFCR_CTCIF5;

	}
}

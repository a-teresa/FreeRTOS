#include "spi_dma.h"


/*
 *  PA5 :  SPI1_SCK
 *	PA6 :  SPI1_MISO
 *	PA7 :  SPI1_MOSI
 *	*/

#define GPIOAEN			(1U<<0)
#define SPI1EN			(1U<<12)

#define CR1_SSM			(1U<<9)
#define CR1_SSI			(1U<<8)
#define CR1_MSTR		(1U<<2)
#define CR1_CPOL		(1U<<1)
#define CR1_PHA			(1U<<0)
#define CR2_TXDMAEN		(1U<<1)
#define CR2_RXDMAEN		(1U<<0)
#define CR1_SPE			(1U<<6)

#define DMA2EN				(1U<<22)

#define DMA_SCR_EN  		(1U<<0)
#define DMA_SCR_MINC		(1U<<10)
#define DMA_SCR_PINC		(1U<<9)
#define DMA_SCR_CIRC		(1U<<8)
#define DMA_SCR_TCIE		(1U<<4)
#define DMA_SCR_TEIE		(1U<<2)
#define DMA_SFCR_DMDIS		(1U<<2)

#define HIFCR_CDMEIF5		(1U<<8)
#define HIFCR_CTEIF5		(1U<<9)
#define HIFCR_CTCIF5		(1U<<11)

#define HIFCR_CDMEIF6		(1U<<18)
#define HIFCR_CTEIF6		(1U<<19)
#define HIFCR_CTCIF6		(1U<<21)

#define HIFSR_TCIF5		(1U<<11)
#define HIFSR_TCIF6		(1U<<21)


void spi1_dma_init(void)
{

	/*************GPIO Configuration*****************/
	/*Enable clock to PORT of SPI pins*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set SPI pins mode to alternate function mode*/
	/*PA5*/
	GPIOA->MODER &=~(1U<<10);
	GPIOA->MODER |=(1U<<11);

	/*PA6*/
	GPIOA->MODER &=~(1U<<12);
	GPIOA->MODER |=(1U<<13);

	/*PA7*/
	GPIOA->MODER &=~(1U<<14);
	GPIOA->MODER |=(1U<<15);

	/*Set alternate type to AF5(SPI1)*/
	/*PA5*/
	GPIOA->AFR[0]  |= (1U<<20);
	GPIOA->AFR[0]  &=~(1U<<21);
	GPIOA->AFR[0]  |= (1U<<22);
	GPIOA->AFR[0]  &=~(1U<<23);

	/*PA6*/
	GPIOA->AFR[0]  |= (1U<<24);
	GPIOA->AFR[0]  &=~(1U<<25);
	GPIOA->AFR[0]  |= (1U<<26);
	GPIOA->AFR[0]  &=~(1U<<27);

	/*PA7*/
	GPIOA->AFR[0]  |= (1U<<28);
	GPIOA->AFR[0]  &=~(1U<<29);
	GPIOA->AFR[0]  |= (1U<<30);
	GPIOA->AFR[0]  &=~(1U<<31);

	/*************SPI Configuration*****************/

	/*Enable clock access to SPI1 module*/
	RCC->APB2ENR |=SPI1EN;

	/*Set software slave management*/
	SPI1->CR1 |= CR1_SSM|CR1_SSI;

	/*Set SPI to master mode*/
	SPI1->CR1 |=CR1_MSTR;

	/*Set CPHA and CPOL*/
	SPI1->CR1 |=SPI_CR1_BR_0|CR1_PHA|CR1_CPOL;

	/*Set clock divider : PCLCK/4*/
	SPI1->CR1 |=(1U<<3);
	SPI1->CR1 &=~(1U<<4);
	SPI1->CR1 &=~(1U<<5);

	/*Select to use DMA*/
	SPI1->CR2 |=CR2_RXDMAEN|CR2_TXDMAEN;

    /*Enable SPI*/
	SPI1->CR1 |=CR1_SPE;
}

void dma2_stream3_spi_tx_init(void)
{
	/************DMA Configuration**********/
	/*Enable clock access to DMA*/
	RCC->AHB1ENR |= DMA2EN;

	/*Disable DMA stream*/
	DMA2_Stream3->CR = 0;

	/*Wait till DMA Stream is disabled*/
	while((DMA2_Stream3->CR & DMA_SCR_EN)){}

    /*******Configure DMA Stream parameters************/
	/*Enable memory addr increment*/
	DMA2_Stream3->CR |= DMA_SCR_MINC;

	/*Select Channel :  CH3*/
	DMA2_Stream3->CR |= (1U<<25);
	DMA2_Stream3->CR |= (1U<<26);
	DMA2_Stream3->CR &= ~(1U<<27);

	/*Set transfer direction : Memory to Peripheral*/
	DMA2_Stream3->CR |=(1U<<6);
	DMA2_Stream3->CR &=~(1U<<7);

	/*Enable transfer complete interrupt*/
	DMA2_Stream3->CR |= DMA_SCR_TCIE;

	/*Enable transfer error interrupt*/
	DMA2_Stream3->CR |= DMA_SCR_TEIE;

	/*Disable direct mode*/
	DMA2_Stream3->FCR |=DMA_SFCR_DMDIS;

	/*Set DMA FIFO threshold*/
	DMA2_Stream3->FCR |=(1U<<0);
	DMA2_Stream3->FCR |=(1U<<1);

	/*Enable DMA Stream interrupt in the NVIC*/
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}


void dma2_stream2_spi_rx_init(void)
{
	/************DMA Configuration**********/
	/*Enable clock access to DMA*/
	RCC->AHB1ENR |= DMA2EN;

	/*Disable DMA stream*/
	DMA2_Stream2->CR = 0;

	/*Wait till DMA Stream is disabled*/
	while((DMA2_Stream2->CR & DMA_SCR_EN)){}

    /*******Configure DMA Stream parameters************/
	/*Enable memory addr increment*/
	DMA2_Stream2->CR |= DMA_SCR_MINC;

	/*Select Channel :  CH3*/
	DMA2_Stream2->CR |= (1U<<25);
	DMA2_Stream2->CR |= (1U<<26);
	DMA2_Stream2->CR &= ~(1U<<27);

	/*Set transfer direction : Peripheral to Memory*/
	DMA2_Stream2->CR &=~(1U<<6);
	DMA2_Stream2->CR &=~(1U<<7);

	/*Enable transfer complete interrupt*/
	DMA2_Stream2->CR |= DMA_SCR_TCIE;

	/*Enable transfer error interrupt*/
	DMA2_Stream2->CR |= DMA_SCR_TEIE;

	/*Disable direct mode*/
	DMA2_Stream2->FCR |=DMA_SFCR_DMDIS;

	/*Set DMA FIFO threshold :  Full*/
	DMA2_Stream2->FCR |=(1U<<0);
	DMA2_Stream2->FCR |=(1U<<1);

	/*Enable DMA Stream interrupt in the NVIC*/
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}


void dma2_stream3_spi_transfer(uint32_t msg_to_send, uint32_t msg_len)
{
	/*Clear interrupt flags*/
	DMA2->LIFCR = LIFCR_CTCIF3 |LIFCR_CTEIF3;

	/*Set Peripheral address*/
	DMA2_Stream3->PAR =	(uint32_t)(&(SPI1->DR));

	/*Set Memory address*/
	DMA2_Stream3->M0AR = msg_to_send;

	/*Set transfer length*/
	DMA2_Stream3->NDTR = msg_len;

	/*Enable the DMA Stream*/
	DMA2_Stream3->CR |= DMA_SCR_EN;
}

void dma2_stream2_spi_receive(uint32_t received_msg, uint32_t msg_len)
{
	/*Clear interrupt flags*/
	DMA2->LIFCR = LIFCR_CTCIF2 |LIFCR_CTEIF2;

	/*Set Peripheral address*/
	DMA2_Stream2->PAR =	(uint32_t)(&(SPI1->DR));

	/*Set Memory address*/
	DMA2_Stream2->M0AR = received_msg;

	/*Set transfer length*/
	DMA2_Stream2->NDTR = msg_len;

	/*Enable the DMA Stream*/
	DMA2_Stream2->CR |= DMA_SCR_EN;

}


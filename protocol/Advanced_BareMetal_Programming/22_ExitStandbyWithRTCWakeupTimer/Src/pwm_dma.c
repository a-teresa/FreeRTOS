#include "pwm_dma.h"

#define GPIOAEN			(1U<<0)
#define TIM2EN			(1U<<0)
#define CR1_CEN			(1U<<0)

#define DIER_CC1DE		(1U<<9)
#define DIER_CC2DE		(1U<<10)
#define DIER_UDE		(1U<<8)
#define DIER_TDE		(1U<<14)

#define DMA1EN				(1U<<21)
#define DMA_SCR_MINC		(1U<<10)
#define DMA_SCR_TCIE		(1U<<4)
#define DMA_SCR_EN  		(1U<<0)
#define SC_CIRC				(1U<<8)

#define LOOKUP_TABLE_LEN		200

uint32_t ch1_duty_cycl_lookup[LOOKUP_TABLE_LEN]=
{
	0,50 ,100 ,151 ,201 ,250 ,300 ,349 ,398 ,446 ,494 ,542 ,589 ,635 ,681
	,726 ,771 ,814 ,857 ,899 ,940 ,981 ,1020 ,1058 ,1095 ,1131 ,1166 ,1200 ,1233 ,1264
	,1294 ,1323 ,1351 ,1377 ,1402 ,1426 ,1448 ,1468 ,1488 ,1505 ,1522 ,1536 ,1550 ,1561
	,1572 ,1580 ,1587 ,1593 ,1597 ,1599 ,1600 ,1599 ,1597 ,1593 ,1587 ,1580 ,1572 ,1561
	,1550 ,1536 ,1522 ,1505 ,1488 ,1468 ,1448 ,1426 ,1402 ,1377 ,1351 ,1323 ,1294 ,1264
	,1233 ,1200 ,1166 ,1131 ,1095 ,1058 ,1020 ,981 ,940 ,899 ,857 ,814 ,771 ,726 ,681 ,635
	,589 ,542 ,494 ,446 ,398 ,349 ,300 ,250 ,201 ,151 ,100 ,50,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
	,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
	,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0

};

uint32_t ch2_duty_cycl_lookup[LOOKUP_TABLE_LEN]=
{
	 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
	,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
	,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
	,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,50 ,100 ,151 ,201 ,250 ,300 ,349 ,398 ,446 ,494
	,542 ,589 ,635 ,681 ,726 ,771 ,814 ,857 ,899 ,940 ,981 ,1020 ,1058 ,1095 ,1131 ,1166 ,1200 ,1233
	,1264 ,1294 ,1323 ,1351 ,1377 ,1402 ,1426 ,1448 ,1468 ,1488 ,1505 ,1522 ,1536 ,1550 ,1561 ,1572 ,1580
	,1587 ,1593 ,1597 ,1599 ,1600 ,1599 ,1597 ,1593 ,1587 ,1580 ,1572 ,1561 ,1550 ,1536 ,1522 ,1505 ,1488
	,1468 ,1448 ,1426 ,1402 ,1377 ,1351 ,1323 ,1294 ,1264 ,1233 ,1200 ,1166 ,1131 ,1095 ,1058 ,1020 ,981

};
/*
DMA1_Stream5_channel 3 is TIM2_CH1
DMA1_Stream6_channel 3 is TIM2_CH2
PA0 : TIM2_CH1
PA1 : TIM2_CH2
*/
void tim2_pa0_pa1_pwm_dma_init(void)
{
	/*********GPIO Configuration***************/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA0 and PA1 mode to alternate function mode*/
	/*PA0*/
	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER |= (1U<<1);

	/*PA1*/
	GPIOA->MODER &= ~(1U<<2);
	GPIOA->MODER |= (1U<<3);

	/*Set alternate function type to TIM2 (AF01)*/
	/*PA0*/
	GPIOA->AFR[0] |=(1U<<0);
	GPIOA->AFR[0] &=~(1U<<1);
	GPIOA->AFR[0] &=~(1U<<2);
	GPIOA->AFR[0] &=~(1U<<3);


	/*PA1*/
	GPIOA->AFR[0] |=(1U<<4);
	GPIOA->AFR[0] &=~(1U<<5);
	GPIOA->AFR[0] &=~(1U<<6);
	GPIOA->AFR[0] &=~(1U<<7);

	/*********TIM2 Configuration***************/

	/*Enable clock access to TIM2*/
	RCC->APB1ENR |=TIM2EN;

	/*Set timer Prescaler*/
	TIM2->PSC = 0 ;    // 16 000 000 no divider  = 16 000 000

	/*Set auto-reload valaue*/
	TIM2->ARR =  1600 - 1;  // 16 000 000 / 1600  =  10000 = 10kHz

	/*Reset the timer count*/
	TIM2->CNT = 0;

	/*Set PA1 (ch2) and PA0 (ch1) to 110 : PWM mode 1*/

	/*ch1*/
	TIM2->CCMR1 &=~(1U<<4);
	TIM2->CCMR1 |= (1U<<5);
	TIM2->CCMR1 |= (1U<<6);

	/*ch2*/
	TIM2->CCMR1 &=~(1U<<12);
	TIM2->CCMR1 |= (1U<<13);
	TIM2->CCMR1 |= (1U<<14);

	/*Configure DMA Interrupt Enable Register*/
	TIM2->DIER |= DIER_CC1DE | DIER_CC2DE | DIER_UDE |DIER_TDE;

	/*Enable ch1 and ch2*/
	/*ch1*/
	TIM2->CCER |=(1U<<0);

	/*ch2*/
	TIM2->CCER |=(1U<<4);

	/*****Configure DMA Parameters***/
	/*Enable clock access DMA*/
	RCC->AHB1ENR |=DMA1EN;

    /*******Stream 5******/
	/*Disable DMA stream*/
	DMA1_Stream5->CR = 0;

	/*Wait till DMA Stream is disabled*/
	while((DMA1_Stream5->CR & DMA_SCR_EN)){}

	/*Select DMA channel : CH3*/
	DMA1_Stream5->CR |=(1U<<25);
	DMA1_Stream5->CR |=(1U<<26);
	DMA1_Stream5->CR &=~(1U<<27);

	/*Enable circular mode*/
	DMA1_Stream5->CR |= SC_CIRC;

	/*Enable Mem Addr increment*/
	DMA1_Stream5->CR |=DMA_SCR_MINC;

	/*Set Mem size :  32bit*/
	DMA1_Stream5->CR &=~(1U<<13);
	DMA1_Stream5->CR |=(1U<<14);

	/*Set Periph size :  32bit*/
	DMA1_Stream5->CR &=~(1U<<11);
	DMA1_Stream5->CR |=(1U<<12);

	/*Set Transfer direction : Mem to Periph */
	DMA1_Stream5->CR |=(1U<<6);
	DMA1_Stream5->CR &=~(1U<<7);

	/*Set number of transfer*/
	DMA1_Stream5->NDTR = (uint16_t)LOOKUP_TABLE_LEN;

	/*Set peripheral address*/
	DMA1_Stream5->PAR =  (uint32_t)(&TIM2->CCR1);
	/*Set memory address*/
	DMA1_Stream5->M0AR = (uint32_t)(&ch1_duty_cycl_lookup);

	/*Enable the DMA Stream*/
	DMA1_Stream5->CR |= DMA_SCR_EN;


    /*******Stream 6******/
	/*Disable DMA stream*/
	DMA1_Stream6->CR = 0;

	/*Wait till DMA Stream is disabled*/
	while((DMA1_Stream6->CR & DMA_SCR_EN)){}

	/*Select DMA channel : CH3*/
	DMA1_Stream6->CR |=(1U<<25);
	DMA1_Stream6->CR |=(1U<<26);
	DMA1_Stream6->CR &=~(1U<<27);

	/*Enable circular mode*/
	DMA1_Stream6->CR |= SC_CIRC;

	/*Enable Mem Addr increment*/
	DMA1_Stream6->CR |=DMA_SCR_MINC;

	/*Set Mem size :  32bit*/
	DMA1_Stream6->CR &=~(1U<<13);
	DMA1_Stream6->CR |=(1U<<14);

	/*Set Periph size :  32bit*/
	DMA1_Stream6->CR &=~(1U<<11);
	DMA1_Stream6->CR |=(1U<<12);

	/*Set Transfer direction : Mem to Periph */
	DMA1_Stream6->CR |=(1U<<6);
	DMA1_Stream6->CR &=~(1U<<7);

	/*Set number of transfer*/
	DMA1_Stream6->NDTR = (uint16_t)LOOKUP_TABLE_LEN;

	/*Set peripheral address*/
	DMA1_Stream6->PAR =  (uint32_t)(&TIM2->CCR2);
	/*Set memory address*/
	DMA1_Stream6->M0AR = (uint32_t)(&ch2_duty_cycl_lookup);

	/*Enable the DMA Stream*/
	DMA1_Stream6->CR |= DMA_SCR_EN;

	/*Enable timer*/
	TIM2->CR1 |=CR1_CEN;

}

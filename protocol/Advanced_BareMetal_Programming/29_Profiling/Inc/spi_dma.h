#ifndef __SPI_DMA_H__
#define __SPI_DMA_H__

#include <stdint.h>

#include "stm32f4xx.h"

#define LISR_TCIF3			(1U<<27)
#define LISR_TCIF2			(1U<<21)

#define LISR_TEIF3			(1U<<25)
#define LISR_TEIF2			(1U<<19)


#define LIFCR_CTCIF2	(1U<<21)
#define LIFCR_CTCIF3	(1U<<27)

#define LIFCR_CTEIF2	(1U<<19)
#define LIFCR_CTEIF3	(1U<<25)


void spi1_dma_init(void);
void dma2_stream3_spi_tx_init(void);
void dma2_stream2_spi_rx_init(void);
void dma2_stream3_spi_transfer(uint32_t msg_to_send, uint32_t msg_len);
void dma2_stream2_spi_receive(uint32_t received_msg, uint32_t msg_len);

#endif


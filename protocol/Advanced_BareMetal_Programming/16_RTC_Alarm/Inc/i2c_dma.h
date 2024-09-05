#ifndef __I2C_DMA_H__
#define __I2C_DMA_H__

#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"


void i2c1_init(void);
void dma1_stream6_i2c1_tx_init(void);
void dma1_stream5_i2c1_rx_init(void);
void dma1_stream6_i2c1_transfer(uint8_t *msg_to_send, uint32_t msg_len);
void dma1_stream5_i2c1_receive(uint8_t * received_msg, uint32_t msg_len);
void i2c_dma_write(uint8_t slave_addr, uint8_t *p_write_buff, uint16_t num_of_bytes);
void i2c_dma_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *p_read_buff, uint16_t num_of_bytes);

#endif

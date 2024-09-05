#ifndef __MPU9250_H__
#define __MPU9250_H__

#include <stdint.h>
#include "spi_dma.h"

#define ACC_FULL_SCALE_2_G       0x00
#define ACC_FULL_SCALE_4_G       0x08
#define ACC_FULL_SCALE_8_G       0x10
#define ACC_FULL_SCALE_16_G      0x18



#define MPU9250_ADDR_ACCELCONFIG  			  0x1C
#define MPU9250_ADDR_USER_CTRL				  0x6A
#define MPU9250_ACCEL_XOUT_H				  0x3B


void mpu9250_accel_config(uint8_t mode);
void mpu9250_accel_update(void);
float mpu9250_get_x(void);
float mpu9250_get_y(void);
float mpu9250_get_z(void);
void mpu9250_ncs_pin_config(void);
void mpu9250_ncs_pin_set(void);
void mpu9250_ncs_pin_reset(void);


#endif

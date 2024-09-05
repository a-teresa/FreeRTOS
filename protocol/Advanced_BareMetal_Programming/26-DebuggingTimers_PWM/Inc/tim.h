#ifndef __TIM_H__
#define __TIM_H__

#include <stdint.h>
#include "stm32f4xx.h"


void pseudo_dly(void);
void tim2_pa0_pa1_pwm_set_dutycycle(uint8_t ch, uint32_t dutycycle);
void tim2_pa0_pa1_pwm_init(void);

#endif

#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>
#include "stm32f4xx.h"


void led_init(void);
void led_on(void);
void led_off(void);
void led_blink(void);

#endif

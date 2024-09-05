#include "systick.h"

#define SYSTICK_LOAD_VAL			3200

/* if 1 sec =  16 000 0000
 *  then 1/5000 sec = ?
 *   ((1/5000)/1)* 16 000 0000 =  3200
 * */
#define	CTRL_ENABLE					(1U<<0)
#define CTRL_TICKINT				(1U<<1)
#define CTRL_CLKSRC					(1U<<2)


void systick_5kHz_interrupt(void)
{
	/*Reload with number of clocks*/
	SysTick->LOAD =  SYSTICK_LOAD_VAL -1;

	/*Clear Systick current value register*/
	SysTick->VAL = 0;

	/*Select Systick interrupt, internal clock and enable Systick*/
	SysTick->CTRL = CTRL_TICKINT |CTRL_CLKSRC |CTRL_ENABLE;

}

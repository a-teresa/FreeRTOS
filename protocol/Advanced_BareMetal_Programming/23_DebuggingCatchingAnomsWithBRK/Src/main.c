#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"



uint32_t final_value;

int main(void)
{
	final_value = 0;

	for( int i = 0; i < 10; i++)
	{
		final_value += i;

		if( 10 == final_value)
		{
			/*Hit a breakpoint*/
			__asm("BKPT #1\n");
		}
	}


	while(1)
	{

	}
}

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>



char _a[1]= {'A'};
char *pt_a = _a;

char hello[]= {"Hello from main"};
char *p_hello = hello;


int __io_putchar(int ch)
{
	ITM_SendChar(ch);

	return ch;
}

int main(void)
{



	while(1)
	{
		   printf("Printf is now available \n");

	}
}

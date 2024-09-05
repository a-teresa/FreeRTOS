#include "Helpers/logger.h"
#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>
#include "uart.h";


//char _a[1]={'A'};
//char *pt_a = _a;
char hello[]={"Hello from main"};
char *p_hello = hello;

int main(void){


	log_info("Program_start");

	uart2_tx_init();

	printf("Hello from main function \n\r");

//	for (int i=0; i<strlen(hello);i++){
//		ITM_SendChar(*p_hello++);
//	}


	while(1)
	{

	}
}

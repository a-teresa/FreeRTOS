#include "main.h"
#include "led.h"


Led_Type led1;
Led_Type led2;

int main(void){

	USART2_init();
	Led_ctor(&led1, RED, OFF);
	Led_ctor(&led1, BLUE, ON);
	Led_getState(&led1);
	Led_getState(&led2);
    Led_setState(&led1, ON);
	while(1){

	}
}

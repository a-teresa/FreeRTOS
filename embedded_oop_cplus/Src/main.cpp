#include "main.h"
#include "led.h"

Led led1(RED, ON);

LedState_Type LED1_state;
LedState_Type LED2_state;
LedState_Type LED3_state;

int main(void){

	USART2_init();

	Led led2(BLUE, ON);

	Led *led3 = new Led(YELLOW, ON);

	LED1_state = led1.getState();
	LED2_state = led2.getState();
	LED3_state = led3->getState();

	led1.setState(OFF);
	led2.setState(OFF);
	led3->setState(OFF);

	delete led3;

	while(1){

	}


}

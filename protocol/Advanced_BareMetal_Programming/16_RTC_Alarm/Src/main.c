#include "rtc.h"
#include "uart.h"
#include <stdio.h>

#define BUFF_LEN		20

uint8_t time_buff[BUFF_LEN] = {0};
uint8_t date_buff[BUFF_LEN] = {0};

static void display_rtc_calendar(void);
uint8_t g_alarm;
int main(void)
{

	uart2_tx_init();

	rtc_init();
	rtc_alarm_init();
	g_alarm = 0;
	while(1)
	{

	}

}

static void display_rtc_calendar(void)
{
	/*Display format :  hh : mm : ss*/
	sprintf((char *)time_buff,"%.2d :%.2d :%.2d",rtc_convert_bcd2bin(rtc_time_get_hour()),
			rtc_convert_bcd2bin(rtc_time_get_minute()),
			rtc_convert_bcd2bin(rtc_time_get_second()));

	printf("Time : %.2d :%.2d :%.2d\n\r",rtc_convert_bcd2bin(rtc_time_get_hour()),
			rtc_convert_bcd2bin(rtc_time_get_minute()),
			rtc_convert_bcd2bin(rtc_time_get_second()));


	/*Display format :  mm : dd : yy*/
	sprintf((char *)date_buff,"%.2d - %.2d - %.2d",rtc_convert_bcd2bin(rtc_date_get_month()),
			rtc_convert_bcd2bin(rtc_date_get_day()),
			rtc_convert_bcd2bin(rtc_date_get_year()));
}


static void alarm_callback(void)
{
	//Do somthing...
	g_alarm =  1;

	printf("ALARM RINGING !!|\n\r");
}


void RTC_Alarm_IRQHandler(void)
{
	/*Get alarm interrupt source enable status*/
	if((RTC->CR & CR_ALRAIE) != 0)
	{
		/*Get the pending status of the Alarm interrupt*/
		if((RTC->ISR & ISR_ALRAF) != 0)
		{
			alarm_callback();

			/*Clear the Alarm interrupt pending bit*/
			RTC->ISR &=~ISR_INIT;
		}


	}
	/*Clear the EXTI's flag for RTC Alarm*/
	EXTI->PR |=(1U<<17);
}

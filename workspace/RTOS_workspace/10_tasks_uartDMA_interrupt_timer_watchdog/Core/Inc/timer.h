/* Timer.h */
/* Pedro Santos */
#ifndef TIMER_H
#define TIMER_H

#include "stm32F4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


#define			PR_10ms			10  		// [ms] Time to Calcule time periode
#define			PR_100ms		100 		// [ms] Time to Calcule time periode
#define			PR_1us			1000		// [ms*1000] = us -> to use on timer 6 "core timer" TImeTick =48000000/ (48 prescaler) = 1us


typedef struct {

		uint16_t count[3]; // [0]->100ms [1]->500ms [2]->1000ms;

		bool f_10ms[8];
    bool f_100ms[13];
    bool f_500ms[6];
    bool f_1000ms[6];


		uint16_t usb_ignore_time;
		uint16_t adc_state_machine;
		uint32_t radio_rfm69_timer_10ms_inc;

		uint16_t sm_read_rtd_thc1_10ms;
		uint16_t sm_read_thc2_10ms;
		uint16_t sm_read_thc3_10ms;


		uint16_t sm_read_rtd_thc1_fault_10ms;
		uint16_t sm_read_thc2_fault_10ms;
		uint16_t sm_read_thc3_fault_10ms;

		uint16_t timer_uart_login_10ms;

		uint16_t timer_send_and_request_info_from_esp;

		uint16_t timer_write_report;

		uint16_t timer_to_change_step;
		/***************************************************/
		// Isolated State Machine
		uint16_t sm_isolated_mode_10ms;

		uint16_t minutes_counter;

		uint16_t sm_alarm_proc_10ms;

}mytime;

extern mytime timer;


/*****************************************************************************/
/*****************************************************************************/
void init_struct_timers(void);
/*****************************************************************************/
/*****************************************************************************/
void timer_flux_disable_interrupt(void);
/*****************************************************************************/
/*****************************************************************************/
void timer_flux_enable_interrupt(void);


#endif

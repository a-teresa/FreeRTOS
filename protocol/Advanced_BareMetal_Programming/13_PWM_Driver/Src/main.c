#include "tim.h"



int main(void)
{
	/*Init pwm at pa0 and pa1*/
	tim2_pa0_pa1_pwm_init();

	/*test 1*/
	//tim2_pa0_pa1_pwm_set_dutycycle(1,100);
	//tim2_pa0_pa1_pwm_set_dutycycle(2,1);

	while(1)
	{
		/*test 2*/

		for( int i = 0; i < 100; i++)
		{
			tim2_pa0_pa1_pwm_set_dutycycle(1,i);
			tim2_pa0_pa1_pwm_set_dutycycle(2,100 - i);
			pseudo_dly();
		}

		for( int i = 0; i < 100; i++)
		{
			tim2_pa0_pa1_pwm_set_dutycycle(1,100 - i);
			tim2_pa0_pa1_pwm_set_dutycycle(2,i);
			pseudo_dly();

		}
	}

}

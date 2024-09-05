#include <stdint.h>
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "stm32f410rx.h"
#include "Helpers/logger.h"

LogLevel system_log_level = LOG_LEVEL_DEBUG;
uint32_t SystemCoreClock = 72000000; //72MHz -> first defined here; only declared in header file; because we are not changing it in run time,
												//it's ok to mannually use iy, otherwise use SysemClockUpdate

//HCLK=72 MHz
/*adapt latency before changing the clock speed- > to not have any problem
 * reading the instructions after increasing the clock frequency -- see "read
 * interface" - reference manual*/

//PLL: M=4, N=72, P=2, Q=3
//AHB prescaller=1
//APB prescaller1 = 2, APB prescaller2 ? 1
//MC01 prescaller = 2


void configure_clock()
{
	MODIFY_REG(FLASH->ACR,
			FLASH_ACR_LATENCY,
			_VAL2FLD(FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2WS)//FLASH_ACR_LATENCY_2WS << FLASH_ACR_LATENCY_Pos
			);
	//Enable HSE
	SET_BIT(RCC->CR, RCC_CR_HSEON);

	//wait until HSE is stable
	while(!READ_BIT(RCC->CR, RCC_CR_HSERDY));


	//configure PLL: source hse; SYSCLK=72 MHZ -> config only before enable PLL

	MODIFY_REG(RCC->PLLCFGR,
			RCC_PLLCFGR_PLLM|RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLQ |RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLSRC,
			_VAL2FLD(RCC_PLLCFGR_PLLM,4)|_VAL2FLD(RCC_PLLCFGR_PLLN, 72)|_VAL2FLD(RCC_PLLCFGR_PLLQ,3)| _VAL2FLD(RCC_PLLCFGR_PLLSRC, 1));

	//enable PLL
		SET_BIT(RCC->CR, RCC_CR_PLLON);

	//wait until PLL is stable
	while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY));

	//Switch system clock to PLL
	MODIFY_REG(RCC->CFGR,
			RCC_CFGR_SW,
			_VAL2FLD(RCC_CFGR_SW, RCC_CFGR_SW_PLL));

	//Configure PPRE1
	MODIFY_REG(RCC->CFGR,
				RCC_CFGR_PPRE1,
				_VAL2FLD(RCC_CFGR_PPRE1, 4));

	//wait until PLL is used
	while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS)  != RCC_CFGR_SWS_PLL);

	//Disable HSI
	CLEAR_BIT(RCC->CR, RCC_CR_HSION);
}
void SystemInit(void)
{
	configure_clock();
}
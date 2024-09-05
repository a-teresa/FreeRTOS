#include "wwdg.h"


void wwdg_init(void)
{
	/*Enable the peripheral clock WWDG*/
	RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

	/*Set prescaler to have a rollover each about ~2s*/
	WWDG->CFR |=(1U<<7);
	WWDG->CFR |=(1U<<8);

	/*Set window value to same value (~2s) as downcounter
	in order to be able to refresh the WWDG almost immediately*/

	WWDG->CFR &=~WWDG_CFR_W;
	WWDG->CFR |= 0x7E;

	/*Set counter*/
	WWDG->CR &=~WWDG_CR_T;
	WWDG->CR |= 0x7E;

	/*Enable WWDG*/
	WWDG->CR |= WWDG_CR_WDGA;

}

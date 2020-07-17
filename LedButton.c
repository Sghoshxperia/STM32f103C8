#include "stm32f10x.h"


int main()
{
	//Setting up Led C13
	RCC ->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC ->CRH |= GPIO_CRH_MODE13;
	GPIOC ->CRH &= ~(GPIO_CRH_CNF13);
	
	//Setting up pushbutton B5
	RCC ->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB ->CRL &= ~(GPIO_CRL_MODE5);
	GPIOB ->CRL |= GPIO_CRL_CNF5_1;
	GPIOB ->CRL &= ~(GPIO_CRL_CNF5_0);
	
	
	while(1)
	{
		if(GPIOB ->IDR & GPIO_IDR_IDR5)
		{
     	GPIOC ->BSRR |= GPIO_BSRR_BS13;
		}
		else
		{
			GPIOC ->BRR |= GPIO_BRR_BR13;
		}
		
		
	}
	
	
}


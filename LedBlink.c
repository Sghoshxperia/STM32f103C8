#include "stm32f10x.h"

int main()
{
	//Setting the internal clock for PORT B
 	 RCC ->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
  	//Setting pin B12 as general purpose push/pull mode
	GPIOB ->CRH &= ~(GPIO_CRH_CNF12);
  	//Setting pin B12 as general output mode
	GPIOB ->CRH |= GPIO_CRH_MODE12_0;
	
	
while(1)
{
	
  //Setting pin B12 (LED ON)
  	GPIOB ->BSRR |= GPIO_BSRR_BS12;
	for(int i = 0; i < 200000; i++);
  
  //Resetting pin B12 (LED OFF)
	GPIOB ->BRR |= GPIO_BRR_BR12;
	for(int i = 0; i < 200000; i++);
	
}

}



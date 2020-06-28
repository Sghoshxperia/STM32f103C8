#include "stm32f10x.h"

int main()
{
	//Pwm in channel 4 of timer4 pinb9
	
	
	//Setting up GPIO pin B9
	RCC ->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	GPIOB ->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;
	GPIOB ->CRH &= ~(GPIO_CRH_CNF9_0);
	
	//Setting up PWM
	RCC ->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TIM4 ->CCER |= TIM_CCER_CC4E;
	TIM4 ->CR1 |= TIM_CR1_ARPE;
        TIM4 ->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;	
	
	TIM4 ->PSC = 8;
	TIM4 ->ARR = 1000;
	TIM4 ->CCR4 = 250;
	
	TIM4 ->EGR = TIM_EGR_UG;
	TIM4 ->CR1 = TIM_CR1_CEN;
	
	while(1)
	{
		
		for(uint16_t i=0;i<1000;i++)
		TIM4 ->CCR4 = i;
		
	}
	
}

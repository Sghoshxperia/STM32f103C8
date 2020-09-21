#include "stm32f10x.h"

volatile static float mapfdone = 0.0;
volatile static float value = 0.0;

float map(float z,float input_l,float input_h,float output_l,float output_h);
uint16_t control(float angle);




float map(float z,float input_l,float input_h,float output_l,float output_h)
{
	float mapf = (z - input_l)/(input_h-input_l)*(output_h-output_l)+output_l;
	mapfdone = mapf;
	
	
	return(mapfdone);
}

uint16_t control(float angle)
{
	//takes an input angle and maps in between 1ms and 2ms for pwm conversion
	
	value = map(angle, 0.0, 180.0, 1.0, 2.0);
	value = (value / 20) * TIM1 ->ARR;

  return (uint16_t)value;
}

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
	
	//Clock freq = 8MHz  
	TIM4 ->PSC = 8;                //Pre-scalar = 8 
	TIM4 ->ARR = 2000;						// Counter = 2000 makes f = 50Hz or T = 20ms
	TIM4 ->CCR4 = 0;
	
	TIM4 ->EGR = TIM_EGR_UG;
	TIM4 ->CR1 = TIM_CR1_CEN;
	
	while(1)
	{
		
		TIM4 ->CCR4 = control(90);
		
	}
	
	
	
}

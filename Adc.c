#include "stm32f10x.h"

volatile static uint16_t myTicks = 0;
volatile static uint32_t val = 0;
volatile static uint32_t mapfdone = 0;


uint16_t map(uint32_t z,float input_l,float input_h,float output_l,float output_h)
{
	float mapf = (z - input_l)/(input_h-input_l)*(output_h-output_l)+output_l;
	mapfdone = (uint16_t)mapf;
	
	
	return(mapfdone);
}


void SysTick_Handler(void)
{
	myTicks++;
}

void delayMs(uint16_t ms)
{
	myTicks = 0;
	while(myTicks<ms);
}

int main()
{
	
	//Setting clock for ADC
	RCC ->CFGR |= RCC_CFGR_ADCPRE_DIV6;
	RCC ->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;

  //Setting the ADC pin A5
	GPIOA ->CRL |= GPIO_CRL_CNF5_1;
	GPIOA ->CRL &= ~(GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	
	//Enable End of Conversion interrupt
	ADC1 ->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	//Setting sampling rate
	ADC1 ->SMPR2 |= ADC_SMPR2_SMP5_0 | ADC_SMPR2_SMP5_1 | ADC_SMPR2_SMP5_2;
	
	//Set the channel
	ADC1 ->SQR3 |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2;
	
	//Switch on ADC and put it on continous mode
	ADC1 ->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
	
	//Setting up delay
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	
	delayMs(1);
	
	//Turning on ADC for the second time
	ADC1 ->CR2 |= ADC_CR2_ADON;
	delayMs(1);
	
	//Calibration
	ADC1 ->CR2 |= ADC_CR2_CAL;
	//delayMs(2);
	while(ADC1 ->CR2 & ADC_CR2_CAL);
	
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
	TIM4 ->CCR4 = 0;
	
	TIM4 ->EGR = TIM_EGR_UG;
	TIM4 ->CR1 = TIM_CR1_CEN;
	
	while(1)
	{
		TIM4 ->CCR4 = map(val, 0, 4095, 0, 1000);
		delayMs(5);
		
	}
		
	
}

void ADC1_2_IRQHandler(void)
{
	if(ADC1 ->SR & ADC_SR_EOC)
		 val = ADC1 ->DR;
}


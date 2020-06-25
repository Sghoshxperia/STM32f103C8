#include "stm32f10x.h"

volatile static uint16_t myTicks=0;

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
	RCC ->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB ->CRH &= ~(GPIO_CRH_CNF12);
	GPIOB ->CRH |= GPIO_CRH_MODE12_0;
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	
	
while(1)
{
	GPIOB ->BSRR |= GPIO_BSRR_BS12;
	delayMs(1000);
	GPIOB ->BRR |= GPIO_BRR_BR12;
	delayMs(1000);
	
}

}

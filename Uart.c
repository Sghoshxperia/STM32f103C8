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

void Blink(int n)
{
  while(n!=0)
	{
		GPIOC ->BSRR |= GPIO_BSRR_BS13;
		delayMs(500);
		GPIOC ->BRR |= GPIO_BRR_BR13;
		delayMs(500);
		n--;
	}

}	

void USART_Receive()
{
  while((USART1 ->SR & USART_SR_RXNE) == 0);
	unsigned char val = USART1 ->DR;
	if(val == 'B')
		Blink(2);
}

void USART_Transmit(unsigned char value)
{
  USART1 ->DR = value;
	while((USART1 ->SR & USART_SR_TXE) == 0);
}

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
	
	//Setting up UART on TX - A9 RX - A10
	RCC ->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	//RX - A10 set as floating input
	GPIOA ->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10_1);
	GPIOA ->CRH |= GPIO_CRH_CNF10_0;
	//TX - A9 set as alternate output push/pull
	GPIOA ->CRH &= ~GPIO_CRH_CNF9_0;
	GPIOA ->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1;
	
	//BaudRate
	USART1 ->BRR |= 0x2580;                                   //Set baudrate 9600
	USART1 ->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; 
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	
	while(1)
	{
		if(GPIOB ->IDR & GPIO_IDR_IDR5)
		{
      USART_Transmit('B'); 
			//Blink(2);	
		}
		else
		{
			USART_Transmit('A');
			//GPIOC ->BRR |= GPIO_BRR_BR13;
		}
		delayMs(100);
		USART_Receive();
		
	}
	
}

/*
 * f4xxx.cpp
 *
 *  Created on: 31 окт. 2020 г.
 *      Author: rusla
 */

#include "f4xxx.h"
#include "stm32f4xx_hal.h"

F4xxx::F4xxx()
{
	// TODO Auto-generated constructor stub

}

void F4xxx::enableAHB1PortA()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

void F4xxx::setPinMode(PortMode mode, unsigned int pin)
{
	uint32_t moder = m_gpioModerPositions[pin]<<1;
	switch (mode)
	{
	case PortMode::Input:
		break;
	case PortMode::Output:
		GPIOA->MODER |= (0x1UL << moder);
		break;
	case PortMode::Alternative:
		GPIOA->MODER |= (0x2UL << moder);
		break;
	case PortMode::Analog:
		GPIOA->MODER |= (0x3UL << moder);
		break;
	}
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0;
}

void F4xxx::setPin5High()
{
	GPIOA->ODR |= GPIO_ODR_OD5;
}

//Настраиваем тактирование системы от внешнего кварца
//через PLL на саксимально возможных частотах.
//Внешний кварц должен быть на 8МГц
//Возвращает:
//  0 - завершено успешно
//  1 - не запустился кварцевый генератор
//  2 - не запустился PLL

#define PLL_M      8
#define PLL_N      336

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      4

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

int F4xxx::clockInit(void)
{
	enableHse();
	//FLASH
	FLASH->ACR |= FLASH_ACR_PRFTEN;
	FLASH->ACR&= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

	//set HSE as PLL source
	RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE;

	RCC->CR &= ~(RCC_CR_PLLON); //disable PLL before changes

	//PLL M
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM); 		//clear all PLLM bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_2; 		//set PLLM = 4 (100)

	//PLL N
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN); 		//clear all PLLN bits with mask
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_3; 		//set PLLN = 168 (1010 1000)
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_5; 		//for 8 or 16 MHz HSE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_7; 		//

	//PLL P
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP); 		//main PLL division PLLP = 2: 00
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_1;

	//PLL Q 7 (0111)
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLQ);
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLQ_0
			| RCC_PLLCFGR_PLLQ_2
			| RCC_PLLCFGR_PLLQ_2);

	RCC->PLLCFGR |= PLL_M|(PLL_Q<<24)|(PLL_P<<16)|(PLL_N<<6)|RCC_PLLCFGR_PLLSRC_HSE;
	RCC->CR|=RCC_CR_PLLON;
	while(!(RCC->CR&RCC_CR_PLLRDY));

	RCC->CFGR &= ~(RCC_CFGR_HPRE); 	//Prescaler 1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //AHB = SYSCLK/1


	//APB2 Prescaler 2
	RCC->CFGR &= ~(RCC_CFGR_PPRE2);
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;  //APB2 /2

	RCC->CFGR &= ~(RCC_CFGR_PPRE1);
	RCC->CFGR|=RCC_CFGR_PPRE1_DIV4; // APB1 /4

	RCC->CFGR &= ~RCC_CFGR_SW; // reset SW0, SW1.
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	RCC->CR|=RCC_CR_PLLON;
	while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL); // wait for switchin to PLL (while PLL is not used as system clock)

	// for power saving
	RCC->CR &= ~(RCC_CR_HSION);

	return 0;
}

void F4xxx::enableHse()
{
	RCC->CR |= (RCC_CR_HSEON); //Enable HSE
	while( !(RCC->CR & RCC_CR_HSERDY) ) {}; //ready to start HSE
}

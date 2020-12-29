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

void F4xxx::enableAHB1(RCCEnableAhb1PeripheralClockFor hardware)
{
	//auto d = RCC_AHB1ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	RCC->AHB1ENR |= (0x1UL << (unsigned)hardware);
}

void F4xxx::setPinMode(Port port, PortMode mode, unsigned int pin)
{
	uint32_t moder = m_gpioModerPositions[pin];
	auto* gpioPort = ((GPIO_TypeDef *) ((unsigned)port));
	switch (mode)
	{
	case PortMode::Input:
		break;
	case PortMode::Output:
		gpioPort->MODER |= (0x1UL << moder);
		break;
	case PortMode::Alternative:
		gpioPort->MODER |= (0x2UL << moder);
		break;
	case PortMode::Analog:
		gpioPort->MODER |= (0x3UL << moder);
		break;
	}
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5; // max speed 3ul (50 mhz)
}

void F4xxx::setPinHigh(Port port, unsigned pin)
{
	auto* gpioPort = ((GPIO_TypeDef *) (unsigned)port);
	gpioPort->ODR |= (0x1UL << pin);
}

void F4xxx::setPinLow(Port port, unsigned pin)
{
	auto* gpioPort = ((GPIO_TypeDef *) (unsigned)port);
	gpioPort->ODR &= ~(0x1UL << pin);
}

//uint32_t PLLSource; /*!< RCC_PLLSource: PLL entry clock source. This parameter must be a value of @ref RCC_PLL_Clock_Source */
//uint32_t PLLM; /*!< PLLM: Division factor for PLL VCO input clock. This parameter must be a number between Min_Data = 0 and Max_Data = 63 */
//uint32_t PLLN; /*!< PLLN: Multiplication factor for PLL VCO output clock. This parameter must be a number between Min_Data = 192 and Max_Data = 432 */
//uint32_t PLLP; /*!< PLLP: Division factor for main system clock (SYSCLK). This parameter must be a value of @ref RCC_PLLP_Clock_Divider */
//uint32_t PLLQ; /*!< PLLQ: Division factor for OTG FS, SDIO and RNG clocks. This parameter must be a number between Min_Data = 4 and Max_Data = 15 */
int F4xxx::clockInit(int pllM, int pllN, int pllP, int pllQ)
{
	enableHse();

	//FLASH
	CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
	FLASH->ACR&= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN|FLASH_ACR_PRFTEN;

	RCC->CR &= ~(RCC_CR_PLLON); //disable PLL before changes
	RCC->CFGR |=RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE2_DIV2|RCC_CFGR_PPRE1_DIV4; // AHB, APB1, APB2

	// set m, n, p, q and HSE as PLL source
	//RCC->PLLCFGR = pllM|(pllN<<6)|(((pllP>>1)-1)<<16)|(pllQ<<24)|RCC_PLLCFGR_PLLSRC_HSE;
	//RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE | (RCC_PLLCFGR_PLLM & 4) | (RCC_PLLCFGR_PLLN & 168) | (RCC_PLLCFGR_PLLP & 2));
	RCC->PLLCFGR=(pllQ<<24)|(pllP<<16)|(pllN<<6)|pllM|RCC_PLLCFGR_PLLSRC_HSE;
	RCC->CR|=RCC_CR_PLLON;
	while(!(RCC->CR&RCC_CR_PLLRDY));

	RCC->CFGR &= ~RCC_CFGR_SW; // reset SW0, SW1.
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // wait for switching to PLL (while PLL is not used as system clock)

	// for power saving
	//RCC->CR &= ~(RCC_CR_HSION);

	return 0;
}

void F4xxx::enableHsi()
{
	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY));

	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while((RCC->CFGR & RCC_CFGR_SWS_HSI) != RCC_CFGR_SWS_HSI);
}

void F4xxx::enableHse()
{

	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER&=~GPIO_MODER_MODE9;
	GPIOC->MODER|=GPIO_MODER_MODE9_1;
	GPIOC->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR9;

	// for control MCO2 (PC9): (freq=SYSCLK/5)
	RCC->CFGR|=RCC_CFGR_MCO2PRE;

	RCC->CR |= (RCC_CR_HSEON); //Enable HSE
	while(!(RCC->CR & RCC_CR_HSERDY) ) {}; //ready to start HSE
}

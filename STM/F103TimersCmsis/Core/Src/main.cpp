/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f1xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int clockInit(void)

{
	// RCC - reset and clock control, CR - Clock control register
	SET_BIT(RCC->CR, RCC_CR_HSEON); //Enable HSE clock

	while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET); //wait until HSE ready

	//Configuring of PLL (HSE crystal frequency is 8MHz)
	// SYSCLK = 72 МГц, USB = 48 МГц, PCLK1 = 36 МГц, PCLK2 = 72 МГц,  ADC = 12 МГц
	RCC->CFGR |= RCC_CFGR_PLLMULL9 //Bits 21:18, 0111: PLL input clock x 9
			| RCC_CFGR_PLLSRC; //Enable PLL as a source of HSE

	RCC->CR |= RCC_CR_PLLON; //Run PLL

	while(!(RCC->CR & RCC_CR_PLLRDY));
	CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	// 2 cycles of Flash wait because 48 MHz < SYSCLK ≤ 72 MHz
	FLASH->ACR &= ~FLASH_ACR_LATENCY_2;
	FLASH->ACR |= FLASH_ACR_LATENCY_2;

	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 //APB2/1
			| RCC_CFGR_PPRE1_DIV2 //APB1/2
			| RCC_CFGR_HPRE_DIV1; //AHB/1

	RCC->CFGR |= RCC_CFGR_SW_PLL;

	while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_1);

	RCC->CR &= ~RCC_CR_HSION;

	return 0;
}

void initPortAClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL &= ~GPIO_CRL_MODE1;
}

void initPortBClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
}

void initPortCClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;
	GPIOC->CRH|= GPIO_CRH_MODE13_1;
}

void initSwdOnlyDebugging()
{
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // JTAG is disabled
}

void initAltFunctionsClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
}

extern "C" void TIM2_IRQHandler (void)
{
	if (TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		GPIOC->ODR^=GPIO_ODR_ODR13;
	}

}

void timer2Init(void)
{
	// (PSC+1)(ARR+1) = TIMER_CLK/EVENT_CLK
	// EVENT_CLK = 72000/(3600*1000)

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 1000-1;

	// ARR - auto-reload value
	TIM2->ARR = 36000-1;

	// Re-initializes the timer counter and generates an update of the registers
	TIM2->EGR |= TIM_EGR_UG;


	NVIC_EnableIRQ (TIM2_IRQn);
}

void timer2Start (void)
{
	//Clear TIM2_IRQn update interrupt
	TIM2->SR &= ~TIM_SR_UIF;

	// Enable interrupt on update
	TIM2->DIER |= TIM_DIER_UIE;

	// Enable timer
	TIM2->CR1 |= TIM_CR1_CEN;
	while (!(TIM2->SR & (1<<0)));
	NVIC_EnableIRQ(TIM2_IRQn);
}

void initPwmTimer2()
{
	initPortAClock();
	initAltFunctionsClock();

	//ch1 -PA0
	GPIOA->CRL |= GPIO_CRL_MODE0;  //50Mhz
	GPIOA->CRL &= ~GPIO_CRL_CNF0; //clear CNF[1:0] for PA0
	GPIOA->CRL |= GPIO_CRL_CNF0_1; //output Push-Pull in alternative function mode
	//ch2 - PA1
	GPIOA->CRL |= GPIO_CRL_MODE1;  //50Mhz
	GPIOA->CRL &= ~GPIO_CRL_CNF1; //clear CNF[1:0] for PA1
	GPIOA->CRL |= GPIO_CRL_CNF1_1; //output Push-Pull in alternative function mode
	//ch3 - PA2
	GPIOA->CRL |= GPIO_CRL_MODE2;  //50Mhz
	GPIOA->CRL &= ~GPIO_CRL_CNF2; //clear CNF[1:0] for PA2
	GPIOA->CRL |= GPIO_CRL_CNF2_1; //output Push-Pull in alternative function mode
	//ch4 - PA3
	GPIOA->CRL |= GPIO_CRL_MODE3;  //50Mhz
	GPIOA->CRL &= ~GPIO_CRL_CNF3; //clear CNF[1:0] for PA3
	GPIOA->CRL |= GPIO_CRL_CNF3_1; //output Push-Pull in alternative function mode

	//TIM2 Settings
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 |= TIM_CR1_ARPE;  //autorelode mode

	//Output Compare Preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;

	TIM2->PSC = 2823 - 1;
	TIM2->ARR = 255-1;  // 72000000/(2823*255) = 100Hz
	TIM2->CCR1 = 20; //ch1 1duty cycle = 50%
	TIM2->CCR2 = 0; //ch2 1duty cycle = 50%
	TIM2->CCR3 = 20; //ch2 1duty cycle = 50%
	TIM2->CCR4 = 1000; //ch2 1duty cycle = 50%

	//TIM2->CCER |= TIM_CCER_CC2P;  //polarity of output signal

	//Capture/Compare 1,2,3,4 output enable
	TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
	//Output Compare Mode - 110 - PWM mode 1(Edge-Aligned Mode)
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
	TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
	TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
	TIM2->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);

	TIM2->EGR   = TIM_EGR_UG;
	//start counting
	TIM2->CR1 |= TIM_CR1_CEN;
	while(!(TIM2->SR & TIM_SR_UIF));
}

void initPwmTimer1()
{
	initPortAClock();
	initAltFunctionsClock();

	//ch1 -PA8
	GPIOA->CRH |= GPIO_CRH_MODE8;  //50Mhz
	GPIOA->CRH &= ~GPIO_CRH_CNF8; //clear CNF[1:0] for PA8
	GPIOA->CRH |= GPIO_CRH_CNF8_1; //output Push-Pull in alternative function mode

	//ch1 -PA9
	GPIOA->CRH |= GPIO_CRH_MODE9;  //50Mhz
	GPIOA->CRH &= ~GPIO_CRH_CNF9; //clear CNF[1:0] for PA9
	GPIOA->CRH |= GPIO_CRH_CNF9_1; //output Push-Pull in alternative function mode

	//ch1 -PA10
	GPIOA->CRH |= GPIO_CRH_MODE10;  //50Mhz
	GPIOA->CRH &= ~GPIO_CRH_CNF10; //clear CNF[1:0] for PA10
	GPIOA->CRH |= GPIO_CRH_CNF10_1; //output Push-Pull in alternative function mode

	//ch1 -PA11
	GPIOA->CRH |= GPIO_CRH_MODE11;  //50Mhz
	GPIOA->CRH &= ~GPIO_CRH_CNF11; //clear CNF[1:0] for PA11
	GPIOA->CRH |= GPIO_CRH_CNF11_1; //output Push-Pull in alternative function mode

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->CR1 = TIM_CR1_ARPE;
	TIM1->PSC = 2823 - 1;
	TIM1->ARR = 255-1;  // 72000000/(2823*255) = 100Hz
	//TIM1->RCR = 0;      //TIM_RepetitionCounter;

	// OC1
	TIM1->CCR1  = 125;    // 0.5мкс.
	TIM1->CCMR1 = (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
	TIM1->CCER  = TIM_CCER_CC1E;

	// OC2
	TIM1->CCR2  = 125;     // 1.0us
	TIM1->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
	TIM1->CCER  |= TIM_CCER_CC2E;

	// OC 3
	TIM1->CCR3  = 125;     // 1.5us.
	TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
	TIM1->CCER  |= TIM_CCER_CC3E;
	// OC4
	TIM1->CCR4  = 24;     // 1us
	TIM1->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
	TIM1->CCER  |= TIM_CCER_CC4E;

	TIM1->EGR   = TIM_EGR_UG; // генерируем update event для загрузки всех теневых регистров
	TIM1->BDTR |= TIM_BDTR_MOE + 12; // разрешение выходных сигналов OCx + dead-time 0.5us
	TIM1->CR1 |= TIM_CR1_CEN;
	while(!(TIM1->SR & TIM_SR_UIF));
}
void timer1_init(void)
{
	RCC-> APB2ENR |=RCC_APB2ENR_TIM1EN;
	// NVIC_EnableIRQ (TIM2_IRQn);
	//TIM2->CR1&=~  TIM_CR1_CEN;
	TIM1->PSC = 2400-1; // Настраиваем делитель что таймер тикал 1000 раз в секунду
	TIM1->ARR = 20 ;
	TIM1->RCR=10-1;

	TIM1->DIER |= TIM_DIER_UIE;

	TIM1->EGR = TIM_EGR_UG;

	TIM1->CR1|=   TIM_CR1_CEN;
	NVIC_EnableIRQ (TIM1_UP_IRQn);
}
int main(void)
{
	//__enable_irq();
	clockInit();
	//initPortBClock();
	//initPortCClock();
	initSwdOnlyDebugging();
	initPwmTimer1();
	//timer1_init();
	//timer2Init();
	//timer2Start();
	while(1){}
}

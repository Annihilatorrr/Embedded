/*
 * f4xxx.h
 *
 *  Created on: 31 окт. 2020 г.
 *      Author: rusla
 */

#ifndef SRC_STM32WRAPPER_F4XXX_H_
#define SRC_STM32WRAPPER_F4XXX_H_

#include <inttypes.h>
#include "stm32f4xx_hal.h"

class F4xxx {
	void enableHse();

	uint32_t m_gpioModerPositions[16]
								  {
		GPIO_MODER_MODER0_Pos,
		GPIO_MODER_MODER1_Pos,
		GPIO_MODER_MODER2_Pos,
		GPIO_MODER_MODER3_Pos,
		GPIO_MODER_MODER4_Pos,
		GPIO_MODER_MODER5_Pos,
		GPIO_MODER_MODER6_Pos,
		GPIO_MODER_MODER7_Pos,
		GPIO_MODER_MODER8_Pos,
		GPIO_MODER_MODER9_Pos,
		GPIO_MODER_MODER10_Pos,
		GPIO_MODER_MODER11_Pos,
		GPIO_MODER_MODER12_Pos,
		GPIO_MODER_MODER13_Pos,
		GPIO_MODER_MODER14_Pos,
		GPIO_MODER_MODER15_Pos
								  };

	uint32_t m_gpioOdrPositions[16]
								{
		GPIO_ODR_OD0,
		GPIO_ODR_OD1,
		GPIO_ODR_OD2,
		GPIO_ODR_OD3,
		GPIO_ODR_OD4,
		GPIO_ODR_OD5,
		GPIO_ODR_OD6,
		GPIO_ODR_OD7,
		GPIO_ODR_OD8,
		GPIO_ODR_OD9,
		GPIO_ODR_OD10,
		GPIO_ODR_OD11,
		GPIO_ODR_OD12,
		GPIO_ODR_OD13,
		GPIO_ODR_OD14,
		GPIO_ODR_OD15
								};
public:

	enum class PortMode
	{
		Input = 0,
				Output,
				Alternative,
				Analog
	};

	enum class Port
	{
		A = AHB1PERIPH_BASE + 0x0000UL,
				B = AHB1PERIPH_BASE + 0x0400UL,
				C = AHB1PERIPH_BASE + 0x0800UL,
				D = AHB1PERIPH_BASE + 0x0C00UL,
				E = AHB1PERIPH_BASE + 0x1000UL,
				H = AHB1PERIPH_BASE + 0x1C00UL
	};

	enum class RCCEnableAhb1PeripheralClockFor
	{
		PortA = RCC_AHB1ENR_GPIOAEN_Pos,
				PortB = RCC_AHB1ENR_GPIOBEN_Pos,
				PortC = RCC_AHB1ENR_GPIOCEN_Pos,
				PortD = RCC_AHB1ENR_GPIODEN_Pos,
				PortE = RCC_AHB1ENR_GPIOEEN_Pos,
				PortH = RCC_AHB1ENR_GPIOHEN_Pos,
				_CRC = RCC_AHB1ENR_CRCEN_Pos,
				_DMA1 = RCC_AHB1ENR_DMA1EN_Pos,
				_DMA2 = RCC_AHB1ENR_DMA2EN_Pos
	};

	F4xxx();
	void enableAHB1(RCCEnableAhb1PeripheralClockFor hardware);
	void setPinMode(Port port, PortMode mode, unsigned int pin);
	void setPinHigh(Port port, unsigned pin);
	void setPinLow(Port port, unsigned pin);
	int clockInit(int pllM, int pllN, int pllP, int pllQ);
};

#endif /* SRC_STM32WRAPPER_F4XXX_H_ */

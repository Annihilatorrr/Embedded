/*
 * spiStm32f1.h
 *
 *  Created on: 2 mar. 2023
 *      Author: VertexNi
 */

#ifndef SPISTM32F401_H_
#define SPISTM32F401_H_

#include "controllerdef.h"

#include "stm32f4xx.h"


struct PortPinPair
{
	GPIO_TypeDef* port;
	uint8_t pin;
};

template <Controller> class Spi;

template <> class Spi<Controller::f401> {

public:
	enum class SpiNumber{Spi1, Spi2, Spi3, Spi4};
	enum class SpiFrameSize{Bit8, Bit16};
private:
	PortPinPair m_cs;
	PortPinPair m_clock;
	PortPinPair m_miso;
	PortPinPair m_mosi;
	SPI_TypeDef* m_spi;
	Spi<Controller::f401>::SpiFrameSize m_frameSize;

	//	void spi3Init(uint8_t spiIndex, GPIO_TypeDef* port, uint8_t csPin, uint8_t clockPin, uint8_t misoPin, uint8_t mosiPin, SpiFrameSize frameSize)
	//	{
	//		m_port = port;
	//		m_csPin = csPin;
	//		m_clockPin = clockPin;
	//		m_misoPin = misoPin;
	//		m_mosiPin = mosiPin;
	//
	//		switch(spiIndex)
	//		{
	//		case 1:
	//			m_spi = SPI1;
	//			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	//			break;
	//		case 2:
	//			m_spi = SPI2;
	//			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	//			break;
	//		case 3:
	//			m_spi = SPI3;
	//			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	//			break;
	//		case 4:
	//			m_spi = SPI4;
	//			RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
	//			break;
	//		}
	//
	//		//RCC->AHB1ENR |=RCC_AHB1ENR_GPIOCEN;
	//		RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	//
	//		GPIOC->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12
	//				| GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12);
	//
	//		GPIOC->MODER |= GPIO_MODER_MODER12_1; // AF output
	//		GPIOC->OTYPER &= ~GPIO_OTYPER_OT12;
	//		GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED12_0;
	//		GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR12;
	//
	//		GPIOC->MODER &= ~GPIO_MODER_MODER11; // input
	//		GPIOC->OTYPER &= ~GPIO_OTYPER_OT11;
	//		GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR11;
	//
	//		GPIOC->MODER |= GPIO_MODER_MODER10_1; // AF output
	//		GPIOC->OTYPER &= ~GPIO_OTYPER_OT10;
	//		GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR10;
	//
	//		GPIOC->MODER |= GPIO_MODER_MODER8_0; // general purpose output
	//		GPIOC->OTYPER &= ~GPIO_OTYPER_OT8;
	//		GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;
	//		GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8;
	//		GPIOC->BSRR = GPIO_BSRR_BS8;
	//
	//		GPIOC->AFR[1] |= GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2 |
	//				GPIO_AFRH_AFSEL11_1 | GPIO_AFRH_AFSEL11_2 |
	//				GPIO_AFRH_AFSEL12_1 | GPIO_AFRH_AFSEL12_2;
	//
	//		//		GPIOC->AFR[0] |= GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_2 |
	//		//				GPIO_AFRH_AFSEL11_0 | GPIO_AFRH_AFSEL11_2 |
	//		//				GPIO_AFRH_AFSEL12_0 | GPIO_AFRH_AFSEL12_2;
	//		SPI3->CR1 = 0 << SPI_CR1_DFF_Pos // 8bit frame
	//				| 0 << SPI_CR1_LSBFIRST_Pos      // MSB first
	//				| 1 << SPI_CR1_SSM_Pos           // Software SS
	//				| 1 << SPI_CR1_SSI_Pos           // SS high
	//				| 0x04 << SPI_CR1_BR_Pos         // Speed: F_PCLK/32
	//				| 1 << SPI_CR1_MSTR_Pos          // Master mode
	//				| 0 << SPI_CR1_CPOL_Pos
	//				| 0 << SPI_CR1_CPHA_Pos;         //SPI: 0
	//
	//		SPI3->CR1 |= SPI_CR1_SPE;
	//
	//		while(!(SPI3->SR & SPI_SR_TXE));      // make sure TX buffer is empty
	//		while(SPI3->SR & SPI_SR_BSY);         // make sure SPI isn't busy
	//	}

	//	void spi1Init(uint8_t spiIndex, GPIO_TypeDef* port, uint8_t csPin, uint8_t clockPin, uint8_t misoPin, uint8_t mosiPin, SpiFrameSize frameSize)
	//	{
	//		m_port = port;
	//		m_csPin = csPin;
	//		m_clockPin = clockPin;
	//		m_misoPin = misoPin;
	//		m_mosiPin = mosiPin;
	//
	//		switch(spiIndex)
	//		{
	//		case 1:
	//			m_spi = SPI1;
	//			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	//			break;
	//		case 2:
	//			m_spi = SPI2;
	//			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	//			break;
	//		case 3:
	//			m_spi = SPI3;
	//			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	//			break;
	//		case 4:
	//			m_spi = SPI4;
	//			RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
	//			break;
	//		}
	//
	//		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	//		RCC->APB2ENR |=RCC_APB2ENR_SPI1EN;
	//
	//		GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7
	//				|GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5|GPIO_OSPEEDER_OSPEEDR6|GPIO_OSPEEDER_OSPEEDR7);
	//
	//		// 7
	//		GPIOA->MODER |= GPIO_MODER_MODER7_1; // AF output
	//		GPIOA->OTYPER &= ~GPIO_OTYPER_OT7;
	//		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_0;
	//		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7;
	//
	//		GPIOA->MODER |= GPIO_MODER_MODER6_1; // input
	//		GPIOA->OTYPER &= ~GPIO_OTYPER_OT6;
	//		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6;
	//
	//		GPIOA->MODER |= GPIO_MODER_MODER5_1; // AF output
	//		GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
	//		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
	//
	//		GPIOA->MODER |= GPIO_MODER_MODER4_0; // general purpose output
	//		GPIOA->OTYPER &= ~GPIO_OTYPER_OT4;
	//		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4;
	//
	//
	//		GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL5_2 |
	//				GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_2 |
	//				GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_2;//(5 << 4*5) | (5 << 4*6) | (5 << 4*7);
	//		SPI1->CR1 = 0 << SPI_CR1_DFF_Pos // 8bit frame
	//				| 0 << SPI_CR1_LSBFIRST_Pos      // MSB first
	//				| 1 << SPI_CR1_SSM_Pos           // Software SS
	//				| 1 << SPI_CR1_SSI_Pos           // SS high
	//				| 0x04 << SPI_CR1_BR_Pos         // Speed: F_PCLK/32
	//				| 1 << SPI_CR1_MSTR_Pos          // Master mode
	//				| 0 << SPI_CR1_CPOL_Pos
	//				| 0 << SPI_CR1_CPHA_Pos;         //SPI: 0
	//
	//		SPI1->CR1 |= SPI_CR1_SPE;
	//
	//		while(!(SPI1->SR & SPI_SR_TXE));      // make sure TX buffer is empty
	//		while(SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy
	//	}

	void spiXInit(uint8_t spiIndex, PortPinPair cs, PortPinPair clock, PortPinPair miso, PortPinPair mosi, SpiFrameSize frameSize)
	{
		m_cs = cs;
		m_miso = miso;
		m_mosi = mosi;
		m_clock = clock;

		switch(spiIndex)
		{
		case 1:
			m_spi = SPI1;
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
			break;
		case 2:
			m_spi = SPI2;
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
			break;
		case 3:
			m_spi = SPI3;
			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
			break;
		case 4:
			m_spi = SPI4;
			RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
			break;
		}

		cs.port->MODER &= ~(0x3UL << cs.pin*2 | 0x3UL << cs.pin*2 );
		miso.port->MODER &= ~(0x3UL << miso.pin*2 | 0x3UL << miso.pin*2 );
		mosi.port->MODER &= ~(0x3UL << mosi.pin*2 | 0x3UL << mosi.pin*2 );
		clock.port->MODER &= ~(0x3UL << clock.pin*2 | 0x3UL << clock.pin*2 );// GPIO_MODER_MODERA

		mosi.port->MODER |= 0x2UL << mosi.pin*2; //GPIO_MODER_MODERD_1 - AF output
		mosi.port->OTYPER &= ~(0x1UL << mosi.pin);
		mosi.port->OSPEEDR |= 0x3UL << mosi.pin*2;  //GPIO_OSPEEDER_OSPEEDRD
		mosi.port->PUPDR &= ~(0x3UL << mosi.pin*2);

		miso.port->MODER |= 0x2UL<< miso.pin*2; //GPIO_MODER_MODERC_1; // input
		miso.port->OTYPER &= ~(0x1UL << miso.pin);
		miso.port->PUPDR &= ~(0x3UL << miso.pin*2);

		clock.port->MODER |= 0x2UL<< clock.pin*2; //GPIO_MODER_MODERB_1; // AF output
		clock.port->OTYPER &= ~(0x1UL << clock.pin);
		clock.port->OSPEEDR |= 0x3UL << clock.pin*2;  //GPIO_OSPEEDER_OSPEEDRB
		clock.port->PUPDR &= ~(0x3UL << clock.pin*2);

		cs.port->MODER |= 0x1UL << cs.pin*2; //GPIO_MODER_MODERA_0; // general purpose output
		cs.port->OTYPER &= ~(0x1UL << cs.pin);
		cs.port->OSPEEDR |= 0x3UL << cs.pin*2;  //GPIO_OSPEEDER_OSPEEDRA
		cs.port->PUPDR &= ~(0x3UL << cs.pin*2);
		cs.port->BSRR = (1 << cs.pin); // CS initially set high

		if (spiIndex == 1)
		{
			clock.port->AFR[0] |= (5 << clock.pin%8*4);
			miso.port->AFR[0] |= (5 << miso.pin%8*4);
			mosi.port->AFR[0] |= (5 << mosi.pin%8*4);
		}
		if (spiIndex == 2)
		{
			clock.port->AFR[1] |= (5 << clock.pin%8*4);
			miso.port->AFR[0] |= (5 << miso.pin%8*4);
			mosi.port->AFR[0] |= (5 << mosi.pin%8*4);
		}
		if (spiIndex == 3)
		{
			clock.port->AFR[1] |= (6 << clock.pin%8*4);
			miso.port->AFR[1] |= (6 << miso.pin%8*4);
			mosi.port->AFR[1] |= (6 << mosi.pin%8*4);
		}

		m_spi->CR1 = 0 << SPI_CR1_DFF_Pos // 8bit frame
				| 0 << SPI_CR1_LSBFIRST_Pos      // MSB first
				| 1 << SPI_CR1_SSM_Pos           // Software SS
				| 1 << SPI_CR1_SSI_Pos           // SS high
				| 0x04 << SPI_CR1_BR_Pos         // Speed: F_PCLK/32
				| 1 << SPI_CR1_MSTR_Pos          // Master mode
				| 0 << SPI_CR1_CPOL_Pos
				| 0 << SPI_CR1_CPHA_Pos;         //SPI: 0

		m_spi->CR1 |= SPI_CR1_SPE;


		while(!(m_spi->SR & SPI_SR_TXE));      // make sure TX buffer is empty
		while(m_spi->SR & SPI_SR_BSY);         // make sure SPI isn't busy
	}

	void initPortAClock()
	{
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	}

	void initPortBClock()
	{
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;
	}

	void initPortCClock()
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}

	void initSpi1(Spi<Controller::f401>::SpiFrameSize frameSize)
	{
		initPortAClock();
		spiXInit(1, PortPinPair{GPIOA, 4}, PortPinPair{GPIOA, 5}, PortPinPair{GPIOA, 6}, PortPinPair{GPIOA, 7}, frameSize);
	}

	void initSpi2(Spi<Controller::f401>::SpiFrameSize frameSize)
	{
		initPortBClock();
		initPortCClock();
		spiXInit(2, PortPinPair{GPIOC, 9}, PortPinPair{GPIOB, 10}, PortPinPair{GPIOC, 2}, PortPinPair{GPIOC, 3}, frameSize);
	}

	void initSpi3(Spi<Controller::f401>::SpiFrameSize frameSize)
	{
		initPortCClock();
		spiXInit(3, {GPIOC, 9}, {GPIOC, 10}, {GPIOC, 11}, {GPIOC, 12}, frameSize);
	}
public:

	Spi(Spi<Controller::f401>::SpiNumber spiNumber, Spi<Controller::f401>::SpiFrameSize frameSize):
		m_frameSize(frameSize)
	{
		switch(spiNumber)
		{
		case SpiNumber::Spi1:
			initSpi1(frameSize);
			break;
		case SpiNumber::Spi2:
			initSpi2(frameSize);
			break;
		case SpiNumber::Spi3:
			initSpi3(frameSize);
			break;
		}
	}


	void sendData(uint8_t address, uint8_t data)
	{
		m_cs.port->BSRR = 1 << (m_cs.pin + 16U);  // CS RESET
		while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
		if (m_frameSize == Spi<Controller::f401>::SpiFrameSize::Bit8)
		{
			m_spi->DR = address;
			while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
			(void) m_spi->DR;
			while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
			m_spi->DR = data;
		}
		else
		{
			m_spi->DR = (uint16_t)address << 8 | data;
		}

		while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		(void) m_spi->DR;
		while(m_spi->SR&SPI_SR_BSY) {}
		m_cs.port->BSRR = 1 << m_cs.pin; // CS SET
	}

	void sendData(uint8_t address, uint8_t* data, int dataLength)
	{
		m_cs.port->BSRR = 1 << m_cs.pin << 16U;  // CS RESET
		while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
		if (m_frameSize == Spi<Controller::f401>::SpiFrameSize::Bit8)
		{
			for(int i = 0; i< dataLength; ++i)
			{
				m_spi->DR = address;
				while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
				(void) m_spi->DR;
				while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
				m_spi->DR = data[i];
				while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
				while(m_spi->SR&SPI_SR_BSY) {}
			}
		}
		else
		{
			for(int i = 0; i< dataLength; ++i)
			{
				m_spi->DR = (uint16_t)address << 8 | data[i];
				while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
				(void) m_spi->DR;
				while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
				while(m_spi->SR&SPI_SR_BSY) {}
			}
		}

		while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		(void) m_spi->DR;
		while(m_spi->SR&SPI_SR_BSY) {}
		m_cs.port->BSRR = 1 << m_cs.pin; // CS SET
	}

	void sendByte(uint8_t data)
	{
		m_cs.port->BSRR = 1 << m_cs.pin << 16U;  // CS RESET
		while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
		if (m_frameSize == Spi<Controller::f401>::SpiFrameSize::Bit8)
		{
			while(!(READ_BIT(m_spi->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
			m_spi->DR = data;
		}
		else
		{
			m_spi->DR = (uint16_t)data << 8;
		}

		while(!(READ_BIT(m_spi->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		(void) m_spi->DR;
		while(m_spi->SR&SPI_SR_BSY) {}
		m_cs.port->BSRR = 1 << m_cs.pin; // CS SET
	}
};

using SpiF401 = Spi<Controller::f401>;
#endif /* SPISTM32F401_H_ */

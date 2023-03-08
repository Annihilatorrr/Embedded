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

template <Controller> class Spi;

template <> class Spi<Controller::f401> {

public:
	enum SpiNumber{Spi1, Spi2};
	enum SpiFrameSize{Bit8, Bit16};
private:
	GPIO_TypeDef* m_port;
	uint8_t m_csPin;
	uint8_t m_clockPin;
	uint8_t m_misoPin;
	uint8_t m_mosiPin;
	SPI_TypeDef* m_spi;
	Spi<Controller::f401>::SpiFrameSize m_frameSize;

	void spiXInit(uint8_t spiIndex, GPIO_TypeDef* port, uint8_t csPin, uint8_t clockPin, uint8_t misoPin, uint8_t mosiPin, SpiFrameSize frameSize)
	{
		m_port = port;
		m_csPin = csPin;
		m_clockPin = clockPin;
		m_misoPin = misoPin;
		m_mosiPin = mosiPin;

		m_spi = spiIndex == 1 ? SPI1:SPI2;
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
		RCC->APB2ENR |=RCC_APB2ENR_SPI1EN ;
		GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7
				|GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5|GPIO_OSPEEDER_OSPEEDR6|GPIO_OSPEEDER_OSPEEDR7);

		// 7
		GPIOA->MODER |= GPIO_MODER_MODER7_1; // AF output
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT7;
		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_0;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7;

		GPIOA->MODER |= GPIO_MODER_MODER6_1; // input
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT6;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6;

		GPIOA->MODER |= GPIO_MODER_MODER5_1; // AF output
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;

		GPIOA->MODER |= GPIO_MODER_MODER4_0; // general purpose output
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT4;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4;

		GPIOA->AFR[0] |= (5 << 4*5) | (5 << 4*6) | (5 << 4*7);
		SPI1->CR1 = 0 << SPI_CR1_DFF_Pos // 8bit frame
				| 0 << SPI_CR1_LSBFIRST_Pos      // MSB first
				| 1 << SPI_CR1_SSM_Pos           // Software SS
				| 1 << SPI_CR1_SSI_Pos           // SS high
				| 0x04 << SPI_CR1_BR_Pos         // Speed: F_PCLK/32
				| 1 << SPI_CR1_MSTR_Pos          // Master mode
				| 0 << SPI_CR1_CPOL_Pos
				| 0 << SPI_CR1_CPHA_Pos;         //SPI: 0

		SPI1->CR1 |= SPI_CR1_SPE;


		while(!(SPI1->SR & SPI_SR_TXE));      // make sure TX buffer is empty
		while(SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy
		//		__IO uint32_t& csPortConfigRegister = csPin > 7 ? port-> : port->CRL;
		//		__IO uint32_t& clockPortConfigRegister = clockPin > 7 ? port->CRH : port->CRL;
		//		__IO uint32_t& misoPortConfigRegister = misoPin > 7 ? port->CRH : port->CRL;
		//		__IO uint32_t& mosiPortConfigRegister = mosiPin > 7 ? port->CRH : port->CRL;
		//
		//		uint32_t csPortConfigTemp = csPortConfigRegister & ~((0b11 << (csPin%8*4+2)) | (0b11 << csPin%8*4) | (0b11 << (clockPin%8*4+2)) | (0b11 << clockPin%8*4) | (0b11 << (misoPin%8*4+2)) | (0b11 << misoPin%8*4) | (0b11 << (mosiPin%8*4+2)) | (0b11 << mosiPin%8*4));
		//		csPortConfigRegister = csPortConfigTemp;
		//
		//		mosiPortConfigRegister   |=  (0b11 << mosiPin%8*4);  // output 50 MHz (11)
		//		mosiPortConfigRegister   &= ~(0b11 << (mosiPin%8*4+2));   // Push-Pull (00)
		//		mosiPortConfigRegister   |=  (0b10 << (mosiPin%8*4+2)); // alternative function push-pull (10)
		//
		//		misoPortConfigRegister   &= ~(0b11 << misoPin%8*4);  // Input (00)
		//		misoPortConfigRegister   |=  (0b10 << (misoPin%8*4+2)); // with pull-up / pull-down
		//		port->BSRR   =  (1 << misoPin);   // Set bit 14 High
		//
		//		clockPortConfigRegister   |=  (0b11 << clockPin%8*4);  // output 50 MHz
		//		clockPortConfigRegister   |=  (0b10 << (clockPin%8*4+2)); // alternative function push-pull
		//
		//		csPortConfigRegister   |=  (0b11 << csPin%8*4);  // output 50 MHz
		//		csPortConfigRegister   &= ~(0b11 << (csPin%8*4+2));	  // Push-Pull General Purpose
		//		port->BSRR  =   (1 << csPin);   // Set bit High
		//
		//		m_spi->CR1 = 0x0000; // reset SPI configuration registers
		//		m_spi->CR2 = 0x0000; // reset SPI configuration registers
		//
		//		if (spiIndex == 2)
		//		{
		//			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // enable spi clock
		//		}
		//
		//		if (spiIndex == 1)
		//		{
		//			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable spi clock
		//		}
		//
		//		m_spi->CR1   &= ~SPI_CR1_SPE; // disable SPI before configuring
		//		m_spi->CR1 = (frameSize == SpiFrameSize::Bit8 ? (0 << SPI_CR1_DFF_Pos):SPI_CR1_DFF)    // 8 bit Data frame format
		//				| 0 << SPI_CR1_LSBFIRST_Pos //  MSB transferred first
		//				| SPI_CR1_SSM               //Software SS
		//				| SPI_CR1_SSI               // NSS (CS) pin is high
		//				| SPI_CR1_BR_0 | SPI_CR1_BR_1  //Baud: F_PCLK/16
		//				| SPI_CR1_MSTR // Master mode
		//				| 0 << SPI_CR1_CPOL_Pos // Clock polarity
		//				| 0 << SPI_CR1_CPHA_Pos;  // Clock phase
		//
		//		m_spi->CR1 |= SPI_CR1_SPE; // Enable SPI


	}

	void initPortAClock()
	{
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	}

	void initPortBClock()
	{
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;
	}

	void initAltFunctionsClock()
	{


	}

	void initSpi1(Spi<Controller::f401>::SpiFrameSize frameSize)
	{
		initAltFunctionsClock();
		initPortAClock();
		spiXInit(1, GPIOA, 4, 5, 6, 7, frameSize);
	}

	void initSpi2(Spi<Controller::f401>::SpiFrameSize frameSize)
	{
		initAltFunctionsClock();
		initPortBClock();
		spiXInit(2, GPIOB, 12, 13, 14, 15, frameSize);
	}
public:

	Spi(Spi<Controller::f401>::SpiNumber spiNumber, Spi<Controller::f401>::SpiFrameSize frameSize):
		m_frameSize(frameSize)
	{
		switch(spiNumber)
		{
		case Spi1:
			initSpi1(frameSize);
			break;
		case Spi2:
			initSpi2(frameSize);
			break;
		}
	}


	void sendData(uint8_t address, uint8_t data)
	{
		m_port->BSRR = 1 << m_csPin << 16U;  // CS RESET
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
		m_port->BSRR = 1 << m_csPin; // CS SET
	}

	void sendData(uint8_t address, uint8_t* data, int dataLength)
	{
		m_port->BSRR = 1 << m_csPin << 16U;  // CS RESET
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
		m_port->BSRR = 1 << m_csPin; // CS SET
	}

	void sendByte(uint8_t data)
	{
		m_port->BSRR = 1 << m_csPin << 16U;  // CS RESET
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
		m_port->BSRR = 1 << m_csPin; // CS SET
	}
};

using SpiF401 = Spi<Controller::f401>;
#endif /* SPISTM32F401_H_ */

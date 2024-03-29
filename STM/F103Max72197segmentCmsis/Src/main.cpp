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
#include "display7segmentmax7219.h"
#include "ledmatrixmax7219.h"
//#include "spi.h"
#include "delay.h"

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

//void initSpi1(void)
//{
//	auto reg = GPIOA->CRL;
//	reg &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4 | GPIO_CRL_CNF5 | GPIO_CRL_MODE5 | GPIO_CRL_CNF6 | GPIO_CRL_MODE6 | GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
//	GPIOA->CRL = reg;
//
//	GPIOA->CRL   |=  GPIO_CRL_MODE7;  // output 50 MHz
//	GPIOA->CRL   &= ~GPIO_CRL_CNF7;	  // Push-Pull
//	GPIOA->CRL   |=  GPIO_CRL_CNF7_1; // alternative function push-pull
//
//	GPIOA->CRL   &= ~GPIO_CRL_MODE6;  // Input
//	GPIOA->CRL   |=  GPIO_CRL_CNF6_1; // with pull-up / pull-down
//	GPIOA->BSRR   =  GPIO_BSRR_BS6;   // Set bit 6 High
//
//	GPIOA->CRL   |=  GPIO_CRL_MODE5;  // output 50 MHz
//	GPIOA->CRL   |=  GPIO_CRL_CNF5_1; // alternative function push-pull
//
//	GPIOA->CRL   |=  GPIO_CRL_MODE4;  // output 50 MHz
//	GPIOA->CRL   &= ~GPIO_CRL_CNF4;	  // Push-Pull General Purpose
//	GPIOA->BSRR   =  GPIO_BSRR_BS4;   // Set bit 4 High
//
//	SPI1->CR1 = 0x0000; // reset SPI configuration registers
//	SPI1->CR2 = 0x0000; // reset SPI configuration registers
//
//	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable spi clock
//	SPI1->CR1   &= ~SPI_CR1_SPE; // disable SPI before configuring
//	SPI1->CR1 = 0 << SPI_CR1_DFF_Pos    // 8 bit Data frame format
//			| 0 << SPI_CR1_LSBFIRST_Pos //  MSB transferred first
//			| SPI_CR1_SSM               //Software SS
//			| SPI_CR1_SSI               // NSS (CS) pin is high
//			| SPI_CR1_BR_0 | SPI_CR1_BR_1  //Baud: F_PCLK/16
//			| SPI_CR1_MSTR // Master mode
//			| 0 << SPI_CR1_CPOL_Pos // Clock polarity
//			| 0 << SPI_CR1_CPHA_Pos;  // Clock phase
//
//	SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI
//}
//
//void spi2_init(void)
//{
//	GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE12 | GPIO_CRH_CNF13 | GPIO_CRH_MODE13 | GPIO_CRH_CNF14 | GPIO_CRH_MODE14 | GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
//
//	GPIOB->CRH   |=  GPIO_CRH_MODE15;  // output 50 MHz (11)
//	GPIOB->CRH   &= ~GPIO_CRH_CNF15;   // Push-Pull (00)
//	GPIOB->CRH   |=  GPIO_CRH_CNF15_1; // alternative function push-pull (10)
//
//	GPIOB->CRH   &= ~GPIO_CRH_MODE14;  // Input (00)
//	GPIOB->CRH   |=  GPIO_CRH_CNF14_1; // with pull-up / pull-down
//	GPIOB->BSRR   =  GPIO_BSRR_BS14;   // Set bit 14 High
//
//	GPIOB->CRH   |=  GPIO_CRH_MODE13;  // output 50 MHz
//	GPIOB->CRH   |=  GPIO_CRH_CNF13_1; // alternative function push-pull
//
//	GPIOB->CRH   |=  GPIO_CRH_MODE12;  // output 50 MHz
//	GPIOB->CRH   &= ~GPIO_CRH_CNF12;	  // Push-Pull General Purpose
//	GPIOB->BSRR   =  GPIO_BSRR_BS12;   // Set bit 12 High
//
//	SPI2->CR1 = 0x0000; // reset SPI configuration registers
//	SPI2->CR2 = 0x0000; // reset SPI configuration registers
//
//	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // enable spi clock
//
//	SPI2->CR1 &= ~SPI_CR1_SPE; // disable SPI before configuring
//	SPI2->CR1 = 0 << SPI_CR1_DFF_Pos    // 8 bit Data frame format
//			| 0 << SPI_CR1_LSBFIRST_Pos //  MSB transferred first
//			| SPI_CR1_SSM               //Software SS
//			| SPI_CR1_SSI               // NSS (CS) pin is high
//			| SPI_CR1_BR_0 | SPI_CR1_BR_1  //Baud: F_PCLK/16
//			| SPI_CR1_MSTR // Master mode
//			| 0 << SPI_CR1_CPOL_Pos // Clock polarity
//			| 0 << SPI_CR1_CPHA_Pos  // Clock phase
//			| SPI_CR1_SPE; // Enable SPI
//}

//void initPortAClock()
//{
//	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
//}
//
//void initPortBClock()
//{
//	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
//}
//void initDmaClock()
//{
//	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
//}
void initSwdOnlyDebugging()
{
	//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // JTAG is disabled
}
//
//void initAltFunctionsClock()
//{
//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
//}
//
//void sendData(uint8_t address, uint8_t data)
//{
//	GPIOA->BSRR = GPIO_BSRR_BR4; // CS LOW
//	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
//
//	SPI1->DR = address;
//	while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
//	(void) SPI1->DR;
//	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
//	SPI1->DR = data;
//
//	while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
//	(void) SPI1->DR;
//	while(SPI1->SR&SPI_SR_BSY) {}
//	GPIOA->BSRR = GPIO_BSRR_BS4;  // CS HIGH
//}

//enum OperationCode: uint8_t
//{
//	OP_DECODEMODE = 9,	///< MAX72xx opcode for DECODE MODE
//	OP_INTENSITY = 10,	///< MAX72xx opcode for SET INTENSITY
//	OP_SCANLIMIT = 11,	///< MAX72xx opcode for SCAN LIMIT
//	OP_SHUTDOWN = 12,	///< MAX72xx opcode for SHUT DOWN
//	OP_DISPLAYTEST = 15	///< MAX72xx opcode for DISPLAY TEST
//};

void singleLedTest(SpiF103& spi)
{
	LedMatrixMax7219<Controller::f103, 8, 4> lm(&spi, 1, 4, 8);
	for(int i = 0; i <= 7; ++i)
	{
		for(int j = 0; j < 8*4; ++j)
		{
			lm.setLed(i, j);
			delayMs(5);
		}
	}
	delayMs(2000);
	for(int i = 0; i <= 7; ++i)
	{
		for(int j = 0; j < 8*4; ++j)
		{
			lm.resetLed(i, j);
			delayMs(5);
		}
	}
}

void stringTest(SpiF103& spi)
{
	LedMatrixMax7219<Controller::f103, 8, 4> lm(&spi, 1, 4, 8);
	delayMs(5000);
	char str[]{"Saturday"};
	lm.shiftString(str, 30, 500);

	char str2[]{"Very Good Testing"};
	lm.shiftString(str2, 30, 500);

	char str3[]{"04.03.2023"};
	lm.shiftString(str3, 30, 500);
}

struct LedPoint
{
	uint32_t x;
	uint32_t y;
};

class Snake
{
	LedMatrixMax7219<Controller::f103, 8, 4> _lm;
	uint8_t m_currentSize;
	uint32_t m_maxX;
	uint32_t m_maxY;
	LedPoint points[256]{};
	LedPoint _m_initialPoint;

	enum class DirectionOnMatrix
	{
		Left,
		Top,
		Right,
		Bottom
	};

	DirectionOnMatrix m_currentDirection = DirectionOnMatrix::Left;
public:
	Snake(LedMatrixMax7219<Controller::f103, 8, 4>& lm, uint8_t initialSize, uint32_t maxX,	uint32_t maxY):
		_lm(lm),
		m_currentSize(initialSize),
		m_maxX(maxX),
		m_maxY(maxY)
	{
		_m_initialPoint = {maxX - m_currentSize + 1, 3};
		for(int i = 0; i < m_currentSize; ++i)
		{
			points[i] = {_m_initialPoint.x+i, _m_initialPoint.y};
			_lm.setLed(points[i].y, points[i].x);
		}
	}

	void moveLeft()
	{
		if (m_currentDirection != DirectionOnMatrix::Right)
		{
			m_currentDirection = DirectionOnMatrix::Left;
			_lm.resetLed(points[m_currentSize - 1].y, points[m_currentSize - 1].x);
			for(int i = m_currentSize - 1; i >0; --i)
			{
				points[i] = points[i-1];
				_lm.setLed(points[i].y, points[i].x);
			}
			points[0] = {--points[0].x, points[0].y};
			_lm.setLed(points[0].y, points[0].x);

		}
	}

	void moveRight()
	{
		if (m_currentDirection != DirectionOnMatrix::Left)
		{
			m_currentDirection = DirectionOnMatrix::Right;
			_lm.resetLed(points[m_currentSize - 1].y, points[m_currentSize - 1].x);
			for(int i = m_currentSize - 1; i >0; --i)
			{
				points[i] = points[i-1];
				_lm.setLed(points[i].y, points[i].x);
			}
			points[0] = {++points[0].x, points[0].y};
			_lm.setLed(points[0].y, points[0].x);

		}
	}

	void moveTop()
	{
		if (m_currentDirection != DirectionOnMatrix::Bottom)
		{
			m_currentDirection = DirectionOnMatrix::Top;
			_lm.resetLed(points[m_currentSize - 1].y, points[m_currentSize - 1].x);
			for(int i = m_currentSize - 1; i >0; --i)
			{
				points[i] = points[i-1];
				_lm.setLed(points[i].y, points[i].x);
			}
			points[0] = {points[0].x, ++points[0].y};
			_lm.setLed(points[0].y, points[0].x);

		}
	}

	void moveBottom()
	{
		if (m_currentDirection != DirectionOnMatrix::Top)
		{
			m_currentDirection = DirectionOnMatrix::Bottom;
			_lm.resetLed(points[m_currentSize - 1].y, points[m_currentSize - 1].x);
			for(int i = m_currentSize - 1; i >0; --i)
			{
				points[i] = points[i-1];
				_lm.setLed(points[i].y, points[i].x);
			}
			points[0] = {points[0].x, --points[0].y};
			_lm.setLed(points[0].y, points[0].x);

		}
	}
};
SpiF103 spi2(SpiF103::Spi2, SpiF103::SpiFrameSize::Bit8, false, false);

volatile uint32_t sysTick = 0;
volatile uint32_t msTicks = 0;

bool checkKeys = false;
extern "C" void SysTick_Handler(void)
{
	++msTicks;
	if (sysTick > 0)
	{
		--sysTick;
	}
	if (checkKeys && msTicks%250)
	{
		uint8_t keys = 0, key_mas[4];
					spi2.readData(0x42, key_mas, 4);
					keys = (key_mas[3]&0x11) << 3 | (key_mas[2]&0x11) << 2 | (key_mas[1]&0x11) << 1 | (key_mas[0]&0x11);
					for(int start = 0;start < 8; start ++)
					{
						spi2.sendData(0xC1+start*2, keys&0x01);
						keys >>= 1;
					}
	}
}

int main(void)
{
	clockInit();
	SysTick_Init(72000000);
	initSwdOnlyDebugging();
	SpiF103 spi1(SpiF103::Spi1, SpiF103::SpiFrameSize::Bit8, true, true);
//	Display7segmentMax7219<Controller::f103> display2(&spi1);
//	display2.init(7, 8);
//	display2.print(-82212);
//
//	for(int i = 1;i <= 10;++i)
//	{
//		display2.clean();
//		display2.print(i);
//		delayMs(100);
//	}
	uint8_t sym_mas[10] = { // ������ ���� ��� ����������
			0x3F, // 0
			0x06, // 1
			0x5B, // 2
			0x4F, // 3
			0x66, // 4
			0x6D, // 5
			0x7D, // 6
			0x07, // 7
			0x7F, // 8
			0x6F  // 9
	};



	// init
	uint8_t brightness = 7;
	spi2.sendByte(0x88|(brightness & 0x07));

	spi2.sendByte(0x40); // ������������� ������
	uint8_t zeros[17]{0xC0};
	spi2.sendData(zeros, 17); // ��������� �����

	spi2.sendByte(0x44); // ������ �� ��������������� ������
	for(int start = 0;start < 8; start ++)
	{
		spi2.sendData(0xC1+start*2, 1&0x01);
	}
	delayMs(100);
	for(int start = 0;start < 8; start ++)
	{
		spi2.sendData(0xC1+start*2, 0&0x01);
	}
	delayMs(100);
	for(int start = 0;start < 8; start ++)
	{
		spi2.sendData(0xC0+start*2, sym_mas[start]);
	}
	delayMs(100);
		int testCounter = 1;
		while(testCounter--)
		{
			stringTest(spi1);
		}
		checkKeys = true;
	while(1){
//		uint8_t keys = 0, key_mas[4];
//			spi2.readData(0x42, key_mas, 4);
//			keys = (key_mas[3]&0x11) << 3 | (key_mas[2]&0x11) << 2 | (key_mas[1]&0x11) << 1 | (key_mas[0]&0x11);
//			for(int start = 0;start < 8; start ++)
//			{
//				spi2.sendData(0xC1+start*2, keys&0x01);
//				keys >>= 1;
//			}
//			delayMs(1000);
	}
	//	SpiF103 spi2(SpiF103::Spi2, SpiF103::SpiFrameSize::Bit8);
	//
	//	return 0;
	//	LedMatrixMax7219<Controller::f103, 8, 4> lm(&spi2, 1, 4, 8);
	//	Snake s(lm, 8, 31, 7);
	//
	//	int delayBetweenSteps = 1000;
	//
	//	s.moveLeft();
	//	delayMs(delayBetweenSteps);
	//	s.moveBottom();
	//	delayMs(delayBetweenSteps);
	//	s.moveLeft();
	//	delayMs(delayBetweenSteps);
	//	s.moveBottom();
	//	delayMs(delayBetweenSteps);
	//	for (int i = 0; i < 15; ++i)
	//	{
	//		s.moveLeft();
	//		delayMs(delayBetweenSteps);
	//	}
	//
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		s.moveTop();
	//		delayMs(delayBetweenSteps);
	//	}
	//
	//	for (int i = 0; i < 17; ++i)
	//	{
	//		s.moveRight();
	//		delayMs(delayBetweenSteps);
	//	}
	//
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		s.moveBottom();
	//		delayMs(delayBetweenSteps);
	//	}
	//singleLedTest(spi1);
	//stringTest(spi1);
	//lm.init();
	//lm.update();
	//	initPortAClock();
	//	initAltFunctionsClock();
	//	initSpi1();
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		sendData(OperationCode::OP_DISPLAYTEST, 0x00);
	//	}
	//	for (int i = 0; i < 4; ++i)
	//		sendData(OperationCode::OP_SCANLIMIT, 0x0f);   //  scan limit = 8 LEDs
	//	for (int i = 0; i < 4; ++i)
	//		sendData(OperationCode::OP_INTENSITY, 0);       //  brightness intensity
	//	for (int i = 0; i < 4; ++i)
	//		sendData(OperationCode::OP_SHUTDOWN, 0x01);    //  power down = 0, normal mode = 1
	//	for (int i = 0; i < 4; ++i)
	//		sendData(OperationCode::OP_INTENSITY, 7);       //  brightness intensity
	//
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		for (int i = 0; i <= 7; i++)
	//		{
	//			setColumn(i, 0);
	//		}
	//	}

	//uint8_t str[]{uint8_t(127), uint8_t(127), uint8_t(96)};
	//lm.displayString(str, 3);

	for (int i = 0; i < 100;++i)
	{
		//delayMs(50);
		//lm.shiftLeft();
	}

	//lm.test();
	//	GPIOA->BSRR = GPIO_BSRR_BR4; // CS LOW
	//	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		SPI1->DR = 1;
	//
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	//		(void) SPI1->DR;
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	//		SPI1->DR = buf[i][0];
	//
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	//		(void) SPI1->DR;
	//		while(SPI1->SR&SPI_SR_BSY) {}
	//	}
	//	GPIOA->BSRR = GPIO_BSRR_BS4;  // CS HIGH
	//	GPIOA->BSRR = GPIO_BSRR_BR4; // CS LOW
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		SPI1->DR = 2;
	//
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	//		(void) SPI1->DR;
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	//		SPI1->DR = buf[i][1];
	//
	//		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	//		(void) SPI1->DR;
	//		while(SPI1->SR&SPI_SR_BSY) {}
	//	}
	//	GPIOA->BSRR = GPIO_BSRR_BS4;  // CS HIGH
	//	Display7segmentMax7219<Controller::f103> display1(&spi1);
	//
	//	display1.init(15, 8);
	//	display1.print(-82212);
	//
	//	for(int i = 0;i <= 100;++i)
	//	{
	//		display1.clean();
	//		display1.print(i);
	//		delayMs(10);
	//	}


}

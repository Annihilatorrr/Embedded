/*
 * application.cpp
 *
 *  Created on: Dec 9, 2022
 *      Author: VertexNi
 */

#include "application.h"

//extern "C" void DMA1_Channel3_IRQHandler(void)
//{
//	int r = 0;
//	r++;
//	int d = r+1;
//}


Application::Application() {
	clockInit();
	// TODO Auto-generated constructor stub

}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

int Application::clockInit(void)

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

void Application::initAltFunctionsClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable A.F.Z
}

void Application::initDmaClock()
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
}

void Application::initPortAClock()
{

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable PortC
}

void Application::initPortBClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable PortC
}

void Application::initPortCClock()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable PortC
}

void Application::initSwdOnlyDebugging()
{
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // JTAG is disabled
}

void Application::setPin(GPIO_TypeDef* port, uint8_t pin)
{
	port->BSRR = (1 << pin);
}

void Application::resetPin(GPIO_TypeDef* port, uint8_t pin)
{
	port->BRR = (1 << pin);
}

void Application::initOuputPushPull2Mhz()
{
//	// GPIO Settings
//	GPIOC->CRH = 0;
//	GPIOC->CRL = 0;
//	GPIOA->CRH = 0;
//	GPIOA->CRL = 0;
//	GPIOB->CRL = 0;
//
//	//MODE: output 2 МГц
//	//CNF: push-pull
//	GPIOC->CRH |= (0x02 << GPIO_CRH_MODE13_Pos); // PC13
//	GPIOC->CRH |= (0x02 << GPIO_CRH_MODE14_Pos); // PC14
//	GPIOA->CRL |= (0x02 << GPIO_CRL_MODE0_Pos); // PA0
//	GPIOB->CRL |= (0x02 << GPIO_CRL_MODE0_Pos); // PB0 DC
//	GPIOB->CRL |= (0x02 << GPIO_CRL_MODE1_Pos); // PB1 RESET
//	GPIOB->CRH |= (0x02 << GPIO_CRH_MODE10_Pos); // PB10 CS
}

void Application::spi1_init(void)
{
	//Включаем тактирование SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// configuration of SPI pins
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk
			| GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk
			| GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk);

	// SCK:
	// MODE5 = 0x03 (11b) - output 50mhz;
	// CNF5  = 0x02 (10b) - Alternate function output Push-pull
	GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_CNF5_1;

	// MISO:
	// MODE6 = 0x00 (00b) - input;
	// CNF6  = 0x01 (01b) -  Floating input
	GPIOA->CRL |= GPIO_CRL_CNF6_0;

	// MOSI:
	// MODE7 = 0x03 (11b) - output 50mhz;
	// CNF7  = 0x02 (10b) - Alternate function output Push-pull
	GPIOA->CRL |= GPIO_CRL_MODE7 | GPIO_CRL_CNF7_1;

//	// CS:
//	// MODE4 = 0x03 (11b) - output 50mhz;
//	// CNF4 = 0x00 (10b)
//	GPIOA->CRL |= GPIO_CRL_MODE4;
//
	GPIOB->CRL = 0;
	GPIOB->CRH = 0;

	// DC for display:
	// MODE0 = 0x03 (11b) - output 50mhz;
	GPIOB->CRL |= GPIO_CRL_MODE0_1;
	//GPIOB->BSRR   =  GPIO_BSRR_BS0;

	// RST for display
	GPIOB->CRL |= GPIO_CRL_MODE1_1;

	// CS for display
	GPIOB->CRH |= GPIO_CRH_MODE10_1;



	SPI1->CR1 = 0x0000;
	SPI1->CR2 = 0x0000;

	SPI1->CR1 = 0<<SPI_CR1_DFF_Pos  //Размер кадра 8 бит
			| 0<<SPI_CR1_LSBFIRST_Pos     //MSB first
			| SPI_CR1_SSM          //Программное управление SS
			| SPI_CR1_SSI          //SS в высоком состоянии
			| SPI_CR1_BR_0        //Скорость передачи: F_PCLK/32
			| SPI_CR1_MSTR // master mode
			| SPI_CR1_SPE;
	// | 0<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos; //Режим работы SPI: 0

	SPI1->CR2 |= 1<<SPI_CR2_TXDMAEN_Pos;
	SPI1->CR2 |= 1<<SPI_CR2_RXDMAEN_Pos;
	SPI1->CR1 |= SPI_CR1_SPE; //Включаем SPI

	DMA1->IFCR = DMA_IFCR_CTCIF3; //сбрасываем флаг прерывания

	NVIC_EnableIRQ(DMA1_Channel3_IRQn); //разрешаем прерывания
	//от канала 3 DMA1
}

void Application::spiDma_Send(uint8_t *data, uint16_t len)
{
	//disable data transfer
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;

	DMA1_Channel3->CPAR = (uint32_t)&(SPI1->DR); //assign DR address to CPAR
	DMA1_Channel3->CMAR = (uint32_t)data;        //assign data address to CMAR
	DMA1_Channel3->CNDTR = len;                  // length of data to be transfered

	//DMA channel setup
	DMA1_Channel3->CCR =
			0 << DMA_CCR_MEM2MEM_Pos    //MEM2MEM off
			| 0x00 << DMA_CCR_PL_Pos    // low priority
			| 0x00 << DMA_CCR_MSIZE_Pos //memory data size 8 bit
			| 0x00 << DMA_CCR_PSIZE_Pos //peripheral data size 8 bit
			| DMA_CCR_MINC              //Memory increment on
			| 0 << DMA_CCR_PINC_Pos     // Peripheral increment off
			| 0 << DMA_CCR_CIRC_Pos     //Circular mode off
			| DMA_CCR_DIR               //Mem2Per
			| DMA_CCR_TCIE              // Enable Transfer complete interrupt
			| DMA_CCR_TEIE;             // Enable Transfer error interrupt
	DMA1_Channel3->CCR |= DMA_CCR_EN; //enable data
}

void Application::initPeriferal()
{
	initPortAClock();
	initPortBClock();
	initPortCClock();
	initDmaClock();
	initAltFunctionsClock();
	initOuputPushPull2Mhz();
	//initSwdOnlyDebugging();

	spi1_init();
}

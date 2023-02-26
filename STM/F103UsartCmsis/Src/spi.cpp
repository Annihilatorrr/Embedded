// STM32F103 SPI1

// PA4  - (OUT)	SPI1_NSS
// PA5  - (OUT)	SPI1_SCK
// PA6  - (IN)	SPI1_MISO (Master In)
// PA7  - (OUT)	SPI1_MOSI (Master Out)

#include "stm32f1xx.h"
#include "spi.h"

void initSPI1(void) {
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN);

	// NSS, 50MHz
	GPIOA->CRL   |=  GPIO_CRL_MODE4;
	GPIOA->CRL   &= ~GPIO_CRL_CNF4;
	GPIOA->BSRR   =  GPIO_BSRR_BS4;

	// SCK, 50MHz
	GPIOA->CRL   |=  GPIO_CRL_MODE5;
	GPIOA->CRL   &= ~GPIO_CRL_CNF5;
	GPIOA->CRL   |=  GPIO_CRL_CNF5_1;

	// MISO
	GPIOA->CRL   &= ~GPIO_CRL_MODE6;
	GPIOA->CRL   &= ~GPIO_CRL_CNF6;
	GPIOA->CRL   |=  GPIO_CRL_CNF6_1;
	GPIOA->BSRR   =  GPIO_BSRR_BS6;

	// MOSI, 50MHz
	GPIOA->CRL   |=  GPIO_CRL_MODE7;
	GPIOA->CRL   &= ~GPIO_CRL_CNF7;
	GPIOA->CRL   |=  GPIO_CRL_CNF7_1;

	// SPI
	SPI1->CR2     = 0x0000;
	SPI1->CR1     = SPI_CR1_MSTR;	// ������
	SPI1->CR1    |= SPI_CR1_BR;		// ��������� �������� SPI Baud rate = Fpclk/256	(2,4,8,16,32,64,128,256)
	SPI1->CR1    |= SPI_CR1_SSM;	// ����������� ����� NSS
	SPI1->CR1    |= SPI_CR1_SSI;	// ���������� ���������, ����� �� ����� NSS ������� �������
	SPI1->CR1    |= SPI_CR1_SPE;	// ��������� ������ ������ SPI
//	SPI1->CR1 &= ~SPI_CR1_CPOL; 	// ���������� ��������� ������� (CK to 0 when idle)
//	SPI1->CR1 &= ~SPI_CR1_CPHA; 	// ���� ��������� ������� (|= SPI_CR1_CPHA - �� ������� ������)
//	SPI1->CR1 |= SPI_CR1_DFF; 		// 16 ��� ������
//	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;	// ������� ��� (MSB) ���������� ������
//	SPI1->CR2 |= SPI_CR2_SSOE; 		// NSS - ������������ ��� ����� ���������� slave select
}

//uint8_t SPI1SendByte(uint8_t data) {
//	while (!(SPI1->SR & SPI_SR_TXE));      				// ���������, ��� ���������� �������� ��������� (STM32F103)
//	SPI1->DR=data;										// ����� � SPI1
//	while (!(SPI1->SR & SPI_SR_RXNE));     				// ���� ��������� ������ (STM32F103)
//	return SPI1->DR;		         					// ������ �������� ������
//}

uint8_t SPI1sendData(uint8_t rg, uint8_t dt)
{
	SPI1_NSS_ON();

	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	SPI1->DR = rg;
	while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	(void) SPI1->DR;
	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	SPI1->DR = dt;
	while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	(void) SPI1->DR;

	SPI1_NSS_OFF();
	return SPI1->DR;
}

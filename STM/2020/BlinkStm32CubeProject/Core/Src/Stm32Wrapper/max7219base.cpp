/*
 * max7219base.cpp
 *
 *  Created on: 30 дек. 2020 г.
 *      Author: rusla
 */

#include "max7219base.h"

Max7219Base::Max7219Base()
{
	m_spi.init();

}

void Max7219Base::sendByte(uint8_t data)
{
    SPI1->DR = data;
    while(!(SPI1->SR & SPI_SR_TXE));// make sure TX buffer is empty
    while(SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy
}

void Max7219Base::sendCommand(uint8_t address, uint8_t command)
{
    GPIOA->ODR &= ~GPIO_ODR_ODR_4;    // chip select
    sendByte(address);
    sendByte(command);
    GPIOA->ODR |= GPIO_ODR_ODR_4;              // deselect
}

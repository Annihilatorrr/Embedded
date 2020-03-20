/* 
* Spi.cpp
*
* Created: 3/19/2020 11:02:56 PM
* Author: VertexLT
*/


#include "Spi.h"

#include <avr/io.h>

Spi::Spi(uint8_t sck = PB5, uint8_t mosi = PB3, uint8_t ss = PB2):
m_sck(sck), m_mosi(mosi), m_ss(m_ss)
{
    DDRB |= (1 << sck); // set SCK as output
    DDRB |= (1 << mosi); // set MOSI as output
    DDRB |= (1 << ss); // set CS as output

    PORTB |= (1 << sck); // set SCK high
    PORTB |= (1 << mosi); // set MOSI high
    PORTB |= (1 << ss); // set CS high
}

void Spi::setCSLow()
{
    PORTB &= ~(1 << PB2); // set CS low
}

void Spi::setCSHigh()
{
    PORTB |= (1 << PB2); // set CS high
}
void Spi::send(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

void Spi::enableSpi()
{
    SPCR |= (1 << SPE);  // enable SPI
}

void Spi::setAsMaster()
{
    SPCR |= (1 << MSTR); // this device is SPI master
}
void Spi::prescaleOsc8(void)
{
    SPCR |= (1 << SPI2X); // clock prescale osc/8
}
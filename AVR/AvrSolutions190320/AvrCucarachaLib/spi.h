/* 
* Spi.h
*
* Created: 3/19/2020 11:02:56 PM
* Author: VertexLT
*/


#ifndef __SPI_H__
#define __SPI_H__

#include <avr/io.h>
class Spi
{
    int m_sck;
    int m_mosi;
    int m_miso;
    int m_ss;
    public:
    Spi(uint8_t sck, uint8_t mosi, uint8_t ss);
    void enableSpi();
    void setAsMaster();
    void prescaleOsc8();
    
    void send(uint8_t data);
    void setCSLow();
    void setCSHigh();
};



#endif //__SPI_H__

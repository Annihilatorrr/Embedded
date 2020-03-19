/*
* SevenSegmentDisplay.cpp
*
* Created: 3/19/2020 11:05:57 PM
* Author: VertexLT
*/


#include "sevensegmentdisplay.h"

#include <stdlib.h>

SevenSegmentDisplay::SevenSegmentDisplay(unsigned char sck, unsigned char mosi, unsigned char ss):
m_spi(sck, mosi, ss)
{
    m_lastDisplayedDigit = 0;
    m_spi.enableSpi();
    m_spi.setAsMaster();
    m_spi.prescaleOsc8();
}

void SevenSegmentDisplay::setIntensity(uint8_t intensity)
{
    DisplaySendCommand((uint8_t)Commands::MAX7219_MODE_INTENSITY, intensity);
}

void SevenSegmentDisplay::DisplayInitialize()
{
    DisplaySendCommand(0x0C, 0x01); // shutdown mode: normal operation
    DisplaySendCommand(0x09, 0xff); // decode mode: BCD for all digits
    DisplaySendCommand(0x0A, 0x0f); // intensity: brightest (last byte)
}

void SevenSegmentDisplay::DisplayInt64Number(long number, bool divideTriad)
{
    unsigned char digits[8]{};
    bool isNegative = number < 0;
    number = abs(number);
    int count = 0;
    do
    {
        unsigned char thisDigit = number % 10;
        digits[7-count] = thisDigit;
        count++;
        number /= 10;
        
    } while(number != 0);
    
    if (isNegative)
    {
        DisplaySendCommand(count + 1, (uint8_t)Commands::MAX7219_CHAR_NEGATIVE);
        turnOnDigit(count);
    }
    else
    {
        turnOnDigit(count-1);
    }

    for (int i = 0; i < count; i++)
    {
        unsigned char thisDigit = digits[7-i];
        DisplaySendCommand(i + 1, thisDigit);

    }

}

void SevenSegmentDisplay::DisplayClear()
{
    for (int i = 1; i <= 8; i++)
    {
        DisplaySendCommand(i, (unsigned char)Commands::MAX7219_CHAR_BLANK);
    }
}

void SevenSegmentDisplay::DisplaySendCommand(uint8_t address, uint8_t data)
{
    m_spi.setCSLow();
    m_spi.send(address);
    m_spi.send(data);
    m_spi.setCSHigh();
}

void SevenSegmentDisplay::turnOnDigit(unsigned char digitIndex)
{
    if (m_lastDisplayedDigit != digitIndex)
    {
        DisplaySendCommand(0x0B, digitIndex); // display size: max character address
        m_lastDisplayedDigit = digitIndex;
    }
}
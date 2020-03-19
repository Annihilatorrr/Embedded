/*
* SevenSegmentDisplay.h
*
* Created: 3/19/2020 11:05:57 PM
* Author: VertexLT
*/


#ifndef __SEVENSEGMENTDISPLAY_H__
#define __SEVENSEGMENTDISPLAY_H__

#include "spi.h"
#include <avr/io.h>

class SevenSegmentDisplay
{
    Spi m_spi;
    int m_lastDisplayedDigit = 0;
    public:
    SevenSegmentDisplay(unsigned char sck = PB5, unsigned char mosi = PB3, unsigned char ss = PB2);
    void setIntensity(uint8_t intensity);
    void DisplayInitialize();
    void DisplayInt64Number(long number, bool divideTriad = false);

    void DisplayClear();

    void DisplaySendCommand(uint8_t address, uint8_t data);

    void turnOnDigit(unsigned char digitIndex);
    
    enum class Commands: uint8_t
    {
        MAX7219_CHAR_BLANK = 0x0f,
        MAX7219_MODE_INTENSITY = 0x0A,
        MAX7219_CHAR_NEGATIVE = 0xa
    };

}; //SevenSegmentDisplay

#endif //__SEVENSEGMENTDISPLAY_H__

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

    void DisplaySendCommand(uint8_t address, uint8_t data);

    char m_letterToCode[256]{};
    public:

    enum class Commands: uint8_t
    {
        MAX7219_CHAR_BLANK = 0x0f,
        MAX7219_MODE_INTENSITY = 0x0A,
        MAX7219_CHAR_NEGATIVE = 0xa,
        MAX7219_MODE_SHUTDOWN = 0x0C
    };

    enum class Segment: uint8_t
    {
        DP = 0b10000000,
        A = 0b01000000,
        B = 0b00100000,
        C = 0b00010000,
        D = 0b00001000,
        E = 0b00000100,
        F = 0b00000010,
        G = 0b00000001,
    };


    SevenSegmentDisplay(uint8_t sck = PB5, uint8_t mosi = PB3, uint8_t ss = PB2);
    void setIntensity(uint8_t intensity);
    void DisplayInitialize();
    void DisplayInt64Number(long number, bool divideTriad = false);
    void displayChar(uint8_t c, uint8_t digitIndex);
    void displaySegment(Segment segmentIndex, uint8_t digitIndex);
    void DisplayClear();

    void turnOnDigit(uint8_t digitIndex);
    void setDecodeMode();
    void resetDecodeMode();


}; //SevenSegmentDisplay

#endif //__SEVENSEGMENTDISPLAY_H__

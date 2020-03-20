/*
* SevenSegmentDisplay.cpp
*
* Created: 3/19/2020 11:05:57 PM
* Author: VertexLT
*/


#include "sevensegmentdisplay.h"

#include <stdlib.h>

SevenSegmentDisplay::SevenSegmentDisplay(uint8_t sck, uint8_t mosi, uint8_t ss):
m_spi(sck, mosi, ss)
{
    m_lastDisplayedDigit = 0;
    m_spi.enableSpi();
    m_spi.setAsMaster();
    m_spi.prescaleOsc8();

    m_letterToCode['-'] = 0b00000001;
    m_letterToCode['_'] = 0b00010000;
    m_letterToCode['.'] = 0b10000000;
    m_letterToCode['0'] = 0x7E;
    m_letterToCode['1'] = 0x30;
    m_letterToCode['2'] = 0x6D;
    m_letterToCode['3'] = 0x79;
    m_letterToCode['4'] = 0x33;
    m_letterToCode['5'] = 0x5B;
    m_letterToCode['6'] = 0x5F;
    m_letterToCode['7'] = 0x70;
    m_letterToCode['8'] = 0x7F;
    m_letterToCode['9'] = 0x7B;
    m_letterToCode[' '] = 0x00;
    m_letterToCode['A'] = 0x77;
    m_letterToCode['a'] = 0x7D;
    m_letterToCode['B'] = 0x7F;
    m_letterToCode['b'] = 0x1F;
    m_letterToCode['C'] = 0x4E;
    m_letterToCode['c'] = 0x0D;
    m_letterToCode['D'] = 0x7E;
    m_letterToCode['d'] = 0x3D;
    m_letterToCode['E'] = 0x4F;
    m_letterToCode['e'] = 0x6f;
    m_letterToCode['F'] = 0x47;
    m_letterToCode['f'] = 0x47;
    m_letterToCode['G'] = 0x5E;
    m_letterToCode['g'] = 0x7B;
    m_letterToCode['H'] = 0x37;
    m_letterToCode['h'] = 0x17;
    m_letterToCode['I'] = 0x30;
    m_letterToCode['i'] = 0x10;
    m_letterToCode['J'] = 0x3C;
    m_letterToCode['j'] = 0x38;
    m_letterToCode['K'] = 0x37;
    m_letterToCode['k'] = 0x17;
    m_letterToCode['L'] = 0x0E;
    m_letterToCode['l'] = 0x06;
    m_letterToCode['M'] = 0x55;
    m_letterToCode['m'] = 0x55;
    m_letterToCode['N'] = 0x15;
    m_letterToCode['n'] = 0x15;
    m_letterToCode['O'] = 0x7E;
    m_letterToCode['o'] = 0x1D;
    m_letterToCode['P'] = 0x67;
    m_letterToCode['p'] = 0x67;
    m_letterToCode['Q'] = 0x73;
    m_letterToCode['q'] = 0x73;
    m_letterToCode['R'] = 0x77;
    m_letterToCode['r'] = 0x05;
    m_letterToCode['S'] = 0x5B;
    m_letterToCode['s'] = 0x5B;
    m_letterToCode['T'] = 0x46;
    m_letterToCode['t'] = 0x0F;
    m_letterToCode['U'] = 0x3E;
    m_letterToCode['u'] = 0x1C;
    m_letterToCode['V'] = 0x27;
    m_letterToCode['v'] = 0x23;
    m_letterToCode['W'] = 0x3F;
    m_letterToCode['w'] = 0x2B;
    m_letterToCode['X'] = 0x25;
    m_letterToCode['x'] = 0x25;
    m_letterToCode['Y'] = 0x3B;
    m_letterToCode['y'] = 0x33;
    m_letterToCode['Z'] = 0x6D;
    m_letterToCode['z'] = 0x6D;
}

void SevenSegmentDisplay::setIntensity(uint8_t intensity)
{
    DisplaySendCommand((uint8_t)Commands::MAX7219_MODE_INTENSITY, intensity);
}

void SevenSegmentDisplay::setDecodeMode()
{
    DisplaySendCommand(0x09, 0xff); // decode mode: BCD for all digits
}

void SevenSegmentDisplay::resetDecodeMode()
{
    DisplaySendCommand(0x09, 0x00); // decode mode: BCD for all digits
}

void SevenSegmentDisplay::DisplayInitialize()
{
    DisplaySendCommand((uint8_t)Commands::MAX7219_MODE_SHUTDOWN, 0x01); // shutdown mode: normal operation
}

void SevenSegmentDisplay::DisplayInt64Number(long number, bool divideTriad)
{
    uint8_t digits[8]{};
    bool isNegative = number < 0;
    number = abs(number);
    int count = 0;
    do
    {
        uint8_t thisDigit = number % 10;
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
        uint8_t thisDigit = digits[7-i];
        DisplaySendCommand(i + 1, thisDigit);
    }
}

void SevenSegmentDisplay::DisplayClear()
{
    for (int i = 1; i <= 8; i++)
    {
        DisplaySendCommand(i, (uint8_t)Commands::MAX7219_CHAR_BLANK);
    }
}

void SevenSegmentDisplay::DisplaySendCommand(uint8_t address, uint8_t data)
{
    m_spi.setCSLow();
    m_spi.send(address);
    m_spi.send(data);
    m_spi.setCSHigh();
}

void SevenSegmentDisplay::turnOnDigit(uint8_t digitIndex)
{
    if (m_lastDisplayedDigit != digitIndex)
    {
        DisplaySendCommand(0x0B, digitIndex); // display size: max character address
        m_lastDisplayedDigit = digitIndex;
    }
}

void SevenSegmentDisplay::displayChar(uint8_t c, uint8_t digitIndex)
{
    const char F  = m_letterToCode[c];
    DisplaySendCommand(digitIndex, F);
}

void SevenSegmentDisplay::displaySegment(Segment segmentIndex, uint8_t digitIndex)
{
    DisplaySendCommand(digitIndex, (uint8_t)segmentIndex);
}

void SevenSegmentDisplay::displaySegments(uint8_t segmentIndex, uint8_t digitIndex)
{
    DisplaySendCommand(digitIndex, segmentIndex);
}
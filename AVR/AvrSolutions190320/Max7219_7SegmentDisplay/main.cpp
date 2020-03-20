/*
* Max7219_7SegmentDisplay.cpp
*
* Created: 3/19/2020 11:01:08 PM
* Author : VertexLT
*/

#define F_CPU 8000000UL
#include "sevensegmentdisplay.h"
#include <util/delay.h>

void demoNumbers()
{
    SevenSegmentDisplay s7display;

    long number = -1;
    s7display.DisplayInitialize();
    while(true)
    {
        while(true)
        {
            _delay_ms(1500);
            //s7display.setIntensity(number%10);
            s7display.DisplayInt64Number(++number);
            
            if (number == 2)
            {
                //s7display.DisplayClear();
                break;
            }

        }
        
        _delay_ms(1000);
        s7display.DisplayClear();
        _delay_ms(200);
        s7display.DisplayInt64Number(number);
        _delay_ms(200);
        s7display.DisplayClear();
        _delay_ms(200);
        s7display.DisplayInt64Number(number);
        _delay_ms(200);
        s7display.DisplayClear();
        _delay_ms(200);
        s7display.DisplayInt64Number(number);
        _delay_ms(1000);
        while(true)
        {
            //s7display.setIntensity(number%10);
            _delay_ms(1500);
            s7display.DisplayInt64Number(--number);
            if (number == -2)
            {
                //s7display.DisplayClear();
                break;
            }
        }
        _delay_ms(1000);
        s7display.DisplayClear();
        _delay_ms(200);
        s7display.DisplayInt64Number(number);
        _delay_ms(200);
        s7display.DisplayClear();
        _delay_ms(200);
        s7display.DisplayInt64Number(number);
        _delay_ms(200);
        s7display.DisplayClear();
        _delay_ms(200);
        s7display.DisplayInt64Number(number);
        _delay_ms(1000);
    }
}
void demoNonDecodeMode()
{
    SevenSegmentDisplay s7display;
    s7display.DisplayInitialize();
    s7display.DisplayClear();
    s7display.resetDecodeMode();
    s7display.setIntensity(0X0f);
    s7display.turnOnDigit(7);
    for (int i = 1; i <= 8; ++i)
    {
        s7display.displayChar('-', i);
    }
    _delay_ms(1000);
    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::A, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::A, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::A, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::B, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::C, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::D, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::E, i);
    }
    _delay_ms(50);

    for (int i = 1; i <= 8; ++i)
    {
        s7display.displaySegment(SevenSegmentDisplay::Segment::F, i);
    }
    _delay_ms(50);
    //s7display.displayChar('-', 1);
    //s7display.displayChar('-', 2);
    //s7display.displayChar('-', 4);
    //s7display.displayChar('-', 4);
    //s7display.displayChar('-', 5);
    //s7display.displayChar('-', 6);
    //s7display.displayChar('-', 7);
    //s7display.displayChar('-', 8);
    _delay_ms(1000);
    while(true)
    {
        //s7display.displayChar('x', 4);
        //_delay_ms(1000);
        //s7display.displayChar('x', 3);
        //_delay_ms(1000);
        //s7display.displayChar('x', 2);
        //_delay_ms(1000);
        //s7display.displayChar('x', 1);
        //_delay_ms(1000);
    }
    
}

int main(void)
{
    //demoNumbers();
    demoNonDecodeMode();
}


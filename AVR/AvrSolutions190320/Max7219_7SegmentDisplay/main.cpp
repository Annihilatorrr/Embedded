/*
* Max7219_7SegmentDisplay.cpp
*
* Created: 3/19/2020 11:01:08 PM
* Author : VertexLT
*/

#define F_CPU 8000000UL
#include "sevensegmentdisplay.h"
#include <util/delay.h>

int main(void)
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


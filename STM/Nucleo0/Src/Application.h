/*
 * application.h
 *
 *  Created on: Apr 5, 2020
 *      Author: VertexLT
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "stm32f4xx.h"


#include "display7segmentmax7219.h"

class Application
{
	Display7SegmentMax7219 m_display;
public:
	Application()
	{

	}

    void delayMs(int delay)
    {
        int j = 0;
        for (int i=0 ; i < delay ; i++)
        {
            j++;
        }
    }

    void start(void)
    {

        m_display.shutDownOn();
        m_display.clear(8);
        m_display.setVisibleDigits(3);
        m_display.setNoDecodeAllDigits();
        for (int i = 0 ; i < 10000; ++i)
        {
        	m_display.displayInt64Number(i, true);
        	delayMs(10000);
        }

    }
};

#endif /* APPLICATION_H_ */

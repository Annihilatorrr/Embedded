/*
* PlayingWithPorts.cpp
*
* Created: 4/18/2020 2:33:45 PM
* Author : VertexLT
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/delay.h>

class PortC
{
    public:
    void setAllAsOutput()
    {
        DDRC = 0b11111111;
    }
    
    void setAllAsInput()
    {
        DDRC = 0;
    }
    
    void togglePin(char pinNumber)
    {
        PORTC = PORTC ^ (1 << pinNumber%8);
    }
    
    void setHigh(char pinNumber)
    {
        PORTC = (1 << pinNumber);
    }
    
    bool bitIsSet(char pinNumber)
    {
        return PINC & (1 << pinNumber);
    }
    
    bool bitIsClear(char pinNumber)
    {
        return !(PINC & (1 << pinNumber));
    }
    
};

class PortB
{
    public:
    void setAllAsOutput()
    {
        DDRB = 0b11111111;
    }
    
    void setAllAsInput()
    {
        DDRB = 0;
    }
    
    void togglePin(char pinNumber)
    {
        PORTB = PORTB ^ (1 << pinNumber%8);
    }
    
    void resetAllPins()
    {
        PORTB = 0;
    }
    
    void setPortMask(char mask)
    {
        PORTB = mask;
    }
};

template <typename P> class Port
{
    P m_port;
    
    public:
    void setAllAsOutput(){m_port.setAllAsOutput();}
    void setAllAsInput(){m_port.setAllAsInput();}
    void togglePin(char pinNumber){m_port.togglePin(pinNumber);}
    void setPortMask(char mask){m_port.setPortMask(mask);}
    void resetAllPins(){m_port.resetAllPins();}
    void setHigh(char pinNumber){m_port.setHigh(pinNumber);}
    bool bitIsSet(char pinNumber){m_port.bitIsSet(pinNumber);}
    bool bitIsClear(char pinNumber){m_port.bitIsClear(pinNumber);}
};

void turnAllOnOneByOneThenOffOneByOne(int pinCount)
{
    Port<PortB> port;
    port.setAllAsOutput();
    port.resetAllPins();

    char i = -1;
    char to = pinCount*2;
    for (char i = 0; i < to; ++i )
    {
        port.togglePin(i);
        _delay_ms(500);
    }
}

void turnAllOnOneByOneThenOffOneByOne2(int pinCount)
{
    Port<PortB> port;
    Port<PortC> inport;
    port.setAllAsOutput();
    port.resetAllPins();
    inport.setAllAsInput();
    inport.setHigh(2);
    char i = -1;
    char to = pinCount*2;
    for (char i = 0; i < to; ++i )
    {
        if (inport.bitIsClear(2))
        {  
          _delay_ms(1000);
        }
        port.togglePin(i);
        _delay_ms(500);
    }
}

void turnSingleOneByOneThenOffOneByOne(int pinCount)
{
    Port<PortB> port;
    port.setAllAsOutput();
    port.resetAllPins();

    char i = -1;
    char to = pinCount*2;
    for (char i = 0; i < to; ++i )
    {
        port.setPortMask(1 << i%8);
        _delay_ms(500);
    }
}

int main(void)
{
    turnAllOnOneByOneThenOffOneByOne2(8);
    //turnAllOnOneByOneThenOffOneByOne(8);
}


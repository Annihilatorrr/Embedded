/*
* PlayingWithPorts.cpp
*
* Created: 4/18/2020 2:33:45 PM
* Author : VertexLT
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

// fires at frequence of 1000000/(15625*64)=1Hz
void timer1_ini(void)
{
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    OCR1A = 15625;
    OCR1B = 7812;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    //CLK/64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    
    // Timer/Counter1 CompareA Match interrupt is enabled.
    // The corresponding interrupt (at vector $00C) is executed if a CompareA match in Timer/Counter1 occurs, i.e. when the OCF1A bit is set in the Timer/Counter Interrupt Flag Register - TIFR.
    TIMSK1 |= (1 << OCIE1A)|(1 << OCIE1B);
}

void timer0_ini(void)
{
    TCCR0A = 0;// set entire TCCR1A register to 0
    TCCR0B = 0;// same for TCCR1B
    TCNT0  = 0;//initialize counter value to 0
    
    OCR0B = 125;
    OCR0A = 250;
    
    TCCR0A = (1<<COM0A0)|(0<<COM0A1); // toggle PD6 at compare match
    TCCR0A |= (1<<COM0B1)|(1<<COM0B0);
    TCCR0A |= (1<<WGM01); // set CTC mode 
    
    TCCR0B  |=  (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(1<<CS02)|(1<<CS00);
    TIMSK0 = (1 << OCIE0A)|(1 << OCIE0B);
}

ISR (TIMER0_COMPA_vect)
{
    PORTB ^= (1 << PORTB0);
}

ISR (TIMER0_COMPB_vect)
{
    PORTB ^= (1 << PORTB1);
}

ISR (TIMER1_COMPA_vect)
{
    PORTB ^= (1 << PORTB2);
}

ISR (TIMER1_COMPB_vect)
{
    PORTB ^= (1 << PORTB3);
}

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
    char to = pinCount<<1;
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
    DDRB = 0xFF;
    DDRD = 0xFF;
    timer0_ini();
    timer1_ini();
    sei();
    while(1);
    //turnAllOnOneByOneThenOffOneByOne2(8);
    //turnAllOnOneByOneThenOffOneByOne(8);
}


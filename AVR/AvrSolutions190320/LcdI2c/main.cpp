#define F_CPU 8000000UL

#include "lcdtwi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int i = 0;
void timer_ini(void)
{
    TIMSK1 |= (1<<OCIE1A); //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)

    //OCR1A = 0b0111101000010010; //записываем в регистр число для сравнения
    OCR1A = 0b0111101000010010; //записываем в регистр число для сравнения
    TCCR1B |= (1<<CS11)|(1<<CS10);//установим делител
}

ISR (TIMER1_COMPA_vect)
{
    PORTB ^= (1 << PORTB5);
}
//----------------------------------------
int main(void)
{
    timer_ini();
    sei();
    LcdDisplay display;
    display.initPcfr(LcdDisplay::Address::PCF8574);
    display.clearlcd();
    display.setpos(0,0);
    display.str_lcd("Hello World");
    display.setpos(2,1);
    display.str_lcd("String 0");
    display.setpos(2,2);
    display.str_lcd("String 3");
    display.setpos(6,3);
    display.str_lcd("String 4");
    while(1)
    {
    }
}
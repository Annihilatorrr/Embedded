#define F_CPU 8000000UL

#include "lcdtwi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
int i = 0;


int ADCsingleREAD(uint8_t adctouse)
{
    int ADCval;

    ADMUX = adctouse;         // use #1 ADC
    ADMUX |= (1 << REFS0);    // use AVcc as the reference
    ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
    
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
    ADCSRA |= (1 << ADEN);    // Enable the ADC

    ADCSRA |= (1 << ADSC);    // Start the ADC conversion

    while(ADCSRA & (1 << ADSC));      // Thanks T, this line waits for the ADC to finish


    ADCval = ADCL;
    ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

    return ADCval;
}

// fires at frequence of 8000000/(15625*1024)=0.5Hz
void timer1_ini(void)
{
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    OCR1A = 15625<<1;
    
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12);// | (1 << CS10);
    // enable timer compare interrupt
    
    // Timer/Counter1 CompareA Match interrupt is enabled.
    // The corresponding interrupt (at vector $00C) is executed if a CompareA match in Timer/Counter1 occurs, i.e. when the OCF1A bit is set in the Timer/Counter Interrupt Flag Register - TIFR.
    TIMSK1 |= (1 << OCIE1A);
    
    
}

ISR (TIMER1_COMPA_vect)
{
    PORTB ^= (1 << PORTB4);
    PORTB ^= (1 << PORTB5);
}
void foo(int iVar)
{
    char buffer[7];
    dtostrf(iVar, 7, 0, buffer);
}
//----------------------------------------
int main(void)
{
    //DDRB = 0xFF;
    //timer1_ini();
    //sei();
    LcdDisplay display;
    display.initPcfr(LcdDisplay::Address::PCF8574);
    display.clearlcd();
    //display.setpos(0,0);
    //display.str_lcd("Hello World");
    //display.setpos(2,1);
    //display.str_lcd("String 0");
    //display.setpos(2,2);
    //display.str_lcd("String 3");
    //display.setpos(6,3);
    //display.str_lcd("String 4");
    int iVar = 24100;
    while(1)
    {
        //iVar = ADCsingleREAD(1);
        display.setpos(0,0);
        char buffer[7]{};
        itoa(iVar, buffer, 10);
        display.str_lcd(buffer);
        //_delay_ms(1000);
    }
}
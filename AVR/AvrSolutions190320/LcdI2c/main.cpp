#define F_CPU 8000000UL

#include "lcdtwi.h"
//----------------------------------------
int main(void)
{
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
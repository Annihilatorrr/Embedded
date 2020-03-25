#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcdtwi.h"
//----------------------------------------
int main(void)
{
    LcdDisplay display;
    //I2C_Init();//инициализируем TWI
    display.init();  //инициализируем дисплей
    display.clearlcd();//очистим дисплей
    display.setpos(0,0);
    display.str_lcd("Hello World!!!");
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
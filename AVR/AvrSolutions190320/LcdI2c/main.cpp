/*
 * LcdI2c.cpp
 *
 * Created: 3/25/2020 9:55:34 AM
 * Author : VertexLT
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include "lcddisplay.h"

int main(void)
{
    LcdDisplay lcd;
    I2C_Init();
    LCD_ini();
    
    clearlcd();//очистим дисплей
    setpos(0,0);
    str_lcd("a");
    //lcd.setpos(2,1);
    //lcd.str_lcd("String 2");
    //lcd.setpos(2,2);
    //lcd.str_lcd("String 3");
    //lcd.setpos(6,3);
    //lcd.str_lcd("String 4");
    
    while(1)
    {
    }
}


#include "main.h"
//----------------------------------------
int main(void)
{
    I2C_Init();//инициализируем TWI
    LCD_ini();  //инициализируем дисплей
    clearlcd();//очистим дисплей
    setpos(0,0);
    str_lcd("Hello World!!!");
    setpos(2,1);
    str_lcd("String 0");
    setpos(2,2);
    str_lcd("String 3");
    setpos(6,3);
    str_lcd("String 4");
    while(1)
    {
    }
}
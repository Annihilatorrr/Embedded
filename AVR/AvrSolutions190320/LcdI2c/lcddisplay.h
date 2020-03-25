/* 
* LcdDisplay.h
*
* Created: 3/25/2020 9:57:31 AM
* Author: VertexLT
*/


#ifndef __LCDDISPLAY_H__
#define __LCDDISPLAY_H__

#include "I2cTwi.h"
class LcdDisplay
{
    I2cTwi m_i2cTwi;
    unsigned char m_portlcd = 0; //ячейка для хранения данных порта микросхемы расширения
public:
    void init(void);
    void sendcharlcd(unsigned char c);
    void setpos(unsigned char x, unsigned y);
    void sendhalfbyte(unsigned char c);
    void sendbyte(unsigned char c, unsigned char mode);
    void clearlcd(void);
    void str_lcd (char str1[]);
    void setE1();// установка линии E в 1
    void setE0(); // установка линии E в 0
    void setRs1 (); // установка линии RS в 1
    void setRs0();// установка линии RS в 0
    void setLed(); // включение подсветки
    void setWrite(); // установка записи в память дисплея
}; //LcdDisplay

void clearlcd(void);
//----------------------------------------
void str_lcd (char str1[]);

void setpos(unsigned char x, unsigned y);
void LCD_ini();
#endif //__LCDDISPLAY_H__

/*
* LcdDisplay.cpp
*
* Created: 3/25/2020 9:57:31 AM
* Author: VertexLT
*/



#include "LcdDisplay.h"
#include "i2ctwi.h"
#include <avr/delay.h>


I2cTwi m_i2cTwi;
#define e1   m_i2cTwi.I2C_SendByteByADDR(portlcd|=0x04,0b01001110) // установка линии E в 1
#define e0   m_i2cTwi.I2C_SendByteByADDR(portlcd&=~0x04,0b01001110)  // установка линии E в 0
#define rs1    m_i2cTwi.I2C_SendByteByADDR(portlcd|=0x01,0b01001110) // установка линии RS в 1
#define rs0   m_i2cTwi.I2C_SendByteByADDR(portlcd&=~0x01,0b01001110)  // установка линии RS в 0
#define setled()    m_i2cTwi.I2C_SendByteByADDR(portlcd|=0x08,0b01001110) // включение подсветки
#define setwrite()   m_i2cTwi.I2C_SendByteByADDR(portlcd&=~0x02,0b01001110)  // установка записи в память дисплея

//----------------------------------------
unsigned char portlcd = 0; //ячейка для хранения данных порта микросхемы расширения
//----------------------------------------
//void sendhalfbyte(unsigned char c)
//{
//c<<=4;
//e1; //включаем линию Е
//_delay_us(50);
//I2C_SendByteByADDR(portlcd|c,0b01001110);
//e0; //выключаем линию Е
//_delay_us(50);
//}

void sendhalfbyte(unsigned char c)
{
    c<<=4;
    _delay_us(50);
    e1;
    _delay_us(50);
    portlcd&=0b00001111;
    //I2C_SendByteByADDR(portlcd|=c,0b01111110);
    m_i2cTwi.I2C_SendByteByADDR(portlcd|=c,0b01001110);
    _delay_us(50);
    e0;
}

//----------------------------------------
void sendbyte(unsigned char c, unsigned char mode)
{
    if (mode==0) rs0;
    else         rs1;
    unsigned char hc=0;
    hc=c>>4;
    sendhalfbyte(hc); sendhalfbyte(c);
}
//----------------------------------------
void sendcharlcd(unsigned char c)
{
    sendbyte(c,1);
}
//----------------------------------------
void setpos(unsigned char x, unsigned y)
{
    switch(y)
    {
        case 0:
        sendbyte(x|0x80,0);
        break;
        case 1:
        sendbyte((0x40+x)|0x80,0);
        break;
        case 2:
        sendbyte((0x14+x)|0x80,0);
        break;
        case 3:
        sendbyte((0x54+x)|0x80,0);
        break;
    }
}
//----------------------------------------
void LCD_ini(void)
{
    m_i2cTwi.init();
    _delay_ms(15); //Ждем 15 мс (стр 45)
    sendhalfbyte(0b00000011);
    _delay_ms(4);
    sendhalfbyte(0b00000011);
    _delay_us(100);
    sendhalfbyte(0b00000011);
    _delay_ms(1);
    sendhalfbyte(0b00000010);
    _delay_ms(1);
    sendbyte(0b00101000, 0); //4бит-режим (DL=0) и 2 линии (N=1)
    _delay_ms(1);
    sendbyte(0b00001100, 0); //включаем изображение на дисплее (D=1), курсоры никакие не включаем (C=0, B=0)
    _delay_ms(1);
    sendbyte(0b00000110, 0); //курсор (хоть он у нас и невидимый) будет двигаться влево
    _delay_ms(1);
    setled();//подсветка
    setwrite();//запись
}
//----------------------------------------
void clearlcd(void)
{
    sendbyte(0b00000001, 0);
    _delay_us(1500);
}
//----------------------------------------
void str_lcd (char str1[])
{
    wchar_t n;
    for(n=0;str1[n]!='\0';n++)
    sendcharlcd(str1[n]);
}
//----------------------------------------

//void LcdDisplay::setE1()
//{
    //m_i2cTwi.I2C_SendByteByADDR(m_portlcd|=0x04,0b01001110);
//} // установка линии E в 1
//void LcdDisplay::setE0()
//{
    //m_portlcd&=~0x04;
    //m_i2cTwi.I2C_SendByteByADDR(m_portlcd,0b01001110);
//}  // установка линии E в 0
//void LcdDisplay::setRs1 ()
//{
    //m_portlcd|=0x01;
    //m_i2cTwi.I2C_SendByteByADDR(m_portlcd,0b01001110);
//} // установка линии RS в 1
//void LcdDisplay::setRs0()
//{
    //m_portlcd&=~0x01;
    //m_i2cTwi.I2C_SendByteByADDR(m_portlcd,0b01001110);
//}  // установка линии RS в 0
//void LcdDisplay::setLed()
//{
    //m_portlcd|=0x08;
    //m_i2cTwi.I2C_SendByteByADDR(m_portlcd,0b01001110);
//} // включение подсветки
//void LcdDisplay::setWrite()
//{
    //m_portlcd&=~0x02;
    //m_i2cTwi.I2C_SendByteByADDR(m_portlcd,0b01001110);
//}  // установка записи в память дисплея
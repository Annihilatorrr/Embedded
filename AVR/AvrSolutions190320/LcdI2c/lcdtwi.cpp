#define F_CPU 8000000UL
#include <util/delay.h>
#include "lcdtwi.h"
//----------------------------------------

//----------------------------------------
//void sendhalfbyte(unsigned char c)
//{
//c<<=4;
//e1; //включаем линию Е
//_delay_us(50);
//I2C_SendByteByADDR(portlcd|c,m_pcfrAddress);
//e0; //выключаем линию Е
//_delay_us(50);
//}

void LcdDisplay::setLed()
{
    m_i2cTwi.sendByteByADDR(m_portLcd|=0x08,m_pcfrAddress);
}

void LcdDisplay::setWrite()
{
    m_i2cTwi.sendByteByADDR(m_portLcd&=~0x02,m_pcfrAddress);
}

void LcdDisplay::setRs0()
{
    m_i2cTwi.sendByteByADDR(m_portLcd&=~0x01,m_pcfrAddress);
}

void LcdDisplay::setRs1()
{
    m_i2cTwi.sendByteByADDR(m_portLcd|=0x01,m_pcfrAddress);
}

void LcdDisplay::setE1()
{
    m_i2cTwi.sendByteByADDR(m_portLcd|=0x04,m_pcfrAddress);
}

void LcdDisplay::setE0()
{
    m_i2cTwi.sendByteByADDR(m_portLcd&=~0x04,m_pcfrAddress);
}

void LcdDisplay::sendhalfbyte(unsigned char c)
{
    c<<=4;
    _delay_us(50);
    setE1();
    _delay_us(50);
    m_portLcd&=0b00001111;
    //I2C_SendByteByADDR(m_portLcd|=c,0b01111110);
    m_i2cTwi.sendByteByADDR(m_portLcd|=c,m_pcfrAddress);
    _delay_us(50);
    setE0();
}

//----------------------------------------
void LcdDisplay::sendbyte(unsigned char c, unsigned char mode)
{
    if (mode==0) setRs0();
    else         setRs1();
    unsigned char hc=0;
    hc=c>>4;
    sendhalfbyte(hc); sendhalfbyte(c);
}
//----------------------------------------
void LcdDisplay::sendcharlcd(unsigned char c)
{
    sendbyte(c,1);
}
//----------------------------------------
void LcdDisplay::setpos(unsigned char x, unsigned y)
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
void LcdDisplay::initPcfr(Address pcfrAddress)
{
    m_pcfrAddress = (unsigned char)pcfrAddress;
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
    setLed();//подсветка
    setWrite();//запись
}
//----------------------------------------
void LcdDisplay::clearlcd(void)
{
    sendbyte(0b00000001, 0);
    _delay_us(1500);
}


//----------------------------------------
void LcdDisplay::str_lcd (char str1[])
{
    wchar_t n;
    for(n=0;str1[n]!='\0';n++)
    sendcharlcd(str1[n]);
}
//----------------------------------------
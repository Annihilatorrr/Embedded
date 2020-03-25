#ifndef LCDTWI_H_
#define LCDTWI_H_

#include "twi.h"

class LcdDisplay
{
    
    I2cTwix m_i2cTwi;
    
    public:
    
    unsigned char portlcd = 0; //ячейка для хранения данных порта микросхемы расширения
//----------------------------------------
    void init(void);
    void setpos(unsigned char x, unsigned y);
    void str_lcd (char str1[]);
    void clearlcd(void);
    void sendcharlcd(unsigned char c);

    void setE1();
    void setE0();
    void setRs1();
    void setRs0();
    void setLed();
    void setWrite();
    
    void sendhalfbyte(unsigned char c);
    void sendbyte(unsigned char c, unsigned char mode);
};
//----------------------------------------


#endif /* LCDTWI_H_ */
/*
* I2cTwi.h
*
* Created: 3/25/2020 9:57:17 AM
* Author: VertexLT
*/


#ifndef __I2CTWI_H__
#define __I2CTWI_H__


class I2cTwi
{
    //variables
    public:
    void init (void);
    
    void I2C_StartCondition(void);
    void I2C_StopCondition(void);

    void I2C_SendByte(unsigned char c);

    void I2C_SendByteByADDR(unsigned char c,unsigned char addr);
    
    unsigned char I2C_ReadByte(void);

    unsigned char ReadLastByte(void);
    

    
}; //I2cTwi

#endif //__I2CTWI_H__

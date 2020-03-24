/*
 * lcd_i2c.h
 *
 * Created: 3/24/2020 3:03:29 PM
 *  Author: VertexLT
 */ 


#ifndef LCD_I2C_H_
#define LCD_I2C_H_


//-----Proto-Type-------//
void lcd_init();			//----LCD Initializing Function
void toggle();				//----Latching function of LCD
void lcd_cmd_hf(char v1);   //----Function to send half byte command to LCD
void lcd_cmd(char v2);		//----Function to send Command to LCD
void lcd_dwr(char v3);		//----Function to send data to LCD
void lcd_msg(char *c);		//----Function to Send String to LCD
void delay(int ms);			//----Delay function
void lcd_lef_sh();			//----Left Shifting Function
void lcd_rig_sh();			//----Right Shifting Function
//----------------------//


#endif /* LCD_I2C_H_ */
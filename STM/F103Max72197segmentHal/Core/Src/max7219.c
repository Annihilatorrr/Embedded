#include "max7219.h"

extern SPI_HandleTypeDef hspi2;
char dg=8;

#define cs_set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define cs_reset() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

void Send_7219 (uint8_t rg, uint8_t dt)
{
  uint16_t dtt = (uint16_t)rg << 8 | dt;
	cs_set();
	HAL_SPI_Transmit (&hspi2, (uint8_t*)&dtt, 1, 5000);
	cs_reset();
}
//------------------------------------------------------
void Clear_7219 (void)
{
	uint8_t i=dg;
	do
	{
		Send_7219(i,0xF);//������ �������
	} while (--i);
}
//------------------------------------------------------
void Number_7219 (volatile long n)
{
	uint8_t ng=0;//���������� ��� ������
	if(n<0)
	{
		ng=1;
		n*=-1;
	}
	uint8_t i=0;
	do
	{
		Send_7219(++i,n%10);//������ �����
		n/=10;
	} while(n);
	if(ng)
	{
		Send_7219(i+1,0x0A);//������ -
	}
}
//-------------------------------------------------------
void NumberL_7219 (volatile int n) //����� � ����� �������
{
  uint8_t ng=0;//���������� ��� ������
  if(n<0)
  {
    ng=1;
    n*=-1;
  }
  uint8_t i=4;
  if(n<1000) Send_7219(8,0xF);//символ пустоты
  if(n<100) Send_7219(7,0xF);//символ пустоты
  if(n<10) Send_7219(6,0xF);//символ пустоты
  do
  {
    Send_7219(++i,n%10);//������ �����
    n/=10;
  } while(n);
  if(ng)
  {
    Send_7219(i+1,0x0A);//������ -
  }
}
//-------------------------------------------------------
void NumberR_7219 (volatile int n)
{
	uint8_t ng=0;//переменная для минуса
	if(n<0)
	{
		ng=1;
		n*=-1;
	}
	uint8_t i=0;
  if(n<1000) Send_7219(4,0xF);//символ пустоты
  if(n<100) Send_7219(3,0xF);//символ пустоты
  if(n<10) Send_7219(2,0xF);//символ пустоты
	do
	{
		Send_7219(++i,n%10);//символ цифры
		n/=10;
	} while(n);
	if(ng)
	{
		Send_7219(i+1,0x0A);//символ -
	}
}
//-------------------------------------------------------
void NumberF_7219 (float f) //����� � ����� ������� c ���������� ������
{
  int n = (int)(f * 10);
  uint8_t ng=0;//���������� ��� ������
  if(n<0)
  {
    ng=1;
    n*=-1;
  }
  int m = n;
  uint8_t i=0;
  do
  {
    if(i==1) Send_7219(++i,(n%10) | 0x80);//������ ����� � �����
    else Send_7219(++i,n%10);//������ �����
    n/=10;
  } while(n);
  if(ng)
  {
    if(m<10)
    {
      Send_7219(i+1,0x80);//���� � ������
      Send_7219(i+2,0x0A);//������ -
    }
    else Send_7219(i+1,0x0A);//������ -
  }
  else
  {
    if((m<10)&&(m!=0))
    {
      Send_7219(i+1,0x80);//���� � ������
    }
  }
}
//-------------------------------------------------------
void NumberLF_7219 (float f) //����� � ����� ������� c ���������� ������
{
  int n = (int)(f * 10);
  uint8_t ng=0;//���������� ��� ������
  if(n<0)
  {
    ng=1;
    n*=-1;
  }
  int m = n;
  uint8_t i=4;
  do
  {
    if(i==5) Send_7219(++i,(n%10) | 0x80);//������ ����� � �����
    else Send_7219(++i,n%10);//������ �����
    n/=10;
  } while(n);
  if(ng)
  {
    if(m<10)
    {
      Send_7219(i+1,0x80);//���� � ������
      Send_7219(i+2,0x0A);//������ -
    }
    else Send_7219(i+1,0x0A);//������ -
  }
  else
  {
    if((m<10)&&(m!=0))
    {
      Send_7219(i+1,0x80);//���� � ������
    }
  }
}
//-------------------------------------------------------
void Init_7219 (void)
{
  Send_7219(0x0F,0x00); //отключим тестовый режим
  Send_7219(0x09,0xFF);//������� ����� �������������
  Send_7219(0x0B,dg-1);//���-�� ������������ ��������
  Send_7219(0x0A,0x06);//������������� ��������
  Send_7219(0x0C,0x01);//������� ���������
  Clear_7219();
}

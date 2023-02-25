// STM32F103 SysTick_Config(72000) + IWDG

#include "stm32f1xx.h"
#include "delay.h"

uint32_t 	TimingDelay;

// ������ ���������� �� ��������� �������� ��� ������� 72 MHz
void initDelay(void) {
	SysTick_Config(72000);
}
// ���������� ���������� ���������� �������, ���������� ������ 1 ms
void SysTick_Handler() {
	if (TimingDelay != 0) TimingDelay--;
}

// ������� ��������� �������� � ������������
void Delay_ms(uint32_t nTime) {
	IWDG_Feed();								// Clear IWDG <3s
	TimingDelay = nTime;
	while (TimingDelay != 0);
}

// ��������� ������������ ����������� �������. ��������� �� 3s: prer=4, rlr=10100
void initIWDG(uint8_t prer, uint16_t rlr) {
	IWDG->KR = 0X5555;							// ���� ��� ������� � �������
  	IWDG->PR = prer;  							// ���������� ������������
  	IWDG->RLR = rlr;  							// ��������� ������� ������������, ���� ���� ���� �� ����� ��������
	IWDG->KR = 0XAAAA;							// ������������
  	IWDG->KR = 0XCCCC;							// ������ � ������
}

// Clear IWDG <3s
void IWDG_Feed(void) {IWDG->KR=0XAAAA;}

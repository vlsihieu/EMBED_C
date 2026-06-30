/*
 * capture.c
 *
 *  Created on: Jun 27, 2026
 *      Author: quocvi
 */
#include "main.h"
#include "capture.h"
#define TIM2_ADDR_BASE 0x40000000
#define GPIOA_ADDR_BASE 0x40010800
void capture_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIOA->CRL &= ~(0b1111 << 0);  // Clr pin 0 GPIOA
	GPIOA->CRL |= (0b1000 << 0);  //
	GPIOA->CRL &= ~(0b1111 << 4);  // Clr pin 1 GPIOA
	GPIOA->CRL |= (0b1000 << 4);
	GPIOA->CRL &= ~(0b1111 << 8);  // Clr pin 2
	GPIOA->CRL |= (0b1000 << 8);
	GPIOA->CRL &= ~(0b1111 << 12);  // Clr pin 3
	GPIOA->CRL |= (0b1000 << 12);
	__HAL_RCC_TIM2_CLK_ENABLE();
	uint16_t* TIM2_CNT = (uint16_t*)(TIM2_ADDR_BASE + 0x24);
	uint32_t* TIM2_ARR = (uint32_t*)(TIM2_ADDR_BASE + 0x2c);
	uint32_t* TIM2_PSC = (uint32_t*)(TIM2_ADDR_BASE + 0x28);
	*TIM2_ARR = 0xFFFF;
	*TIM2_PSC = 800-1 ;
	uint16_t* TIM2_CCMR1 = (uint16_t*)(TIM2_ADDR_BASE + 0x18);
	uint16_t* TIM2_CCMR2 = (uint16_t*)(TIM2_ADDR_BASE + 0x1C);
	*TIM2_CCMR1 &= ~(0xFFu << 0);  // Xóa toàn bộ cấu hình CC1 (CC1S, IC1PSC, IC1F)
	*TIM2_CCMR1 |= ((0b01 << 0)|(0b01<<8)); // Set chế độ Input mode cho CH1, CH2
	*TIM2_CCMR2 &= ~(0xFFu << 0);  // Xóa toàn bộ cấu hình CC2
	*TIM2_CCMR2 |= ((0b01 << 0)|(0b01<<8)); // Set chế độ Input mode cho CH3, CH4
	uint16_t* TIM2_CCER = (uint16_t*)(TIM2_ADDR_BASE +  0x20);
	*TIM2_CCER &= ~(1 << 1); // clear rising để làm trigger capture
	*TIM2_CCER |=((1<<5)|(1 << 9) |(1 << 13)); // Input capture falling egde
	*TIM2_CCER |= ((1 << 0)|(1 << 4) |(1 << 8) |(1 << 12)); // Enable channel 1,2,3,4
	*TIM2_CNT = 0;
	uint16_t* TIM2_SMCR = (uint16_t*)(TIM2_ADDR_BASE +  0x08);
	*TIM2_SMCR |= (0b100 << 0) | (0b101 << 4 ); // Reset CNT khi có cạnh lên
	uint16_t* TIM2_CR1   = (uint16_t*)(TIM2_ADDR_BASE + 0x00);
	*TIM2_CR1 |= (1u << 0);   // CEN = 1: cho phép bộ đếm chạy
}
    uint32_t capture_read()
    {
    	uint32_t* TIM2_CCR1 = (uint32_t*)(TIM2_ADDR_BASE + 0x34);
    	return *TIM2_CCR1;

    }
    uint32_t capture_pulse_width1()
    {
    	uint32_t* TIM2_CCR2 = (uint32_t*)(TIM2_ADDR_BASE + 0x38);
    	return *TIM2_CCR2;
    }
    uint32_t capture_pulse_width2()
    {
    	uint32_t* TIM2_CCR3 = (uint32_t*)(TIM2_ADDR_BASE + 0x3C);
    	return *TIM2_CCR3;
    }
    uint32_t capture_pulse_width3()
    {
    	uint32_t* TIM2_CCR4 = (uint32_t*)(TIM2_ADDR_BASE + 0x40);
    	return *TIM2_CCR4;
    }
    uint32_t capture_pulse_width4()
    {
        return 0;
    }

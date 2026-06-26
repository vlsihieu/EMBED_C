/*
 * PWM.c
 *
 *  Created on: Jun 26, 2026
 *      Author: DUONG DINH TU
 */
#include "main.h"
#include "PWM.h"

void PWM_init(void)
{
	/* 1. Cấu hình PB6,7,8,9 là Output AF Push-Pull (50MHz) */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	uint32_t *CRL = (uint32_t*)0x40010c00;
	uint32_t *CRH = (uint32_t*)0x40010c04;

	// mỗi pin chiếm 4 bit: CNF[1:0]:MODE[1:0]; 0b1011 = AF Push-Pull, 50MHz
	*CRL &= ~(0xF << 24); *CRL |= (0xB<<24); // PB6  ; CNF(10), Mode(11) => 1011 = B.
	*CRL &= ~(0xF << 28); *CRL |= (0xB<<28); // PB7
	*CRH &= ~(0xF << 0); *CRH |= (0xB<<0); //PB8
	*CRH &= ~(0xF << 4); *CRH |= (0xB<<4); //PB9

	/* 2. Cấu hình TIM4 chạy PWM mode 1, 4 channel */
	__HAL_RCC_TIM4_CLK_ENABLE();
	uint32_t *PSC = (uint32_t*)0x40000828;
	uint32_t *ARR = (uint32_t*)0x4000082c;
	uint32_t *CCR1 = (uint32_t*)0x40000834;
	uint32_t *CCR2 = (uint32_t*)0x40000838;
	uint32_t *CCR3 = (uint32_t*)0x4000083c;
	uint32_t *CCR4 = (uint32_t*)0x40000840;
	uint32_t *CCMR1_Output = (uint32_t*)0x40000818;  // quản lý CH1, CH2
	uint32_t *CCMR2_Output = (uint32_t*)0x4000081c;  // quản lý CH3, CH4
	uint32_t *CCER = (uint32_t*)0x40000820;
	uint32_t *CR1 = (uint32_t*)0x40000800;

	 // f_PWM = TimerClock / (PSC+1) / (ARR+1) = 8.000.000 / 8000 / 1000 = 1Hz
	*PSC = 8000-1;
	*ARR = 1000;

	// Duty cycle = CCRx/ ARR
	*CCR1 = 250; // 25%
	*CCR2 = 500; // 50%
	*CCR3 = 750; // 750%
	*CCR4 = 1000; // 100%

	// CCMR1: OC1M (bit4-6) và OC2M (bit12-14) = 110 -> PWM mode 1
	*CCMR1_Output &= ~((0x3<<0)|(0x7<<4)|(0x3<<8)|(0x7<<12));
	*CCMR1_Output |=  ((0b110 << 4) | (0b110 << 12));

	// CCMR2: OC3M (bit4-6) và OC4M (bit12-14) = 110 -> PWM mode 1
	*CCMR2_Output &= ~((0b11 << 0) | (0b111 << 4) | (0b11 << 8) | (0b111 << 12));
	*CCMR2_Output |=  ((0b110 << 4) | (0b110 << 12));

	// Enable output 4 channel: CC1E, CC2E, CC3E, CC4E
	*CCER |= ((1 << 0) | (1 << 4) | (1 << 8) | (1 << 12));

    // Enable Counter (CEN)
    *CR1 |= (1 << 0);

}


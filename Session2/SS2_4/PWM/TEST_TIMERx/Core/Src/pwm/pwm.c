/*
 * t.c
 *
 *  Created on: Jun 24, 2026
 *      Author: Admin
 */
#include "main.h"
#include "pwm.h"
/**
 * @brief Generate pwm with frequency = 1Hz, Duty = 50%
 * @param None
 * @retval None
 */
void PWM_init()
{
	uint32_t *GPIOB_CRL = (uint32_t*)0x40010C00;
    uint32_t *GPIOB_CRH = (uint32_t*)0x40010C04;

    // Cấu hình chân TIM4 CH1 (PB6) và CH2 (PB7) trên thanh ghi CRL
    *GPIOB_CRL &= ~((0xF << 24) | (0xF << 28));
    *GPIOB_CRL |=  ((0b1011 << 24) | (0b1011 << 28));

    // Cấu hình chân TIM4 CH3 (PB8) và CH4 (PB9) trên thanh ghi CRH
    *GPIOB_CRH &= ~((0xF << 0) | (0xF << 4));
    *GPIOB_CRH |=  ((0b1011 << 0) | (0b1011 << 4));



    uint32_t *PSC = (uint32_t*)0x40000828;
    uint32_t *ARR = (uint32_t*)0x4000082c;
    uint32_t *CCR1 = (uint32_t*)0x40000834;
    uint32_t *CCR2 = (uint32_t*)0x40000838;
    uint32_t *CCR3 = (uint32_t*)0x4000083C;
    uint32_t *CCR4 = (uint32_t*)0x40000840;
    uint32_t *CCMR1_Output = (uint32_t*)0x40000818; // Quản lý CH1, CH2
    uint32_t *CCMR2_Output = (uint32_t*)0x4000081C; // Quản lý CH3, CH4
    uint32_t *CCER = (uint32_t*)0x40000820;
    uint32_t *CR1 = (uint32_t*)0x40000800;

    // 72MHz / 72000 = 1kHz -> 1 tick = 1ms
    *PSC = 72000 - 1;
    // Period = 1000 tick = 1000ms = 1Hz
    *ARR = 999;

    // install pulse
    *CCR1 = 250;
    *CCR2 = 500;
    *CCR3 = 750;
    *CCR4 = 1000;

    // Select Channel 1, 2 work in compare --> Mode PWM1
    *CCMR1_Output &= ~((0b11 << 0) | (0b111 << 4) | (0b11 << 8) | (0b111 << 12));
    *CCMR1_Output |=  ((0b110 << 4) | (0b110 << 12));

    // Select Channel 3, 4 work in compare --> Mode PWM1
    *CCMR2_Output &= ~((0b11 << 0) | (0b111 << 4) | (0b11 << 8) | (0b111 << 12));
    *CCMR2_Output |=  ((0b110 << 4) | (0b110 << 12));

    // Enable output cho cả 4 kênh (CC1E, CC2E, CC3E, CC4E)
    *CCER |= (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12);

    // Start timer (CEN = 1)
    *CR1 |= (1 << 0);
}

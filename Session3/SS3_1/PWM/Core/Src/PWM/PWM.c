/*
 * PWM.c
 *
 *  Created on: Jun 27, 2026
 *      Author: phitruong
 */

#include "main.h"
#include "PWM.h"

void PWM_init()
{
    /* ---- 1. Enable clock GPIOA và GPIOB ---- */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    uint32_t *GPIOA_CRL = (uint32_t*)0x40010800;  // GPIOA CRL
    uint32_t *GPIOB_CRL = (uint32_t*)0x40010C00;  // GPIOB CRL

    /* PA6 – TIM3 CH1: CRL bit[27:24] */
    *GPIOA_CRL &= ~(0xF << 24);
    *GPIOA_CRL |=  (0xB << 24);  // AF Push-Pull 50MHz

    /* PA7 – TIM3 CH2: CRL bit[31:28] */
    *GPIOA_CRL &= ~(0xF << 28);
    *GPIOA_CRL |=  (0xB << 28);

    /* PB0 – TIM3 CH3: CRL bit[3:0] */
    *GPIOB_CRL &= ~(0xF << 0);
    *GPIOB_CRL |=  (0xB << 0);

    /* PB1 – TIM3 CH4: CRL bit[7:4] */
    *GPIOB_CRL &= ~(0xF << 4);
    *GPIOB_CRL |=  (0xB << 4);

    /* ---- 2. Enable clock TIM3 ---- */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 Base: 0x40000400 */
    uint32_t *CR1          = (uint32_t*)0x40000400;
    uint32_t *EGR          = (uint32_t*)0x40000414;  // Event Generation
    uint32_t *CCMR1_Output = (uint32_t*)0x40000418;  // CH1, CH2
    uint32_t *CCMR2_Output = (uint32_t*)0x4000041C;  // CH3, CH4
    uint32_t *CCER         = (uint32_t*)0x40000420;
    uint32_t *PSC          = (uint32_t*)0x40000428;
    uint32_t *ARR          = (uint32_t*)0x4000042C;
    uint32_t *CCR1         = (uint32_t*)0x40000434;
    uint32_t *CCR2         = (uint32_t*)0x40000438;
    uint32_t *CCR3         = (uint32_t*)0x4000043C;
    uint32_t *CCR4         = (uint32_t*)0x40000440;

    /* ---- 3. Prescaler & Auto-reload ---- */
    *PSC = 8000 - 1;    // 8MHz / 8 = 1MHz
    *ARR = 1000 ;  // 1MHz / 1000 = 1kHz

    /* ---- 4. Update Generation (load PSC, ARR vào shadow register) ---- */
    *EGR |= (1 << 0);

    /* ---- 5. Duty cycle ---- */
    *CCR1 = 250;   // 25%
    *CCR2 = 500;   // 50%
    *CCR3 = 750;   // 75%
    *CCR4 = 1000;   // 100%

    /* ---- 6. PWM Mode 1 + Preload enable ---- */
    // 0x68 = OC1M=110(PWM1), OC1PE=1, CC1S=00
    *CCMR1_Output &= ~((0xFF << 0) | (0xFF << 8));
    *CCMR1_Output |=  ((0x68 << 0) | (0x68 << 8));

    *CCMR2_Output &= ~((0xFF << 0) | (0xFF << 8));
    *CCMR2_Output |=  ((0x68 << 0) | (0x68 << 8));

    /* ---- 7. Enable output CH1, CH2, CH3, CH4 ---- */
    *CCER |= ((1<<0) | (1<<4) | (1<<8) | (1<<12));

    /* ---- 8. Enable counter ---- */
    *CR1 |= (1 << 0);
}

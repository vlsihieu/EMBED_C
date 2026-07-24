/*
 * IC.c
 *
 *  Created on: Jul 10, 2026
 *      Author: PC
 */

#include <a timer.h>

void capture_init(TIM_TypeDef *TIMx){
    if (TIMx == TIM1) {
        RCC->APB2ENR |= (1 << 11);   // TIM1EN
    }
    else if (TIMx == TIM2) {
        RCC->APB1ENR |= (1 << 0);    // TIM2EN
    }
    else if (TIMx == TIM3) {
        RCC->APB1ENR |= (1 << 1);    // TIM3EN
    }
    else if (TIMx == TIM4) {
        RCC->APB1ENR |= (1 << 2);    // TIM4EN
    }
    else {
        return;
    }


    TIMx->PSC = 71;           // Không prescaler để đo chính xác
    TIMx->ARR = 0xFFFF;      // Full 16-bit

            // Input Capture config
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |= TIM_CCMR1_CC1S_0;     // CC1S = 01 : map to TI1

            // Rising edge + Enable capture
    TIMx->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
    TIMx->CCER |= TIM_CCER_CC1E;

    TIMx->DIER |= TIM_DIER_CC1IE;        // Enable interrupt CC1

    TIMx->CR1 |= TIM_CR1_CEN;            // Bật timer
}


/*
 * PWM.c
 *
 *  Created on: Aug 1, 2026
 *      Author: PC
 */

#include <PWM.h>


void TIM2_init(){
	RCC->APB1ENR |= (1<<0);

	TIM2->CR1 = 0;

	TIM2->ARR = 19999;
	TIM2->PSC = 71;

	TIM2->CR1 =(1<<7);
	TIM2->CCMR1 = 0;
	TIM2->CCMR1 = (0X06<<4)|(0x01<<3);

	TIM2->CCER = 0;
	TIM2->CCER |=(1<<0);

	TIM2->EGR = (1<<0);
	TIM2->CR1 = (1<<0);
}

void angle_to_CCR(uint16_t angle){
    if(angle > 180) angle = 180;
    uint16_t pulse_us = 1000 + (angle * 1000UL) / 180;
    TIM2->CCR1 = pulse_us; // vì 1 tick = 1us nên CCR = pulse_us luôn
}






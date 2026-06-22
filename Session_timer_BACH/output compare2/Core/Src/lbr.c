/*
 * lbr.c
 *
 *  Created on: Jun 22, 2026
 *      Author: PC
 */
#include "lbr.h"

void GPIO_init(GPIO_TypeDef *GPIOx,uint8_t pin, uint8_t mode, uint8_t cnf, uint8_t pull ){
	if(GPIOx == GPIOA){
		RCC->APB2ENR|= 1<<2;
	}
	else if(GPIOx == GPIOB){
		RCC->APB2ENR|= 1<<3;
	}
	else if(GPIOx == GPIOC){
		RCC->APB2ENR|= 1<<4;
	}
	else if(GPIOx == GPIOD){
		RCC->APB2ENR|= 1<<5;
	}

	//config pin
    if(pin < 8){
	   GPIOx->CRL &= ~(0xf << (pin *4));
	   GPIOx->CRL |= ((mode|(cnf<<2))<<(pin*4));
    }
    else{
    	GPIOx->CRH &= ~(0xf << ((pin-8)*4));
    	GPIOx->CRH |= ((mode | (cnf<<2)) << ((pin-8)*4));
    }

    if (pull == PULL_UP){
    	GPIOx->ODR |= (0x01<<(pin));
    }
    else if(pull == PULL_DOWN){
    	GPIOx->ODR &= ~(0x01<<pin);
    }



}


void GPIO_write(GPIO_TypeDef *GPIOx,uint8_t pin_number, uint8_t state){
	if(state == 1){
		GPIOx->BSRR = 1<<pin_number;
	}
	else if(state ==0){
		GPIOx->BSRR = 1<<(pin_number+16);
    }
}


uint8_t GPIO_read(GPIO_TypeDef *GPIOx, uint8_t pin_number){
	return ((GPIOx->IDR & (1<< pin_number)) !=0) ? 1 : 0;
}

void TIM2_PWM_Init(uint8_t channel) {
    RCC->APB1ENR |= (1<<0);

    TIM2->CR1 &= ~(1<<0);  // tắt timer trước khi config

    TIM2->PSC = 71;
    TIM2->ARR = 999;

    // CR1: center-aligned + ARPE
    TIM2->CR1 &= ~(0b11<<5);
    TIM2->CR1 |=  (0b10<<5);
    TIM2->CR1 |=  (1<<7);

    if (channel == CH1) {
        TIM2->CCMR1 &= ~(0xFF << 0);   // clear toàn bộ bits CH1
        TIM2->CCMR1 |=  (6 << 4);      // OC1M = PWM mode 1
        TIM2->CCMR1 |=  (1 << 3);      // OC1PE = 1
        TIM2->CCER  &= ~(0x3 << 0);
        TIM2->CCER  |=  (1 << 0);      // CC1E
    }
    else if (channel == CH2) {
        TIM2->CCMR1 &= ~(0xFF << 8);   // clear toàn bộ bits CH2
        TIM2->CCMR1 |=  (6 << 12);     // OC2M = PWM mode 1
        TIM2->CCMR1 |=  (1 << 11);     // OC2PE = 1
        TIM2->CCER  &= ~(0x3 << 4);
        TIM2->CCER  |=  (1 << 4);      // CC2E
    }
    else if (channel == CH3) {
        TIM2->CCMR2 &= ~(0xFF << 0);   // clear toàn bộ bits CH3
        TIM2->CCMR2 |=  (6 << 4);      // OC3M = PWM mode 1
        TIM2->CCMR2 |=  (1 << 3);      // OC3PE = 1
        TIM2->CCER  &= ~(0x3 << 8);
        TIM2->CCER  |=  (1 << 8);      // CC3E
    }
    else if (channel == CH4) {
        TIM2->CCMR2 &= ~(0xFF << 8);   // clear toàn bộ bits CH4
        TIM2->CCMR2 |=  (6 << 12);     // OC4M = PWM mode 1
        TIM2->CCMR2 |=  (1 << 11);     // OC4PE = 1
        TIM2->CCER  &= ~(0x3 << 12);
        TIM2->CCER  |=  (1 << 12);     // CC4E
    }

    TIM2->EGR |= (1<<0);   // update event — load PSC, ARR, CCR
    TIM2->CR1 |= (1<<0);   // CEN = start
}

void TIM2_PWM_SetDuty(uint8_t channel, uint16_t value) {
    if      (channel == CH1) TIM2->CCR1 = value;
    else if (channel == CH2) TIM2->CCR2 = value;
    else if (channel == CH3) TIM2->CCR3 = value;
    else if (channel == CH4) TIM2->CCR4 = value;
}

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

    TIM2->PSC = 63;
    TIM2->ARR = 999;


    TIM2->CR1 &= ~(0b11<<5);
    TIM2->CR1 |=  (0b10<<5);
    TIM2->CR1 |=  (1<<7);

    if (channel == CH1) {
        TIM2->CCMR1 &= ~(0xFF << 0);
        TIM2->CCMR1 |=  (6 << 4);
        TIM2->CCMR1 |=  (1 << 3);
        TIM2->CCER  &= ~(0x3 << 0);
        TIM2->CCER  |=  (1 << 0);
    }
    else if (channel == CH2) {
        TIM2->CCMR1 &= ~(0xFF << 8);
        TIM2->CCMR1 |=  (6 << 12);
        TIM2->CCMR1 |=  (1 << 11);
        TIM2->CCER  &= ~(0x3 << 4);
        TIM2->CCER  |=  (1 << 4);
    }
    else if (channel == CH3) {
        TIM2->CCMR2 &= ~(0xFF << 0);
        TIM2->CCMR2 |=  (6 << 4);
        TIM2->CCMR2 |=  (1 << 3);
        TIM2->CCER  &= ~(0x3 << 8);
        TIM2->CCER  |=  (1 << 8);
    }
    else if (channel == CH4) {
        TIM2->CCMR2 &= ~(0xFF << 8);
        TIM2->CCMR2 |=  (6 << 12);
        TIM2->CCMR2 |=  (1 << 11);
        TIM2->CCER  &= ~(0x3 << 12);
        TIM2->CCER  |=  (1 << 12);
    }

    TIM2->EGR |= (1<<0);
    TIM2->CR1 |= (1<<0);
}

void TIM2_PWM_SetDuty(uint8_t channel, uint16_t value) {
    if      (channel == CH1) TIM2->CCR1 = value;
    else if (channel == CH2) TIM2->CCR2 = value;
    else if (channel == CH3) TIM2->CCR3 = value;
    else if (channel == CH4) TIM2->CCR4 = value;
}


void TIM3_init(){
    RCC->APB1ENR |= (1<<0);

    TIM3->CR1 &= ~(1<<0);
    TIM3->PSC = 71;
    TIM3->ARR = 999;

    TIM3->CR1 &= ~(0x01<<4);
    TIM3->CR1 |= (0x01<<4);
    TIM3->CR1 &= ~(0x01<<7);
    TIM3->CR1 |= (0x01<<7);
    TIM3->EGR &= ~(0x01<<1);
    TIM3->EGR |= (0x01<<1);

    TIM3->CR1 |= (0X01<<0);
}

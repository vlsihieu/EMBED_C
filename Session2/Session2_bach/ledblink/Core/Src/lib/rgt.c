/*
 * rgt.c
 *
 *  Created on: May 28, 2026
 *      Author: PC
 */
#include "rgt.h"

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

    //config pull
    if(pull==PULL_UP){
    	GPIOx->ODR |= (1<<pin);
    }
    else if(pull==PULL_DOWN){
    	GPIOx->ODR &= ~(1<<pin);
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


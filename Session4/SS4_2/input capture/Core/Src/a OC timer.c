/*
 * OC timer.c
 *
 *  Created on: Jul 10, 2026
 *      Author: PC
 */
#include <a timer.h>

static uint8_t g_pwm_initialized[5] = {0};


// khởi tạo timer
PWM_Status_t TIM2_init(PWM_channel selection){

	RCC->APB1ENR |= (1<<0);

	TIM2->ARR = 999;
	TIM2->PSC = 71;

	TIM2->CR1 &= ~(1<<7);
	TIM2->CR1 |= (1<<7); //buffer

    if(selection == CH1){
    	TIM2->CCMR1 &= ~(0xff<<0);
    	TIM2->CCMR1 |= (6<<4);
    	TIM2->CCER &= ~(1<<0);
    	TIM2->CCER |= (1<<0);
    }
    else if(selection == CH2){
    	TIM2->CCMR1 &= ~(0xff<<8);
    	TIM2->CCMR1 |= (6<<12);
    	TIM2->CCER &= ~(1<<4);
    	TIM2->CCER |= (1<<4);
    }
    else if(selection == CH3){
    	TIM2->CCMR2 &= ~(0xff<<0);
    	TIM2->CCMR2 |= (6<<4);
    	TIM2->CCER &= ~(1<<8);
    	TIM2->CCER |= (1<<8);
    }
    else if(selection == CH4){
    	TIM2->CCMR2 &= ~(0xff<<8);
    	TIM2->CCMR2 |= (6<<12);
    	TIM2->CCER &= ~(1<<12);
    	TIM2->CCER |= (1<<12);
    }


    TIM2->EGR |= (1<<0);
    TIM2->CR1 |= (1<<0);   // Bật TIM2


    g_pwm_initialized[selection] = 1;
    return PWM_OK;




}


PWM_Status_t TIM2_setduty(PWM_channel selection, uint16_t pwm){

	if(g_pwm_initialized[selection] != 1){
		return PWM_ERR_NOT_INITIALIZED;
	}

	if(pwm > (TIM2->ARR)){
		return PWM_ERR_DUTY_OUT_OF_RANGE;
	}

	switch(selection){
    case CH1: TIM2->CCR1 = pwm; break;
    case CH2: TIM2->CCR2 = pwm; break;
    case CH3: TIM2->CCR3 = pwm; break;
    case CH4: TIM2->CCR4 = pwm; break;
	}
	return PWM_OK;
}



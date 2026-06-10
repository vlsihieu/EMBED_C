/*
 * cnt.c
 *
 *  Created on: Jun 8, 2026
 *      Author: PC
 */

#include "cnt.h"
#include <stdint.h>

void TIMx_init(TIM_TypeDef *TIMx){
	if(TIMx){
		RCC->APB1ENR |= 1<<0;
		TIMx->ARR = 999;
		TIMx->PSC = 7;

		TIMx->CR1 |= (1<<0);
		TIMx->CR1 |= (0<<4);
		TIMx->EGR |= (1<<0);

	}

}




void delayms(TIM_TypeDef *TIMx,uint16_t msecond) {
    uint16_t count = 0;

    while (count < msecond) {
        if (TIMx->SR & TIM_SR_UIF) {       // chờ timer tràn (= 1ms)
            TIMx->SR &= ~TIM_SR_UIF;       // clear cờ
            count++;
        }
    }
}



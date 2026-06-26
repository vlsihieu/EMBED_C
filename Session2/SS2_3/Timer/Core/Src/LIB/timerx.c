/*
 * timerx.c
 *
 *  Created on: Jun 14, 2026
 *      Author: vdtt0
 */
#include "timerx.h"


void Timer2_4_IT_Config(TIM_TypeDef *TIMx)
{
	// Clock enable
	if(TIMx == TIM2)
	{
		RCC->APB1ENR |= 1 << 0;
	}
	else if(TIMx == TIM4)
	{
		RCC->APB1ENR |= 1 << 3;
	}

//	config 1s
//	TIMx->ARR = 10000;  // 1s
//	TIMx->PSC = 7200 - 1;
//	APB1 = 72Mhz
//  ARR  = 1000
//  PSC  = 72
//  Config timer 1s ?

	TIMx->ARR = 10000;  // 1s
	TIMx->PSC = 7200 - 1;

	//  Direction
	TIMx->CR1 &= ~(1 << 4); // Mode counter up

	// Counter enable
	TIMx->CR1 |= (1 << 0);

	// Update generation
	TIMx->EGR |= (1 << 0);
}

//viết thêm hàm delay ms?
void delay_ms(TIM_TypeDef *TIMx, uint32_t ms)
{

        // Reset thanh ghi đếm (Counter) về 0
        TIMx->CNT = 0;

        // F=72Mhz PSC=7200   --> f'=F/PSC = 72Mhz/7200-1 = 10000 --> t'=1/f'=1/10000 = 0.1ms -->(1s=1000ms)
        // 0.1ms x10000= 1000ms =1s
        while(TIMx->CNT < ms);

}

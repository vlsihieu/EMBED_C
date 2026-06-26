/*
 * timer.c
 *
 *  Created on: Jun 26, 2026
 *      Author: DUONG DINH TU
 */
#include "timer.h"

void Timer_2_and_4_Config(TIM_TypeDef *TIMx)
{
	/* Enable the clock for the corresponding timer*/
	if (TIMx == TIM2)
	{
		RCC->APB1ENR |= 1<<0; //bit0 = TIM2 EN
	}
	else if (TIMx == TIM4){
		RCC->APB1ENR |= 1<<3;  // bit3 = TIM4 EN
	}
	// APB1 Timer clock = 8MHz
	// PSC = 8000-1 -> CK_CNT = 8MHz / 8000 = 1kHz  => 1 tick = 1ms
	TIMx->ARR = 10000;
	TIMx->PSC = 8000-1;

	TIMx->CR1 &= ~(1<<4); // Direction = Up-counting
	TIMx->EGR |= (1 <<0);    // Update Generation: nạp PSC/ARR
	TIMx->CR1 |= (1<<0); //Counter Enable
}

// delay chặn (blocking), đơn vị ms, dựa vào timer đã được cấu hình tick 1ms.
void delay_ms (TIM_TypeDef *TIMx, uint16_t ms)
{
	TIMx->CNT = 0;
	while (TIMx->CNT < (uint32_t)ms);
}


/*
 * Timerx.c
 *
 *  Created on: Jun 9, 2026
 *      Author: Asus
 */

#include"Timerx.h"
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
//	APB1 = 72MHz
//  ARR  = 10000
//  PSC  = 7200-1
//  Config timer 1s ?

	TIMx->ARR = 10000;  
	TIMx->PSC = 7200- 1;

	//  Direction
	TIMx->CR1 &= ~(1 << 4); // Mode counter up
	// Update generation
		TIMx->EGR |= (1 << 0);

	// Counter enable
	TIMx->CR1 |= (1 << 0);



}

//  Delay
void delay_ms(TIM_TypeDef *TIMx, uint16_t ms)
{
	// Reset CNT về 0
	TIMx ->CNT = 0;
	// CNT chạy cho đến khi bằng số ms cụ thể
	/*  
	Để set time 1s
	Chọn PSC= 7200 để chia nhỏ clock :
	   F = 72M/7200 = 10000 cout/ 1s 
	Chọn ARR = 10000:
      1 cout = 0.1ms
	  => 0.1ms x 10000 = 1s

	*/ 
	while (TIMx ->CNT < ms);

}

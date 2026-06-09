/*
 * TIMER.c
 *
 *  Created on: Jun 5, 2026
 *      Author: quocvi
 */
#include"TIMER.H"
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
//	APB2 = 8MHz
//  ARR  = 10000
//  PSC  = 8000
//  Config timer 1s ?

	TIMx->ARR = 0xFFFF ;  // 1s
	TIMx->PSC = 8000- 1;

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
	while (TIMx ->CNT < ms);

}



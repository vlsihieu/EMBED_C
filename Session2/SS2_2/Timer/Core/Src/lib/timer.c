#include"TIMER.H"
void Timer2_4_IT_Config(TIM_TypeDef *TIMx)
{

	if(TIMx == TIM2)
	{
		RCC->APB1ENR |= 1 << 0;
	}
	else if(TIMx == TIM4)
	{
		RCC->APB1ENR |= 1 << 3;
	}

	TIMx->ARR = 10000 ;
	TIMx->PSC = 8000- 1;

	//  Direction
	TIMx->CR1 &= ~(1 << 4);
	// Update generation
	TIMx->EGR |= (1 << 0);

	// Counter enable
	TIMx->CR1 |= (1 << 0);
}

//  Delay
void delay_ms(TIM_TypeDef *TIMx, uint16_t ms)
{

	TIMx ->CNT = 0;

	while (TIMx->CNT < (uint32_t)ms);

}

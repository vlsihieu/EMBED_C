/*
 * private.c
 *
 *  Created on: Aug 30, 2026
 *      Author: PC
 */


#include "private.h"



// config tim2 for pwm
void init_pwm_timer(TIMx_HandleTypeDef *htim, TIMx_OC_InitTypeDef *sconfig, uint16_t pulse){

	htim->Instance = TIM2x;
	htim->Init.Period = 999;
	htim->Init.Prescaler = 71; //1khz
	htim->Init.ClockDivision = 0;
	htim->Init.CounterMode =0;
	htim->Init.RepetitionCounter = 0;

	HAL_TIM_PWM_Init(htim);

	sconfig->OCMode = TIM_OCMODE_PWM1;
	sconfig->Pulse = pulse;
	sconfig->OCFastMode = 0;
	sconfig->OCPolarity = TIM_OCPOLARITY_HIGH;

	HAL_TIM_PWM_ConfigChannel(htim, sconfig, TIM_CHANNEL_1);
}


// config tim3 for input capture
void init_IC_timer(TIMx_HandleTypeDef *htim, TIMx_IC_InitTypeDef *sconfig){
	htim->Instance = TIM3x;
	htim->Init.Period = 0xFFFF;
	htim->Init.Prescaler = 71; //1khz
	htim->Init.ClockDivision = 0;
	htim->Init.CounterMode =0;
	htim->Init.RepetitionCounter = 0;

	HAL_TIM_IC_Init(htim);

	sconfig->ICSelection = TIM_ICSELECTION_DIRECTTI;
	sconfig->ICFilter = 0;
	sconfig->ICPolarity = TIM_ICPOLARITY_RISING;
	sconfig->ICPrescaler = 0;

	HAL_TIM_IC_ConfigChannel(htim, sconfig, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
}

void init_gpio(){
    GPIOx_InitTypeDef_t gpio_pa0 = {0};
    gpio_pa0.Mode  = GPIOx_MODE_AF_PP;
    gpio_pa0.Pin   = GPIOx_PIN_0;
    gpio_pa0.Pull  = GPIOx_NOPULL;
    gpio_pa0.Speed = GPIOx_SPEED_FREQ_MEDIUM;
    GPIO_Init_t(GPIOAx, &gpio_pa0);

    GPIOx_InitTypeDef_t gpio_pa6 = {0};
    gpio_pa6.Mode  = GPIOx_MODE_AF_INPUT;
    gpio_pa6.Pin   = GPIOx_PIN_6;
    gpio_pa6.Pull  = GPIOx_NOPULL;
    gpio_pa6.Speed = GPIOx_SPEED_FREQ_LOW;
    GPIO_Init_t(GPIOAx, &gpio_pa6);
}

void init_clock(void){
	HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM2);
	HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM3);
	HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_GPIOA);
}






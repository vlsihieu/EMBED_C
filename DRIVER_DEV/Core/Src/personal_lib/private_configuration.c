/*
 * private_còniguration.c
 *
 *  Created on: Aug 25, 2026
 *      Author: PC
 */
#include "private_configuration.h"


void init_clock(void){
	HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM2);
	HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM3);
	HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_USART2);
	HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_GPIOA);
	HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_GPIOB);
}

void init_gpio(void){
	// PB0 = nút nhấn 1
	GPIOx_InitTypeDef_t gpio_pb0 = {0};
	gpio_pb0.Mode = GPIOx_MODE_INPUT;
	gpio_pb0.Pin = GPIOx_PIN_0;
	gpio_pb0.Pull = GPIOx_PULLUP;
	gpio_pb0.Speed = GPIOx_SPEED_FREQ_LOW;
	GPIO_Init_t(GPIOBx, &gpio_pb0);

	// PB1 = nút nhấn 2
	GPIOx_InitTypeDef_t gpio_pb1 = {0};
	gpio_pb1.Mode = GPIOx_MODE_INPUT;
	gpio_pb1.Pin = GPIOx_PIN_1;
	gpio_pb1.Pull = GPIOx_PULLUP;
	gpio_pb1.Speed = GPIOx_SPEED_FREQ_LOW;
	GPIO_Init_t(GPIOBx, &gpio_pb1);

	// PB5 = led
	GPIOx_InitTypeDef_t gpio_pb5 = {0};
	gpio_pb5.Mode = GPIOx_MODE_OUTPUT_PP;
	gpio_pb5.Pin = GPIOx_PIN_5;
	gpio_pb5.Pull = GPIOx_NOPULL;
	gpio_pb5.Speed = GPIOx_SPEED_FREQ_HIGH;
	GPIO_Init_t(GPIOBx, &gpio_pb5);

	// PB6 = led
	GPIOx_InitTypeDef_t gpio_pb6 = {0};
	gpio_pb6.Mode = GPIOx_MODE_OUTPUT_PP;
	gpio_pb6.Pin = GPIOx_PIN_6;
	gpio_pb6.Pull = GPIOx_NOPULL;
	gpio_pb6.Speed = GPIOx_SPEED_FREQ_HIGH;
	GPIO_Init_t(GPIOBx, &gpio_pb6);

	// PA0 = timer PWM
    GPIOx_InitTypeDef_t gpio_pa0 = {0};
    gpio_pa0.Mode  = GPIOx_MODE_AF_PP;
    gpio_pa0.Pin   = GPIOx_PIN_0;
    gpio_pa0.Pull  = GPIOx_NOPULL;
    gpio_pa0.Speed = GPIOx_SPEED_FREQ_MEDIUM;
    GPIO_Init_t(GPIOAx, &gpio_pa0);


    // PA2 = USART2_TX
    GPIOx_InitTypeDef_t gpio_pa2 = {0};
    gpio_pa2.Mode  = GPIOx_MODE_AF_PP;
    gpio_pa2.Pin   = GPIOx_PIN_2;
    gpio_pa2.Pull  = GPIOx_NOPULL;
    gpio_pa2.Speed = GPIOx_SPEED_FREQ_MEDIUM;
    GPIO_Init_t(GPIOAx, &gpio_pa2);

    // PA3 = USART2_RX
    GPIOx_InitTypeDef_t gpio_pa3 = {0};
    gpio_pa3.Mode  = GPIOx_MODE_INPUT;
    gpio_pa3.Pin   = GPIOx_PIN_3;
    gpio_pa3.Pull  = GPIOx_NOPULL;
    gpio_pa3.Speed = GPIOx_SPEED_FREQ_MEDIUM;
    GPIO_Init_t(GPIOAx, &gpio_pa3);
}

void init_uart(UARTx_HandleTypeDef *huart){
    huart->Instance = USART2x;
    huart->Init.BaudRate = 115200;
    huart->Init.WordLength = UARTx_WordLength_8B;
    huart->Init.Mode = UARTx_Mode_TX_RX;
    huart->Init.Parity = UARTx_Parity_NONE;
    huart->Init.StopBits = UARTx_StopBits_1;
    UARTx_Init(huart);
}

void init_timer_pwm(TIMx_HandleTypeDef *htim , TIMx_OC_InitTypeDef *sConfig){
    htim->Instance = TIM2x;
    htim->Init.Prescaler = 71;
    htim->Init.Period = 19999;
    htim->Init.ClockDivision = 0;
    htim->Init.RepetitionCounter = 0;
    htim->Init.CounterMode = 0;
    HAL_TIM_Base_Init(htim);


	sConfig->OCFastMode = 0;
	sConfig->OCMode = TIM_OCMODE_PWM1;
	sConfig->OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig->Pulse = 1500;
	HAL_TIM_PWM_ConfigChannel(htim, sConfig, TIM_CHANNEL_1);
}

void init_timer_delay(TIMx_HandleTypeDef *htim){
    htim->Instance = TIM3x;
    htim->Init.Prescaler = 71;
    htim->Init.Period = 0xFFFF;
    htim->Init.ClockDivision = 0;
    htim->Init.RepetitionCounter = 0;
    htim->Init.CounterMode = 0;
    HAL_TIM_Base_Init(htim);
}
void servo(TIMx_HandleTypeDef *htim2 ,uint16_t angle){
    if(angle > 180) angle = 180;
    uint16_t pulse_us = 1000 + (angle * 1000UL) / 180;
    htim2->Instance->CCR1 = pulse_us;
}






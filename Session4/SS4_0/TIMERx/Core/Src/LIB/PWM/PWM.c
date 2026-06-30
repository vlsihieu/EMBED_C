/*
 * PWM.c
 *
 *  Created on: Jun 23, 2026
 *      Author: quocvi
 */

/*
 * PWM.c
 *
 *  Created on: Jun 23, 2026
 *      Author: quocvi
 */
#include"main.h"
#include "PWM.h"
/**
 * Generate pwm with frequency= 1Hz, Duty= 50%
 *
 */
void PWM_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t*CRL = (uint32_t*)0x40010c00;
	uint32_t*CRH = (uint32_t*)0x40010c04;
	*CRL &= ~((0xF << 24) | (0xF << 24)); // reset pin 6
	*CRL |= (0b1011 << 24); // Enable pin 6
	*CRL &= ~((0xF << 28) | (0xF << 28)); // reset pin 7
	*CRL |= (0b1011 <<28); // Enable pin 7
	*CRH &= ~((0xF << 0) | (0xF << 0)); // Reset pin 8
	*CRH |= (0b1011  <<0);  // Enable pin 8
	*CRH &= ~((0xF << 4) | (0xF << 4)); // Reset pin 9
	*CRH |= (0b1011  <<4);  // Enable pin 9
	__HAL_RCC_TIM4_CLK_ENABLE();
	uint32_t *PSC = (uint32_t*)0x40000828;
	uint32_t *ARR = (uint32_t*)0x4000082c;
	uint32_t *CCR1 = (uint32_t*)0x40000834;
	uint32_t *CCR2 = (uint32_t*)0x40000838;
	uint32_t *CCR3 = (uint32_t*)0x4000083C;
	uint32_t *CCR4 = (uint32_t*)0x40000840;
	uint32_t *CCMR1_Output = (uint32_t*)0x40000818; // Quản lí CH1, 2
	uint32_t *CCMR2_Output = (uint32_t*)0x4000081C; // Quản lý CH3, CH4
	uint32_t *CCER = (uint32_t*)0x40000820;
	uint32_t *CR1 = (uint32_t*)0x40000800;
	*PSC =8000-1;
	*ARR = 1000;
	// Cài đặt độ rộng xung lần lượt 25%, 500%, 75%, 100%.
	*CCR1 = 250;
	*CCR2 = 500;
	*CCR3 = 750;
	*CCR4 = 1000;

	//Select Channel 1 work in compare --> Mode PWM1
	*CCMR1_Output &= ~((0b11 << 0) | (0b111 << 4) | (0b11 << 8) | (0b111 << 12));
	*CCMR1_Output |= ((0b110 << 4) | (0b110 << 12));
	*CCMR2_Output &= ~((0b11 << 0) | (0b111 << 4) | (0b11 << 8) | (0b111 << 12));
	*CCMR2_Output |= ((0b110 << 4) | (0b110 << 12));

	//Enable channel 1,2,3,4( Compare/capture)
	*CCER |= ((1 << 0) | (1 << 4) | (1 << 8) | (1 << 12));
	//Enable cnt
	*CR1 |= 1<<0;



}



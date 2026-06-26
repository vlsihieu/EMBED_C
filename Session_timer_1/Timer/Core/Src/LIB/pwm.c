/*
 * pwm.c
 *
 *  Created on: Jun 25, 2026
 *      Author: Asus
 */

#include"main.h"
void pwm_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

    uint32_t* CRL =(uint32_t*)0x40010800;
    uint32_t* CRH =(uint32_t*)0x40010804;
	*CRL &=~(0x0F << 24);
	*CRL |=(0b1011 << 24);
	*CRL &=~(0x0F << 28);
	*CRL |=(0b1011 << 28);
	*CRH &=~(0x0F << 0);
	*CRH |=(0b1011 << 0);
	*CRH &=~(0x0F << 4);
	*CRH |=(0b1011 << 4);

	uint32_t* PSC =(uint32_t*)0x40000828;
	uint32_t* ARR = (uint32_t*)0x4000082c;

	uint32_t* CCR1 = (uint32_t*)0x40000834;
	uint32_t* CCR2 = (uint32_t*)0x40000838;
	uint32_t* CCR3 = (uint32_t*)0x4000083c;
	uint32_t* CCR4 = (uint32_t*)0x40000840;

	uint32_t* CCMR1_Output = (uint32_t*)0x40000818;
	uint32_t* CCMR2_Output = (uint32_t*)0x4000081c;
	uint32_t* CCER = (uint32_t*)0x40000820;
	uint32_t* CR1 = (uint32_t*)0x40000800;

	*ARR = 10000;
	*PSC = 72000-1;
	*CCR1 = 2500;
	*CCR2 = 5000;
	*CCR3 = 7500;
	*CCR4 = 10000;

	// SELECT CHANNEL
	*CCMR1_Output &=~((0b11 << 0)|(0b11 << 8)|(0b111 << 4)|(0b111 << 12));
	*CCMR1_Output |=(0b110 << 4 )|(0b110 << 12);

	*CCMR2_Output &=~((0b11 << 0)|(0b11 << 8)|(0b111 << 4)|(0b111 << 12));
	*CCMR2_Output |=(0b110 << 4 )|(0b110 << 12);

	// Enable channel
	*CCER = (1 << 0)|(1 << 4)|(1 << 8)|(1 << 12);

	// Enable counter
	*CR1 |=(1 << 0);





}

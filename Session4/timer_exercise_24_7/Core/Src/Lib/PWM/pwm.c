#include"main.h"
#include"pwm.h"

void PWM_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	uint32_t*CRL = (uint32_t*)0x40010c00;
	uint32_t*CRH = (uint32_t*)0x40010c04;

	// 0xF bit mask, it corresponds  to 1111
	// mode = 11(output max speed 50Mhz); CNF=10.
	*CRL &= ~(0xF<<24); *CRL |= (0b1011<<24); //PB6
	*CRL &= ~(0xF<<28); *CRL |= (0b1011<<28); //PB7
	*CRH &= ~(0xF<<0); *CRH |= (0b1011<<0);  //PB8
	*CRH &= ~(0xF<<4); *CRH |= (0b1011<<4);  //PB9


	//output compare (PWM)
	__HAL_RCC_TIM4_CLK_ENABLE();
	uint32_t*PSC = (uint32_t*)0x40000828;
	uint32_t*ARR = (uint32_t*)0x4000082c;
	uint32_t*CCR1 = (uint32_t*)0x40000834;
	uint32_t*CCR2 = (uint32_t*)0x40000838;
	uint32_t*CCR3 = (uint32_t*)0x4000083c;
	uint32_t*CCR4 = (uint32_t*)0x40000840;

	*PSC = 8000-1; // [0, 1, 2.....7999] = 8000
	*ARR = 1000-1;


	// setup individual pulse widths like 25%, 50%, 75%, 100%.
	*CCR1 = 250;
	*CCR2 = 500;
	*CCR3 = 750;
	*CCR4 = 1000;

	uint32_t *CCMR1_Output = (uint32_t*)0x40000818; //  CH1, CH2
	uint32_t *CCMR2_Output = (uint32_t*)0x4000081C; // CH3, CH4

	*CCMR1_Output &= ~((0b11<<0)|(0b111<<4)|(0b11<<8)|(0b111<<12));
	*CCMR1_Output |= ((0b110 << 4) | (0b110 << 12));
	*CCMR2_Output &= ~((0b11<<0)|(0b111<<4)|(0b11<<8)|(0b111<<12));
	*CCMR2_Output |= ((0b110 << 4) | (0b110 << 12));



	uint32_t *CCER = (uint32_t*)0x40000820;
	uint32_t *CR1  = (uint32_t*)0x40000800;

	//Enable channel 1,2,3,4( Compare/capture)
	*CCER |= ((1 << 0) | (1 << 4) | (1 << 8) | (1 << 12));
	*CR1  |= 1 << 0; //Enable cnt



}

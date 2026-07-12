/*
 * capture.c
 *
 *  Created on: Jul 11, 2026
 *      Author: Asus
 */

#include"main.h"
#include"capture.h"
void capture_init()
{
	// cấp clock cho chân GPIO và timer
	__HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIOA pin 0 -> pin 3
    uint32_t* CRL = (uint32_t*)0x40010800;
    *CRL &=~((0b1111 << 0)|
    		(0b1111 << 4)|
			(0b1111 << 8)|
			(0b1111 << 12));
    *CRL |=((0b0100 << 0)|(0b0100 << 4)|(0b0100 << 8)|(0b0100 << 12));

    // set timer basic
    uint16_t* Tim2_ARR = (uint16_t*)0x4000002c;
    uint16_t* Tim2_PSC = (uint16_t*)0x40000028;
    uint16_t* Tim2_CNT = (uint16_t*)0x40000024;
    *Tim2_ARR = 0xffff;
    *Tim2_PSC = 71;

    // set 4 channel
    uint16_t* CCMR1 = (uint16_t*)0x40000018;
    uint16_t* CCMR2 = (uint16_t*)0x4000001c;
    *CCMR1 &=~((0b11u << 0)|(0b11u << 8)); // set ch1 , ch2
    *CCMR1 |= (0b01 << 0) | (0b01 << 8);

    *CCMR2 &=~((0b11u << 0)|(0b11u << 8)); // set ch3 , ch4
    *CCMR2 |= (0b01 << 0) | (0b01 << 8);

    // set  rising để capture 1 chu kỳ ở channel 1
    uint16_t* CCER = (uint16_t*)0x40000020;
    *CCER &=~(1 << 1);
    *CCER |=(1 << 0);
    // set falling để capture giá trị duty ở 3 channel còn lại
    *CCER |=(1 << 5)|(1 << 9)|(1 << 13);
	//enable 4 channel
    *CCER |=(1 << 0)|(1 << 4)|(1 << 8)|(1 << 12);
    *TIM2_CNT = 0;

    // reset về 0 sau 1T
    uint16_t* SMCR = (uint16_t*)0x40000008;
    *SMCR &=~((0b111u << 0)
    		|(0b111u << 4));
    *SMCR |=(0b100 << 0)|(0b101 << 4);

    // enable counter
    uint16_t* TIM2_CR1   = (uint16_t*)0x40000000;
    *TIM2_CR1 |= (1 << 0);
}

uint32_t capture_read()
   {
   	uint32_t* TIM2_CCR1 = (uint32_t*)0x40000034;
   	return *TIM2_CCR1;

   }
   uint32_t capture_pulse_width1()
   {
   	uint32_t* TIM2_CCR2 = (uint32_t*)0x40000038;
   	return *TIM2_CCR2;
   }
   uint32_t capture_pulse_width2()
   {
   	uint32_t* TIM2_CCR3 = (uint32_t*)0x4000003c;
   	return *TIM2_CCR3;
   }
   uint32_t capture_pulse_width3()
   {
   	uint32_t* TIM2_CCR4 = (uint32_t*)0x40000040;
   	return *TIM2_CCR4;
   }
   uint32_t capture_pulse_width4()
   {
       return 0;
   }








#include "main.h"
#include "capture.h"

#define TIM2_ADDR_BASE 0x40000000
#define GPIOA_ADDR_BASE 0x40010800

void capture_init()
{
	// configure 4 input pins
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIOA->CRL &= ~(0xF <<0); // pin 0
	GPIOA->CRL |= (0b1000<<0);
	GPIOA->CRL &= ~(0xF <<4);  // pin 1
	GPIOA->CRL |= (0b1000<<4);
	GPIOA->CRL &= ~(0xF <<8); // pin 2
	GPIOA->CRL |= (0b1000<<8);
	GPIOA->CRL &= ~(0xF <<12);  // pin 3
	GPIOA->CRL |= (0b1000<<12);


	// configure TIM2
	__HAL_RCC_TIM2_CLK_ENABLE();
	uint32_t* TIM2_ARR = (uint32_t*)(TIM2_ADDR_BASE+0x2C);
	// or: uint32_t* TIM2_ARR = (uint32_t*)0x4000002c;
	uint32_t* TIM2_PSC = (uint32_t*)0x40000028;


	*TIM2_ARR = 0xFFFF;  // Max value (16-bit): 65535
	*TIM2_PSC = 800-1;



	//select Input Capture mode for 4 channels (CCMR)
	uint16_t* TIM2_CCMR1 = (uint16_t*)0x40000018;
	uint16_t* TIM2_CCMR2 = (uint16_t*)0x4000001c;

	*TIM2_CCMR1 &= ~(0b11111111<<0); //clear CC1 (CC1S, IC1PSC, IC1F)
	*TIM2_CCMR1 &= ~(0b11111111<<8);  // clear CC2 (bit 8-15).
    *TIM2_CCMR1 |= ((0b01 << 0)|(0b01<<8)); // Set chế độ Input mode cho CH1, CH2

    *TIM2_CCMR2 &= ~(0b11111111<<0); //clear CC3
    *TIM2_CCMR2 &= ~(0b11111111<<8);  //clear CC4
    *TIM2_CCMR2 |= ((0b01<<0)|(0b01<<8)); // set chế độ input mode cho CH3, CH4


    // rising&falling --CCER
    uint16_t* TIM2_CCER = (uint16_t*)0x40000020;
    *TIM2_CCER &= ~(1<<1);  // clear rising để làm trigger capture
    *TIM2_CCER |= ((1<<5)|(1<<9)|(1<<13)); //input capture falling egde
    *TIM2_CCER |= ((1<<0)|(1<<4)|(1<<8)|(1<<12)); //Enable cho CH1, CH2, CH3, CH4


    uint32_t* TIM2_CNT = (uint32_t*)0x40000024;
    *TIM2_CNT = 0;  // reset CNT to 0 before starting



    // cấu hình  slave mode control register (chỉ áp dụng cho đo chu kỳ)

    uint16_t* TIM2_SMCR = (uint16_t*)0x40000008;
    *TIM2_SMCR |= (0b100<<0)|(0b101<<4); //reset cnt khi có rising.



    // khởi động boojj đếm.
    uint16_t* TIM2_CR1 = (uint16_t*)0x40000000;
    *TIM2_CR1 |= (1<<0); // counter enable

}

// các hàm giá trị
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







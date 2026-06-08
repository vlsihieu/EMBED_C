/*
 * make library by ChucDao
 * congchuc289@gmail.com
 */
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "Define.h"
#include <stdint.h>

typedef enum {
	AFIO_EN = 0,

	IOPA_EN = 2,
	IOPB_EN,
	IOPC_EN,
	IOPD_EN,
	IOPE_EN,
	IOPF_EN,
	IOPG_EN,
	ADC1_EN,
	ADC2_EN,
	TIM1_EN,
	SPI1_EN,
	TIM8_EN,
	USART1_EN,
	ADC3_EN,

	TIM9_EN = 19,
	TIM10_EN,
	TIM11_EN
} RCC_APB2ENR;

typedef enum {
	TIM2_EN = 0,
	TIM3_EN,
	TIM4_EN,
	TIM5_EN,
	TIM6_EN,
	TIM7_EN,
	TIM12_EN,
	TIM13_EN,
	TIM14_EN,

	WWDg_EN = 11,

	SPI2_EN = 14,
	SPI3_EN,

	USART2_EN = 17,
	USART3_EN,
	UART4_EN,
	UART5_EN,
	I2C1_EN,
	I2C2_EN,
	USB_EN,

	CAN_EN = 25,

	BKP_EN = 27,
	PWR_EN,
	DAC_EN
} RCC_APB1ENR;

typedef enum{
	DMA1x = 0,
	DMA2x,
	SRAMx_EN,
	Res1x,
	FLITFx_EN,
	Res2x,
	CRCx_EN,
	Res3x,
	FSMCx_EN,
	Res4x,
	SDIOx_EN,
	Res5x
} RCC_AHBENR;

void Config_Clock_HSE_Register(void);
void Config_Clock_HSI_Register(void);
void RCC_APB1_Peripheral(RCC_APB1ENR index);
void RCC_APB2_Peripheral(RCC_APB2ENR index);
void RCC_AHB_Peripheral(RCC_AHBENR index);
void RCC_DeInit(void);
#endif /* MY_LIBRARY_LIBRARY_STM32_REGISTER_CLOCK_H_ */

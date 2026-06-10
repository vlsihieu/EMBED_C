/*
 * cnt.h
 *
 *  Created on: Jun 8, 2026
 *      Author: PC
 */
#ifndef MYLIB_INC_GPIO_DRV_H_
#define MYLIB_INC_GPIO_DRV_H_

#include <stdint.h>
#include "stm32f103xb.h"

void TIMx_init(TIM_TypeDef *TIMx);
void delayms(TIM_TypeDef *TIMx,uint16_t msecond);

#endif


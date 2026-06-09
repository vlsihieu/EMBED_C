/*
 * TIMERx.h
 *
 *  Created on: Jun 9, 2026
 *      Author: quocvi
 */

#ifndef SRC_LIB_TIMER_H_
#define SRC_LIB_TIMER_H_
#include "main.h"
#include <stdint.h>
void Timer2_4_IT_Config(TIM_TypeDef *TIMx);
void delay_ms(TIM_TypeDef *TIMx, uint16_t ms);
#endif /* SRC_LIB_TIMER_H_ */

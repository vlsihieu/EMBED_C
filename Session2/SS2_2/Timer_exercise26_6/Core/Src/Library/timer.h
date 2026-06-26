/*
 * timer.h
 *
 *  Created on: Jun 26, 2026
 *      Author: DUONG DINH TU
 */

#ifndef SRC_LIBRARY_TIMER_H_
#define SRC_LIBRARY_TIMER_H_
#include "main.h"
#include <stdint.h>

void Timer_2_and_4_Config(TIM_TypeDef *TIMx);
void delay_ms(TIM_TypeDef *TIMx, uint16_t ms);




#endif /* SRC_LIBRARY_TIMER_H_ */

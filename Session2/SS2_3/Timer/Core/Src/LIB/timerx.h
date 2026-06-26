/*
 * timerx.h
 *
 *  Created on: Jun 14, 2026
 *      Author: vdtt0
 */

#ifndef SRC_LIB_TIMERX_H_
#define SRC_LIB_TIMERX_H_
#include "main.h"
#include <stdint.h>
void Timer2_4_IT_Config(TIM_TypeDef *TIMx);
void delay_ms(TIM_TypeDef *TIMx, uint32_t ms);


#endif /* SRC_LIB_TIMERX_H_ */

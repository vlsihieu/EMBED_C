/*
 * private.h
 *
 *  Created on: Aug 30, 2026
 *      Author: PC
 */

#ifndef SRC_LIB_PERSONAL_LIB_PRIVATE_H_
#define SRC_LIB_PERSONAL_LIB_PRIVATE_H_
#include "../peripheral_Lib/Driver/timer/hal_timer.h"
#include "../peripheral_Lib/Driver/gpio/hal_gpio.h"
#include "../peripheral_Lib/Utils/common/define.h"
#include "../peripheral_Lib/Driver/rcc/hal_clock.h"



void init_pwm_timer(TIMx_HandleTypeDef *htim, TIMx_OC_InitTypeDef *sconfig, uint16_t pulse);
void init_IC_timer(TIMx_HandleTypeDef *htim, TIMx_IC_InitTypeDef *sconfig);
void init_gpio();
void init_clock(void);


#endif /* SRC_LIB_PERSONAL_LIB_PRIVATE_H_ */

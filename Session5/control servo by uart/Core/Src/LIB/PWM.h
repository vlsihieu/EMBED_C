/*
 * PWM.h
 *
 *  Created on: Aug 1, 2026
 *      Author: PC
 */

#ifndef SRC_LIB_PWM_H_
#define SRC_LIB_PWM_H_

#include "stm32f103xb.h"
#include <stdint.h>

void TIM2_init();
void angle_to_CCR(uint16_t angle);


#endif /* SRC_LIB_PWM_H_ */

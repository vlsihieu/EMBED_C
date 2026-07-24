/*
 * timer.h
 *
 *  Created on: Jul 10, 2026
 *      Author: PC
 */

#ifndef INC_A_TIMER_H_
#define INC_A_TIMER_H_

#include <stdint.h>
#include "stm32f103xb.h"


typedef enum {
	CH1 = 1,
	CH2,
	CH3,
	CH4,
} PWM_channel;


typedef enum {
    PWM_OK = 0,
    PWM_ERR_INVALID_TIMER,
    PWM_ERR_DUTY_OUT_OF_RANGE,
    PWM_ERR_NOT_INITIALIZED,
} PWM_Status_t;

//driver OC
PWM_Status_t TIM2_init(PWM_channel selection);
PWM_Status_t TIM2_setduty(PWM_channel selection, uint16_t pwm);

//driver IC
void capture_init(TIM_TypeDef *TIMx);


#endif /* INC_A_TIMER_H_ */

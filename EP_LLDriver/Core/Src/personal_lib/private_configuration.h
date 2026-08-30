/*
 * private_configuration.h
 *
 *  Created on: Aug 25, 2026
 *      Author: PC
 */

#ifndef SRC_PERSONAL_LIB_PRIVATE_CONFIGURATION_H_
#define SRC_PERSONAL_LIB_PRIVATE_CONFIGURATION_H_
#include "hal_gpio.h"
#include "hal_timer.h"
#include "hal_dma.h"
#include "hal_clock.h"
#include "hal_uart.h"


void init_clock(void);
void init_gpio(void);
void init_uart(UARTx_HandleTypeDef *huart);
void init_timer_pwm(TIMx_HandleTypeDef *htim , TIMx_OC_InitTypeDef *sConfig);
void init_timer_delay(TIMx_HandleTypeDef *htim);
void servo(TIMx_HandleTypeDef *htim2 ,uint16_t angle);



#endif /* SRC_PERSONAL_LIB_PRIVATE_CONFIGURATION_H_ */

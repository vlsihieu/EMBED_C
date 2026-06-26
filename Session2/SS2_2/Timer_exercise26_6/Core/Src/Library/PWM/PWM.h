/*
 * PWM.h
 *
 *  Created on: Jun 26, 2026
 *      Author: DUONG DINH TU
 */

#ifndef SRC_LIBRARY_PWM_PWM_H_
#define SRC_LIBRARY_PWM_PWM_H_

/**
 - Tạo PWM 4 kênh trên TIM4 (PB6..PB9), tần số 1Hz,
 - duty cycle 4 kênh: 25% / 50% / 75% / 100%
 */
void PWM_init(void);

#endif /* SRC_LIBRARY_PWM_PWM_H_ */

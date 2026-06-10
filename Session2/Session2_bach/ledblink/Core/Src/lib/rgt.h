/*
 * rgt.h
 *
 *  Created on: May 28, 2026
 *      Author: PC
 */
#ifndef MYLIB_INC_GPIO_DRV_H_
#define MYLIB_INC_GPIO_DRV_H_

#include <stdint.h>
#include "stm32f103xb.h"

// MODE
#define INPUT_MODE                    ((uint8_t)0x0)
#define OUTPUT_MODE_2MHZ              ((uint8_t)0x1)
#define OUTPUT_MODE_10MHZ             ((uint8_t)0x2)
#define OUTPUT_MODE_50MHZ             ((uint8_t)0x3)

// CNF IF MODE == 0(input)
#define ANALOG                        ((uint8_t)0x0)
#define FLOATING                      ((uint8_t)0x1)
#define PULL_UP_DOWN                  ((uint8_t)0x2)

// CNF IF MODE != 0(OUTPUT)
#define PUSH_PULL                     ((uint8_t)0x0)
#define OPEN_DRAIN                    ((uint8_t)0x1)
#define AL_FUNC_PUSH_PULL             ((uint8_t)0x2)
#define AL_FUNC_OPEN_DRAIN            ((uint8_t)0x3)

//pull
#define NO_PULL                       ((uint8_t)0x0)
#define PULL_UP                       ((uint8_t)0x1)
#define PULL_DOWN                     ((uint8_t)0x2)

#define PIN_0                         ((uint8_t)0x0)
#define PIN_1                         ((uint8_t)0x1)
#define PIN_2                         ((uint8_t)0x2)
#define PIN_3                         ((uint8_t)0x3)
#define PIN_4                         ((uint8_t)0x4)
#define PIN_5                         ((uint8_t)0x5)
#define PIN_6                         ((uint8_t)0x6)
#define PIN_7                         ((uint8_t)0x7)
#define PIN_8                         ((uint8_t)0x8)
#define PIN_9                         ((uint8_t)0x9)
#define PIN_10                        ((uint8_t)0x0A)
#define PIN_11                        ((uint8_t)0x0B)
#define PIN_12                        ((uint8_t)0x0C)
#define PIN_13                        ((uint8_t)0x0D)
#define PIN_14                        ((uint8_t)0x0E)
#define PIN_15                        ((uint8_t)0x0F)


void GPIO_init(GPIO_TypeDef *GPIOx,uint8_t pin, uint8_t mode, uint8_t cnf, uint8_t pull );
void GPIO_write(GPIO_TypeDef *GPIOx,uint8_t pin_number, uint8_t state);
uint8_t GPIO_read(GPIO_TypeDef *GPIOx, uint8_t pin_number);



#endif




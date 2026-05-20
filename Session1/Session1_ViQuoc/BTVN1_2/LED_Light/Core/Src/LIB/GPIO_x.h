/*
 * GPIO_x.h
 *
 * Created on: May 20, 2026
 * Author: quocvi
 */

#ifndef SRC_LIB_GPIO_X_H_
#define SRC_LIB_GPIO_X_H_

#include <stdint.h>
#include "stm32f103xb.h"

// MODE INPUT
#define INPUT_ANALOG_MODE           ((uint8_t)0x0)
#define INPUT_FLOATING_IN           ((uint8_t)0x1)
#define INPUT_PULL_UP_DOW           ((uint8_t)0x2)

// MODE OUTPUT
#define OUT_PUSH_PULL               ((uint8_t)0x0)
#define OUT_OPEN_DRAIN              ((uint8_t)0x1)
#define OUT_AL_FUNC_PUSH_PULL       ((uint8_t)0x2)
#define OUT_AL_FUNC_OPEN_DRAIN      ((uint8_t)0x03)

// MODE SPEED
#define INPUT_MODE                  ((uint8_t)0x0)
#define OUTPUT_SPEED_10MHZ          ((uint8_t)0x1)
#define OUTPUT_SPEED_2MHZ           ((uint8_t)0x2)
#define OUTPUT_SPEED_50MHZ          ((uint8_t)0x3)

// PULL CONFIGURATION
#define NO_PULL                     ((uint8_t)0x0)
#define PULL_UP                     ((uint8_t)0x1)
#define PULL_DOWN                   ((uint8_t)0x2)

// PIN SELECTION
#define GPIOx_Pin_0                  ((uint16_t)0)   /*!< Pin 0 selected */
#define GPIOx_Pin_1                  ((uint16_t)1)   /*!< Pin 1 selected */
#define GPIOx_Pin_2                  ((uint16_t)2)   /*!< Pin 2 selected */
#define GPIOx_Pin_3                  ((uint16_t)3)   /*!< Pin 3 selected */
#define GPIOx_Pin_4                  ((uint16_t)4)   /*!< Pin 4 selected */
#define GPIOx_Pin_5                  ((uint16_t)5)   /*!< Pin 5 selected */
#define GPIOx_Pin_6                  ((uint16_t)6)   /*!< Pin 6 selected */
#define GPIOx_Pin_7                  ((uint16_t)7)   /*!< Pin 7 selected */
#define GPIOx_Pin_8                  ((uint16_t)8)   /*!< Pin 8 selected */
#define GPIOx_Pin_9                  ((uint16_t)9)   /*!< Pin 9 selected */
#define GPIOx_Pin_10                 ((uint16_t)10)  /*!< Pin 10 selected */
#define GPIOx_Pin_11                 ((uint16_t)11)  /*!< Pin 11 selected */
#define GPIOx_Pin_12                 ((uint16_t)12)  /*!< Pin 12 selected */
#define GPIOx_Pin_13                 ((uint16_t)13)  /*!< Pin 13 selected */
#define GPIOx_Pin_14                 ((uint16_t)14)  /*!< Pin 14 selected */
#define GPIOx_Pin_15                 ((uint16_t)15)  /*!< Pin 15 selected */

/* define function prototypes */
void GPIOx_Init(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t mode, uint8_t pull, uint8_t speed);
uint8_t GPIOx_Read(GPIO_TypeDef *GPIOx, uint16_t pin_number);
void GPIOx_Write(GPIO_TypeDef *GPIOx, uint16_t pin_number, uint8_t state);
void GPIOx_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin_number);

#endif /* SRC_LIB_GPIO_X_H_ */

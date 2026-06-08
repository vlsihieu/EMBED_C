/*
 * GPIOx.h
 *
 *  Created on: May 6, 2026
 *      Author: hieuk
 */

#ifndef LIB_GPIO_INC_GPIOX_IP_H_
#define LIB_GPIO_INC_GPIOX_IP_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                     INCLUDES AND 
 **********************************************************************************************************************/

#include <stdint.h>
#include "Define.h"
#include <CLOCKx_Ip.h>


/***********************************************************************************************************************
 *                                                   LOCAL MACROS
 **********************************************************************************************************************/

 /**
 * @brief Validate GPIO pin action (RESET or SET).
 */
#define ASSET_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || \
										((ACTION) == GPIO_PIN_SET))

/**
 * @brief Validate GPIO pin mask.
 */
#define ASSET_GPIO_PIN(PIN)         (((((uint32_t)PIN) & GPIO_PIN_MASK ) != 0x00u) && \
									((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00u))

/**
 * @brief Validate GPIO mode.
 */									
#define ASSET_GPIO_MODE(MODE) 	(((MODE) == GPIO_MODE_INPUT)             ||\
                            	((MODE) == GPIO_MODE_OUTPUT_PP)          ||\
                            	((MODE) == GPIO_MODE_OUTPUT_OD)          ||\
                           	 	((MODE) == GPIO_MODE_AF_PP)              ||\
                            	((MODE) == GPIO_MODE_AF_OD)              ||\
                            	((MODE) == GPIO_MODE_IT_RISING)          ||\
                            	((MODE) == GPIO_MODE_IT_FALLING)         ||\
                            	((MODE) == GPIO_MODE_IT_RISING_FALLING)  ||\
                            	((MODE) == GPIO_MODE_EVT_RISING)         ||\
                            	((MODE) == GPIO_MODE_EVT_FALLING)        ||\
                            	((MODE) == GPIO_MODE_EVT_RISING_FALLING) ||\
                            	((MODE) == GPIO_MODE_ANALOG))

/**
 * @brief Check if GPIO speed is valid.
 */
#define ASSET_GPIO_SPEED(SPEED) 	(((SPEED) == GPIO_SPEED_FREQ_LOW) || \
                              		((SPEED) == GPIO_SPEED_FREQ_MEDIUM) || ((SPEED) == GPIO_SPEED_FREQ_HIGH))

/**
 * @brief Check if GPIO pull setting is valid.
 */
#define ASSET_GPIO_PULL(PULL) 		(((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || \
                            		((PULL) == GPIO_PULLDOWN))

/**
 * @brief Check if GPIO instance is valid.
 */
#define ASSET_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB) || \
                                        ((INSTANCE) == GPIOC) || \
                                        ((INSTANCE) == GPIOD) || \
                                        ((INSTANCE) == GPIOE))

/**
 * @brief Check if AF instance is valid.
 */
#define ASSET_GPIO_AF_INSTANCE(INSTANCE) ASSET_GPIO_ALL_INSTANCE(INSTANCE)

/**
 * @brief GPIO pin x mask
 */
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

/**
 * @brief GPIO port x base pointer.
 */
#define GPIOAx 	((GPIO_typedef_t *)GPIOx_Portx_A_Address)
#define GPIOBx 	((GPIO_typedef_t *)GPIOx_Portx_B_Address)
#define GPIOCx	((GPIO_typedef_t *)GPIOx_Portx_C_Address)
#define GPIODx	((GPIO_typedef_t *)GPIOx_Portx_D_Address)
#define GPIOEx 	((GPIO_typedef_t *)GPIOx_Portx_E_Address)
#define GPIOFx	((GPIO_typedef_t *)GPIOx_Portx_F_Address)
#define GPIOGx	((GPIO_typedef_t *)GPIO_Portx_G_Address)
#define AFIOx 	((AFIO_typedef_t *)AFIOx_Address)

/**
 * @brief Bit definition for GPIO_LCKR register
 */
#define GPIOx_LCKR_LCK0_Pos                   (0U)                              
#define GPIOx_LCKR_LCK0_Msk                   (0x1UL << GPIOx_LCKR_LCK0_Pos)      	///< 0x00000001 */
#define GPIOx_LCKR_LCK0                       GPIOx_LCKR_LCK0_Msk                	///< Port x Lock bit 0 */
#define GPIOx_LCKR_LCK1_Pos                   (1U)                          
#define GPIOx_LCKR_LCK1_Msk                   (0x1UL << GPIOx_LCKR_LCK1_Pos)      	///< 0x00000002 */
#define GPIOx_LCKR_LCK1                       GPIOx_LCKR_LCK1_Msk                	///< Port x Lock bit 1 */
#define GPIOx_LCKR_LCK2_Pos                   (2U)                          
#define GPIOx_LCKR_LCK2_Msk                   (0x1UL << GPIOx_LCKR_LCK2_Pos)      	///< 0x00000004 */
#define GPIOx_LCKR_LCK2                       GPIOx_LCKR_LCK2_Msk                	///< Port x Lock bit 2 */
#define GPIOx_LCKR_LCK3_Pos                   (3U)                          
#define GPIOx_LCKR_LCK3_Msk                   (0x1UL << GPIOx_LCKR_LCK3_Pos)      	///< 0x00000008 */
#define GPIOx_LCKR_LCK3                       GPIOx_LCKR_LCK3_Msk                	///< Port x Lock bit 3 */
#define GPIOx_LCKR_LCK4_Pos                   (4U)                          
#define GPIOx_LCKR_LCK4_Msk                   (0x1UL << GPIOx_LCKR_LCK4_Pos)      	///< 0x00000010 */
#define GPIOx_LCKR_LCK4                       GPIOx_LCKR_LCK4_Msk                	///< Port x Lock bit 4 */
#define GPIOx_LCKR_LCK5_Pos                   (5U)                          
#define GPIOx_LCKR_LCK5_Msk                   (0x1UL << GPIOx_LCKR_LCK5_Pos)      	///< 0x00000020 */
#define GPIOx_LCKR_LCK5                       GPIOx_LCKR_LCK5_Msk                	///< Port x Lock bit 5 */
#define GPIOx_LCKR_LCK6_Pos                   (6U)                          
#define GPIOx_LCKR_LCK6_Msk                   (0x1UL << GPIOx_LCKR_LCK6_Pos)      	///< 0x00000040 */
#define GPIOx_LCKR_LCK6                       GPIOx_LCKR_LCK6_Msk                	///< Port x Lock bit 6 */
#define GPIOx_LCKR_LCK7_Pos                   (7U)                          
#define GPIOx_LCKR_LCK7_Msk                   (0x1UL << GPIOx_LCKR_LCK7_Pos)      	///< 0x00000080 */
#define GPIOx_LCKR_LCK7                       GPIOx_LCKR_LCK7_Msk                	///< Port x Lock bit 7 */
#define GPIOx_LCKR_LCK8_Pos                   (8U)                          
#define GPIOx_LCKR_LCK8_Msk                   (0x1UL << GPIOx_LCKR_LCK8_Pos)      	///< 0x00000100 */
#define GPIOx_LCKR_LCK8                       GPIOx_LCKR_LCK8_Msk                	///< Port x Lock bit 8 */
#define GPIOx_LCKR_LCK9_Pos                   (9U)                          
#define GPIOx_LCKR_LCK9_Msk                   (0x1UL << GPIOx_LCKR_LCK9_Pos)      	///< 0x00000200 */
#define GPIOx_LCKR_LCK9                       GPIOx_LCKR_LCK9_Msk                	///< Port x Lock bit 9 */
#define GPIOx_LCKR_LCK10_Pos                  (10U)                         
#define GPIOx_LCKR_LCK10_Msk                  (0x1UL << GPIOx_LCKR_LCK10_Pos)     	///< 0x00000400 */
#define GPIOx_LCKR_LCK10                      GPIOx_LCKR_LCK10_Msk               	///< Port x Lock bit 10 */
#define GPIOx_LCKR_LCK11_Pos                  (11U)                         
#define GPIOx_LCKR_LCK11_Msk                  (0x1UL << GPIOx_LCKR_LCK11_Pos)     	///< 0x00000800 */
#define GPIOx_LCKR_LCK11                      GPIOx_LCKR_LCK11_Msk               	///< Port x Lock bit 11 */
#define GPIOx_LCKR_LCK12_Pos                  (12U)                         
#define GPIOx_LCKR_LCK12_Msk                  (0x1UL << GPIOx_LCKR_LCK12_Pos)     	///< 0x00001000 */
#define GPIOx_LCKR_LCK12                      GPIOx_LCKR_LCK12_Msk               	///< Port x Lock bit 12 */
#define GPIOx_LCKR_LCK13_Pos                  (13U)                         
#define GPIOx_LCKR_LCK13_Msk                  (0x1UL << GPIOx_LCKR_LCK13_Pos)     	///< 0x00002000 */
#define GPIOx_LCKR_LCK13                      GPIOx_LCKR_LCK13_Msk               	///< Port x Lock bit 13 */
#define GPIOx_LCKR_LCK14_Pos                  (14U)                         
#define GPIOx_LCKR_LCK14_Msk                  (0x1UL << GPIOx_LCKR_LCK14_Pos)     	///< 0x00004000 */
#define GPIOx_LCKR_LCK14                      GPIOx_LCKR_LCK14_Msk               	///< Port x Lock bit 14 */
#define GPIOx_LCKR_LCK15_Pos                  (15U)                         
#define GPIOx_LCKR_LCK15_Msk                  (0x1UL << GPIOx_LCKR_LCK15_Pos)     	///< 0x00008000 */
#define GPIOx_LCKR_LCK15                      GPIOx_LCKR_LCK15_Msk                	///< Port x Lock bit 15 */
#define GPIOx_LCKR_LCKK_Pos                   (16U)                         
#define GPIOx_LCKR_LCKK_Msk                   (0x1UL << GPIOx_LCKR_LCKK_Pos)      	///< 0x00010000 */
#define GPIOx_LCKR_LCKK                       GPIOx_LCKR_LCKK_Msk                 	///< Lock key */

/***********************************************************************************************************************
 *                                       LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

 /**
 * @brief GPIO pin state definition.
 */
typedef enum{
	LOW = 0,
	HIGH
}GPIO_PinState_t;

/**
 * @brief GPIO status return type.
 */
typedef enum
{
  STD_OK       = 0x00U,
  STD_ERROR    = 0x01U,
  STD_BUSY     = 0x02U,
  STD_TIMEOUT  = 0x03U
}GPIO_StatusTypeDef_t;

/**
 * @brief GPIO register structure definition.
 */
typedef struct
{
	volatile unsigned int CRL;
	volatile unsigned int CRH;
	volatile unsigned int IDR;
	volatile unsigned int ODR;
	volatile unsigned int BSRR;
	volatile unsigned int BRR;
	volatile unsigned int LCKR;
}GPIO_typedef_t;


typedef enum
{
	OUTPUT_PUSH_PULL = 0,
	OUTPUT_OPEN_DRAIN,
	AF_OUTPUT_PUSH_PULL,
	AF_OUTPUT_OPEN_DRAIN,
	INPUT_ANALOG,
	INPUT_FLOATING,
	INPUT_PULLUP,
	INPUT_PULLDOWN
}GPIO_MODE;


/* Enum for function remap */
typedef enum {
    AFIO_REMAP_NONE = 0,
    AFIO_REMAP_USART1,
    AFIO_REMAP_USART2,
    AFIO_REMAP_USART3_FULL,
    AFIO_REMAP_SPI1,
    AFIO_REMAP_I2C1,
    AFIO_REMAP_TIM2_FULL,
    AFIO_REMAP_TIM3_FULL,
    AFIO_REMAP_TIM4,
    AFIO_REMAP_CAN_PB89,
    AFIO_REMAP_DISABLE_JTAG_KEEP_SWD
} AFIO_Remap_t;

typedef struct
{
  uint32_t Pin;
  uint32_t Mode;
  uint32_t Pull;
  uint32_t Speed;
} GPIO_InitTypeDef_t;

typedef struct
{
	volatile unsigned int EVCR;
	volatile unsigned int MAPR;
	volatile unsigned int EXTICR1;
	volatile unsigned int EXTICR2;
	volatile unsigned int EXTICR3;
	volatile unsigned int EXTICR4;
	volatile unsigned int MAPR2;
}AFIO_typedef_t;

/***********************************************************************************************************************
 *                                                  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

void GPIO_Init_t(GPIO_typedef_t *GPIOx, GPIO_InitTypeDef_t * GPIO_Init);
void GPIO_WritePin_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin, GPIO_PinState_t State);
void GPIO_Toggle_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin);
GPIO_PinState_t GPIO_ReadPin_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin);
GPIO_StatusTypeDef_t GPIO_LockPin_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin);
void AFIO_RemapConfig(AFIO_Remap_t remap);

#ifdef __cplusplus
}
#endif

#endif /* LIB_GPIO_INC_GPIOX_IP_H_ */

/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_rs485.h
 *
 * Description:  HAL RS485 Driver — Data types, register structures, bitmasks,
 *               and API prototypes for RS485 communication wrapper over UART on STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          10/08/2026          Initial release for STM32F103xB RS485 Module
 *
 **********************************************************************************************************************/

#ifndef HAL_RS485_H_
#define HAL_RS485_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "hal_uart.h"

/***********************************************************************************************************************
 *                                          HEADER FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                               MACROS / DEFINES
 **********************************************************************************************************************/

#define RS485_DEFAULT_TIMEOUT        (1000U) /*!< Default timeout in milliseconds */

/***********************************************************************************************************************
 *                                       TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief RS485 DE/RE Pin Active Level Enum
 */
typedef enum {
    RS485_DE_ACTIVE_HIGH = 0U, /*!< High level = Transmit, Low level = Receive */
    RS485_DE_ACTIVE_LOW  = 1U  /*!< Low level = Transmit, High level = Receive */
} RS485_DE_Polarity_t;

/**
 * @brief RS485 GPIO Pin Configuration Struct
 */
typedef struct {
    void               *GPIOx;   /*!< GPIO Port Base Address (e.g., GPIOA, GPIOB) */
    uint16_t            GPIO_Pin;/*!< GPIO Pin Mask (e.g., GPIO_PIN_8) */
} RS485_PinConfig_t;

/**
 * @brief RS485 Peripheral Handle Structure
 */
typedef struct {
    UARTx_HandleTypeDef *huart;        /*!< Pointer to underlying UART handle */
    RS485_PinConfig_t    DE_Pin;       /*!< Driver Enable / Receiver Enable Pin */
    RS485_DE_Polarity_t  Polarity;     /*!< DE Pin Polarity logic */
    void (*SetPinFunc)(void *GPIOx, uint16_t Pin, bool Level); /*!< Function pointer to set GPIO output level */
} RS485_HandleTypeDef;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

HALx_StatusTypeDef RS485_Init(RS485_HandleTypeDef *hrs485, UARTx_HandleTypeDef *huart, RS485_PinConfig_t dePin, RS485_DE_Polarity_t polarity, void (*SetPinFunc)(void*, uint16_t, bool));
void               RS485_SetMode_TX(RS485_HandleTypeDef *hrs485);
void               RS485_SetMode_RX(RS485_HandleTypeDef *hrs485);
HALx_StatusTypeDef RS485_Transmit(RS485_HandleTypeDef *hrs485, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HALx_StatusTypeDef RS485_Receive(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HALx_StatusTypeDef RS485_Transmit_IT(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef RS485_Receive_IT(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef RS485_Transmit_DMA(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef RS485_Receive_DMA(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size);
void               RS485_TxCpltCallback(RS485_HandleTypeDef *hrs485);

#ifdef __cplusplus
}
#endif

#endif /* HAL_RS485_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_lin.h
 *
 * Description:  HAL LIN Driver — Data types, register structures, bitmasks,
 *               and API prototypes for LIN (Local Interconnect Network) module on STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          10/08/2026          Initial release for STM32F103xB LIN Module
 *
 **********************************************************************************************************************/

#ifndef HAL_LIN_H_
#define HAL_LIN_H_

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

#define USARTx_CR2_LBDL_Pos        (5U)
#define USARTx_CR2_LBDL            (1U << USARTx_CR2_LBDL_Pos)  /*!< 0: 10-bit break, 1: 11-bit break detection */

#define USARTx_CR2_LBDIE_Pos       (6U)
#define USARTx_CR2_LBDIE           (1U << USARTx_CR2_LBDIE_Pos) /*!< LIN Break Detection Interrupt Enable */

#define USARTx_CR2_LINEN_Pos       (14U)
#define USARTx_CR2_LINEN           (1U << USARTx_CR2_LINEN_Pos) /*!< LIN Mode Enable */

#define USARTx_CR1_SBK_Pos         (0U)
#define USARTx_CR1_SBK             (1U << USARTx_CR1_SBK_Pos)   /*!< Send Break Character */

#define USARTx_SR_LBD_Pos          (8U)
#define USARTx_SR_LBD              (1U << USARTx_SR_LBD_Pos)    /*!< LIN Break Detection Flag */

#define LIN_SYNC_BYTE              (0x55U)                       /*!< Fixed LIN Sync Byte Pattern */

#define LIN_DEFAULT_TIMEOUT        (1000U)                       /*!< Default transmission timeout in ms */

/***********************************************************************************************************************
 *                                       TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief LIN Break Frame Length Selection Enum
 */
typedef enum {
    LIN_BreakLength_10Bit = 0x00000000U,
    LIN_BreakLength_11Bit = USARTx_CR2_LBDL
} LIN_BreakLength_t;

/**
 * @brief LIN Checksum Method Enumeration
 */
typedef enum {
    LIN_CHECKSUM_CLASSIC  = 0U, /*!< LIN 1.3: Checksum calculated over Data Payload only */
    LIN_CHECKSUM_ENHANCED = 1U  /*!< LIN 2.x: Checksum calculated over PID + Data Payload */
} LIN_ChecksumType_t;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

HALx_StatusTypeDef LINx_Init(UARTx_HandleTypeDef *huart, LIN_BreakLength_t breakLength);
void               LINx_SendBreak(UARTx_HandleTypeDef *huart);
uint8_t            LINx_CalculatePID(uint8_t id);
uint8_t            LINx_CalculateChecksum(uint8_t pid, const uint8_t *pData, uint8_t len, LIN_ChecksumType_t type);
HALx_StatusTypeDef LINx_SendHeader(UARTx_HandleTypeDef *huart, uint8_t id);
HALx_StatusTypeDef LINx_SendResponse(UARTx_HandleTypeDef *huart, uint8_t id, const uint8_t *pData, uint8_t len, LIN_ChecksumType_t type);
HALx_StatusTypeDef LINx_SendFrame(UARTx_HandleTypeDef *huart, uint8_t id, const uint8_t *pData, uint8_t len, LIN_ChecksumType_t type);
void               LINx_IRQHandler(UARTx_HandleTypeDef *huart);
void               LINx_BreakCallback(UARTx_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* HAL_LIN_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
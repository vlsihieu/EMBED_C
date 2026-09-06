/***********************************************************************************************************************
 * @file    hal_soft_spi.h
 * @brief   Software (Bit-Banging) SPI Driver Header for STM32F103xB.
 * @project STM32F103_BareMetal_Drivers
 * @compiler GCC
 **********************************************************************************************************************/

#ifndef HAL_SOFT_SPI_H
#define HAL_SOFT_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "define.h"

/***********************************************************************************************************************
 *                                           MACROS & CONSTANTS
 **********************************************************************************************************************/

/** @defgroup SoftSPI_Clock_Polarity SPI Clock Polarity (CPOL) */
#define SOFT_SPI_CPOL_LOW                    (0x00U) /*!< SCK idles LOW  */
#define SOFT_SPI_CPOL_HIGH                   (0x01U) /*!< SCK idles HIGH */

/** @defgroup SoftSPI_Clock_Phase SPI Clock Phase (CPHA) */
#define SOFT_SPI_CPHA_1EDGE                  (0x00U) /*!< Sample on first edge  */
#define SOFT_SPI_CPHA_2EDGE                  (0x01U) /*!< Sample on second edge */

/** @defgroup SoftSPI_Bit_Order SPI First Bit Direction */
#define SOFT_SPI_FIRSTBIT_MSB                (0x00U) /*!< MSB transmitted first */
#define SOFT_SPI_FIRSTBIT_LSB                (0x01U) /*!< LSB transmitted first */

/** @defgroup SoftSPI_Driver_State Software SPI Driver State */
#define SOFT_SPI_STATE_RESET                 (0x00U)
#define SOFT_SPI_STATE_READY                 (0x01U)
#define SOFT_SPI_STATE_BUSY                  (0x02U)

/** @defgroup SoftSPI_GPIO_Mode STM32F1 GPIO CRL/CRH nibble values */
#define SOFT_SPI_GPIO_OUTPUT_PP_10MHZ         (0x01U)
#define SOFT_SPI_GPIO_INPUT_FLOATING          (0x04U)

/** @defgroup SoftSPI_Pin_Validation */
#define IS_SOFT_SPI_PIN(PIN)                 ((PIN) < 16U)
#define IS_SOFT_SPI_CPOL(CPOL)               (((CPOL) == SOFT_SPI_CPOL_LOW) || \
                                               ((CPOL) == SOFT_SPI_CPOL_HIGH))
#define IS_SOFT_SPI_CPHA(CPHA)               (((CPHA) == SOFT_SPI_CPHA_1EDGE) || \
                                               ((CPHA) == SOFT_SPI_CPHA_2EDGE))
#define IS_SOFT_SPI_FIRSTBIT(FIRSTBIT)        (((FIRSTBIT) == SOFT_SPI_FIRSTBIT_MSB) || \
                                               ((FIRSTBIT) == SOFT_SPI_FIRSTBIT_LSB))

/***********************************************************************************************************************
 *                                           DATATYPES & STRUCTURES
 **********************************************************************************************************************/

/**
 * @brief  Functional State definition (ENABLE / DISABLE).
 * @req    FR-SPI-011
 * @req    FR-SPI-027
 */
typedef enum
{
    DISABLE = 0U,
    ENABLE  = 1U
} FunctionalState;

/**
 * @brief   Software SPI GPIO pin configuration.
 *
 * @note    GPIO peripheral clocks shall be enabled before calling SoftSPI_Init().
 * @note    Pin fields are pin indexes 0..15, not bit masks.
 */
typedef struct
{
    GPIOx_typedef_t *SckPort;      /*!< GPIO port used for SCK.  */
    uint16_t         SckPin;       /*!< SCK pin index: 0..15.    */
    GPIOx_typedef_t *MosiPort;     /*!< GPIO port used for MOSI. */
    uint16_t         MosiPin;      /*!< MOSI pin index: 0..15.   */
    GPIOx_typedef_t *MisoPort;     /*!< GPIO port used for MISO. */
    uint16_t         MisoPin;      /*!< MISO pin index: 0..15.   */
    GPIOx_typedef_t *CsPort;       /*!< GPIO port used for CS.   */
    uint16_t         CsPin;        /*!< CS pin index: 0..15.     */
} SoftSPI_PinConfigTypeDef;

/**
 * @brief   Software SPI initialization configuration.
 *
 * @note    DelayUs is implemented by a software NOP loop. Its real time depends on CPU clock,
 *          compiler optimization and instruction timing; it is therefore an approximate delay.
 */
typedef struct
{
    SoftSPI_PinConfigTypeDef Pins;      /*!< GPIO pin mapping. */
    uint8_t                  CPOL;      /*!< SOFT_SPI_CPOL_LOW / SOFT_SPI_CPOL_HIGH. */
    uint8_t                  CPHA;      /*!< SOFT_SPI_CPHA_1EDGE / SOFT_SPI_CPHA_2EDGE. */
    uint8_t                  FirstBit;  /*!< SOFT_SPI_FIRSTBIT_MSB / SOFT_SPI_FIRSTBIT_LSB. */
    uint32_t                 DelayUs;   /*!< Approximate software delay count; 0 = maximum speed. */
} SoftSPI_InitTypeDef;

/**
 * @brief   Software SPI handle.
 */
typedef struct
{
    SoftSPI_InitTypeDef Init;           /*!< Software SPI configuration. */
    uint8_t             State;          /*!< SOFT_SPI_STATE_xxx. */
} SoftSPI_HandleTypeDef;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/**
 * @brief   Initializes Software SPI GPIO pins and default line levels.
 * @param   hspi: Pointer to Software SPI handle.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_Init(SoftSPI_HandleTypeDef *hspi);

/**
 * @brief   Controls the active-low CS/NSS line.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   NewState: ENABLE selects the slave (CS LOW), DISABLE releases it (CS HIGH).
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_ChipSelect(SoftSPI_HandleTypeDef *hspi, FunctionalState NewState);

/**
 * @brief   Transmits and receives one byte through Software SPI.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   txData: Byte to transmit.
 * @retval  uint8_t: Byte sampled from MISO.
 */
uint8_t SoftSPI_TransmitReceiveByte(SoftSPI_HandleTypeDef *hspi, uint8_t txData);

/**
 * @brief   Transmits a data buffer through Software SPI.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   pData: Pointer to transmit buffer.
 * @param   Size: Number of bytes to transmit.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_Transmit(SoftSPI_HandleTypeDef *hspi,
                                    const uint8_t *pData,
                                    uint16_t Size);

/**
 * @brief   Receives a data buffer through Software SPI by transmitting dummy 0xFF bytes.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   pData: Pointer to receive buffer.
 * @param   Size: Number of bytes to receive.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_Receive(SoftSPI_HandleTypeDef *hspi,
                                   uint8_t *pData,
                                   uint16_t Size);

/**
 * @brief   Performs a full-duplex buffer transfer.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   pTxData: Pointer to transmit buffer.
 * @param   pRxData: Pointer to receive buffer.
 * @param   Size: Number of bytes to exchange.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_TransmitReceive(SoftSPI_HandleTypeDef *hspi,
                                           const uint8_t *pTxData,
                                           uint8_t *pRxData,
                                           uint16_t Size);

#ifdef __cplusplus
}
#endif

#endif /* HAL_SOFT_SPI_H */

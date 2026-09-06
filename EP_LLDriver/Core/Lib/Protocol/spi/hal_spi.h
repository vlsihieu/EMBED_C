/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: hal_spi.h
 *
 * Description: HAL SPI Driver Header - Core definitions and prototypes for
 *              Serial Peripheral Interface peripherals on STM32F103xB devices.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           22/08/2026          Initial release for STM32F103xB SPI Module
 *
 **********************************************************************************************************************/

#ifndef HAL_SPI_H
#define HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                  INCLUDES
 **********************************************************************************************************************/

// #include "stm32f103xb.h"
#include "hal_clock.h"
#include "define.h"
#include <stdint.h>
#include <stddef.h>

/***********************************************************************************************************************
 *                                                    MACROS
 **********************************************************************************************************************/


/**
 * @brief  SPI CR1 register bit-position definitions.
 * @req    HWR-SPI-003
 * @req    HWR-SPI-004
 * @req    HWR-SPI-005
 * @req    HWR-SPI-006
 * @req    HWR-SPI-007
 * @req    HWR-SPI-008
 * @req    HWR-SPI-009
 * @req    HWR-SPI-010
 * @req    HWR-SPI-011
 * @req    HWR-SPI-012
 */
#define POSITION_SPIx_CR1_CPHA             (0U)
#define POSITION_SPIx_CR1_CPOL             (1U)
#define POSITION_SPIx_CR1_MSTR             (2U)
#define POSITION_SPIx_CR1_BR               (3U)
#define POSITION_SPIx_CR1_SPE              (6U)
#define POSITION_SPIx_CR1_LSBFIRST         (7U)
#define POSITION_SPIx_CR1_SSI              (8U)
#define POSITION_SPIx_CR1_SSM              (9U)
#define POSITION_SPIx_CR1_RXONLY           (10U)
#define POSITION_SPIx_CR1_DFF              (11U)
#define POSITION_SPIx_CR1_CRCNEXT          (12U)
#define POSITION_SPIx_CR1_CRCEN            (13U)
#define POSITION_SPIx_CR1_BIDIOE           (14U)
#define POSITION_SPIx_CR1_BIDIMODE         (15U)

/**
 * @brief  SPI CR2 register bit-position definitions.
 * @req    HWR-SPI-009
 * @req    HWR-SPI-013
 * @req    HWR-SPI-014
 */
#define POSITION_SPIx_CR2_RXDMAEN          (0U)
#define POSITION_SPIx_CR2_TXDMAEN          (1U)
#define POSITION_SPIx_CR2_SSOE             (2U)
#define POSITION_SPIx_CR2_ERRIE            (5U)
#define POSITION_SPIx_CR2_RXNEIE           (6U)
#define POSITION_SPIx_CR2_TXEIE            (7U)

/**
 * @brief  SPI SR register bit-position definitions.
 * @req    HWR-SPI-015
 * @req    HWR-SPI-016
 */
#define POSITION_SPIx_SR_RXNE              (0U)
#define POSITION_SPIx_SR_TXE               (1U)
#define POSITION_SPIx_SR_CHSIDE            (2U)
#define POSITION_SPIx_SR_UDR               (3U)
#define POSITION_SPIx_SR_CRCERR            (4U)
#define POSITION_SPIx_SR_MODF              (5U)
#define POSITION_SPIx_SR_OVR               (6U)
#define POSITION_SPIx_SR_BSY               (7U)

/**
 * @brief  SPI register bit masks used by control and transfer logic.
 * @req    HWR-SPI-007
 * @req    HWR-SPI-015
 */
#define SPIx_CR1_SPE_MASK                  (0x01U << POSITION_SPIx_CR1_SPE)
#define SPIx_SR_RXNE_MASK                  (0x01U << POSITION_SPIx_SR_RXNE)
#define SPIx_SR_TXE_MASK                   (0x01U << POSITION_SPIx_SR_TXE)
#define SPIx_SR_BSY_MASK                   (0x01U << POSITION_SPIx_SR_BSY)

/**
 * @brief  SPI peripheral instance validation macro.
 * @req    HWR-SPI-002
 * @req    FR-SPI-024
 * @req    NFR-SPI-006
 */
#define IS_SPI_ALL_INSTANCE(INSTANCE)      (((INSTANCE) == SPI1) || ((INSTANCE) == SPI2))

/***********************************************************************************************************************
 *                                       TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief  Internal SPI flag state used by polling helper.
 */
typedef enum
{
    SPIx_FLAG_RESET = 0U,
    SPIx_FLAG_SET   = 1U
} SPIx_FlagStatus_t;

/**
 * @brief  Functional State definition (ENABLE / DISABLE).
 * @req    FR-SPI-011
 * @req    FR-SPI-027
 */
typedef enum
{
    FunctionalState_DISABLE = 0U,
    FunctionalState_ENABLE  = 1U
} FunctionalState;

/**
 * @brief  SPI Mode (Master / Slave).
 * @req    HWR-SPI-005
 * @req    FR-SPI-002
 */
typedef enum
{
    SPIx_Mode_SLAVE                  = 0x00000000U,
    SPIx_Mode_MASTER                 = (0x01U << 2U)
} SPIx_Mode_t;

/**
 * @brief  SPI Communication Direction Options.
 * @req    HWR-SPI-010
 * @req    FR-SPI-003
 */
typedef enum
{
    SPIx_Direction_2LINES            = 0x00000000U,
    SPIx_Direction_2LINES_RXONLY     = (0x01U << 10U),
    SPIx_Direction_1LINE_RX          = (0x01U << 15U),
    SPIx_Direction_1LINE_TX          = ((0x01U << 15U) | (0x01U << 14U))
} SPIx_Direction_t;

/**
 * @brief  SPI Data Frame Format (8-bit or 16-bit).
 * @req    HWR-SPI-011
 * @req    FR-SPI-004
 */
typedef enum
{
    SPIx_DataSize_8BIT               = 0x00000000U,
    SPIx_DataSize_16BIT              = (0x01U << 11U)
} SPIx_DataSize_t;

/**
 * @brief  SPI Clock Polarity (CPOL).
 * @req    HWR-SPI-004
 * @req    FR-SPI-005
 */
typedef enum
{
    SPIx_CLkPolarity_LOW             = 0x00000000U,
    SPIx_CLkPolarity_HIGH            = (0x01U << 1U)
} SPIx_CLKPolarity_t;

/**
 * @brief  SPI Clock Phase (CPHA).
 * @req    HWR-SPI-003
 * @req    FR-SPI-006
 */
typedef enum
{
    SPIx_CLKPhase_1EDGE              = 0x00000000U,
    SPIx_CLKPhase_2EDGE              = (0x01U << 0U)
} SPIx_CLKPhase_t;

/**
 * @brief  SPI Slave Select Management Mode.
 * @req    HWR-SPI-009
 * @req    FR-SPI-007
 */
typedef enum
{
    SPIx_NSS_HARD_INPUT              = 0x00000000U,
    SPIx_NSS_HARD_OUTPUT             = (0x01U << 2U),
    SPIx_NSS_SOFT                    = ((0x01U << 9U) | (0x01U << 8U))
} SPIx_NSS_t;

/**
 * @brief  SPI BaudRate Prescaler.
 * @req    HWR-SPI-006
 * @req    FR-SPI-008
 */
typedef enum
{
    SPIx_BauRatePresCaler_2          = (0x00U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_4          = (0x01U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_8          = (0x02U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_16         = (0x03U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_32         = (0x04U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_64         = (0x05U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_128        = (0x06U << POSITION_SPIx_CR1_BR),
    SPIx_BauRatePresCaler_256        = (0x07U << POSITION_SPIx_CR1_BR)
} SPIx_BaudRatePrescaler_t;

/**
 * @brief  SPI Bit Order Transmit.
 * @req    HWR-SPI-008
 * @req    FR-SPI-009
 */
typedef enum
{
    SPIx_FirstBit_MSB                = 0x00000000U,
    SPIx_FirstBit_LSB                = (0x01U << 7U)
} SPIx_FirstBit_t;

/**
 * @brief  SPI Status Return Definitions.
 * @req    FR-SPI-025
 * @req    NFR-SPI-005
 */
typedef enum
{
    SPIx_STATUS_OK                   = 0x00U,
    SPIx_STATUS_ERROR                = 0x01U,
    SPIx_STATUS_BUSY                 = 0x02U,
    SPIx_STATUS_TIMEOUT              = 0x03U
} SPIx_StatusTypeDef_t;

/**
 * @brief  SPI Initialization Structure definition.
 * @req    FR-SPI-001, FR-SPI-002, FR-SPI-003, FR-SPI-004
 * @req    FR-SPI-005, FR-SPI-006, FR-SPI-007, FR-SPI-008
 * @req    FR-SPI-009, FR-SPI-026
 */
typedef struct
{
    SPIx_Mode_t              Mode;               /*!< Specifies the SPI operating mode */
    SPIx_Direction_t         Direction;          /*!< Specifies the SPI bidirectional data mode */
    SPIx_DataSize_t          DataSize;           /*!< Specifies the SPI data size */
    SPIx_CLKPolarity_t       CLKPolarity;        /*!< Specifies the serial clock steady state */
    SPIx_CLKPhase_t          CLKPhase;           /*!< Specifies the clock active edge for bit capture */
    SPIx_NSS_t               NSS;                /*!< Specifies whether the NSS signal is managed by HW or SW */
    SPIx_BaudRatePrescaler_t BaudRatePrescaler; /*!< Specifies the Baud Rate prescaler value */
    SPIx_FirstBit_t          FirstBit;           /*!< Specifies whether data transfers start from MSB or LSB */
} SPIx_InitTypeDef_t;

/***********************************************************************************************************************
 *                                           GLOBAL VARIABLE DECLARATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                FUNCTION PROTOTYPES
 **********************************************************************************************************************/


void SPIx_Init_t(SPIx_TypeDef *SPIx, SPIx_InitTypeDef_t *SPI_Init);
void SPIx_DeInit_t(SPIx_TypeDef *SPIx);
void SPIx_Cmd_t(SPIx_TypeDef *SPIx, FunctionalState NewState);
HALx_StatusTypeDef SPIx_Transmit_t(SPIx_TypeDef *SPIx, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HALx_StatusTypeDef SPIx_Receive_t(SPIx_TypeDef *SPIx, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HALx_StatusTypeDef SPIx_TransmitReceive_t(SPIx_TypeDef *SPIx,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);
void SPIx_EXTI_IRQHandler_t(SPIx_TypeDef *SPIx);
void SPIx_TxCpltCallback_t(SPIx_TypeDef *SPIx);
void SPIx_RxCpltCallback_t(SPIx_TypeDef *SPIx);

#ifdef __cplusplus
}
#endif

#endif /* HAL_SPI_H */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/

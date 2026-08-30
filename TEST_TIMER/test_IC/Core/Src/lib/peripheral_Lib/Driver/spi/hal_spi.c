/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: hal_spi.c
 *
 * Description: HAL SPI Driver — Core implementation for Serial Peripheral Interface
 *              peripherals on STM32F103xB devices.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           22/08/2026          Initial release for STM32F103xB SPI Module
 *
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                  INCLUDES
 **********************************************************************************************************************/

#include "../../../peripheral_Lib/Driver/spi/hal_spi.h"

/***********************************************************************************************************************
 *                                            SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                               LOCAL MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                     LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                               LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                               LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                              GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                           LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static HALx_StatusTypeDef SPI_WaitFlagTimeout(SPIx_TypeDef *SPIx, uint32_t FlagBit, SPIx_FlagStatus_t Status, uint32_t Timeout);

/***********************************************************************************************************************
 *                                               LOCAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief   Waits for a specific SPI flag to reach the designated state or until a timeout occurs.
 * @param   SPIx: Pointer to SPI peripheral.
 * @param   FlagBit: Flag mask to check in the SR register.
 * @param   Status: Desired flag status (SET or RESET).
 * @param   Timeout: Timeout duration in loop cycles.
 * @retval  HALx_StatusTypeDef: Status of the operation (OK or TIMEOUT).
 */
static HALx_StatusTypeDef SPI_WaitFlagTimeout(SPIx_TypeDef *SPIx, uint32_t FlagBit, SPIx_FlagStatus_t Status, uint32_t Timeout)
{
    uint32_t tickstart = Timeout;

    /* @req HWR-SPI-015, FR-SPI-018, NFR-SPI-004 */
    while (tickstart > 0U)
    {
        /* @req HWR-SPI-015 */
        if (((SPIx->SR & FlagBit) ? SPIx_FLAG_SET : SPIx_FLAG_RESET) == Status)
        {
            /* @req FR-SPI-018, NFR-SPI-005 */
            return STD_OK;
        }
        /* @req FR-SPI-018, NFR-SPI-004 */
        tickstart--;
    }
    /* @req FR-SPI-018, NFR-SPI-005 */
    return SPIx_STATUS_TIMEOUT;
}

/***********************************************************************************************************************
 *                                              GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief   Initializes the specified SPI peripheral.
 * @param   SPIx: Pointer to SPI peripheral.
 * @param   SPI_Init: Pointer to SPI initialization structure.
 * @retval  None
 */
void SPIx_Init_t(SPIx_TypeDef *SPIx, SPIx_InitTypeDef_t *SPI_Init)
{
    uint32_t tmpreg_cr1 = 0U;
    uint32_t tmpreg_cr2 = 0U;

    /* @req HWR-SPI-002, FR-SPI-024, NFR-SPI-006 */
    assert_param(IS_SPI_ALL_INSTANCE(SPIx));

    /* @req HWR-SPI-017, FR-SPI-001 */
    if (SPIx == SPI1x)
    {
        /* @req HWR-SPI-002, HWR-SPI-017, FR-SPI-001 */
        HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_SPI1);
    }
    else if (SPIx == SPI2x)
    {
        /* @req HWR-SPI-002, HWR-SPI-017, FR-SPI-001 */
        HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_SPI2);
    }

    /* @req HWR-SPI-007, FR-SPI-001 */
    SPIx->CR1 &= ~SPIx_CR1_SPE_MASK;

    /* ---------------- Configure SPIx CR1 ---------------- */
    tmpreg_cr1 = SPIx->CR1;

    /* @req HWR-SPI-003, HWR-SPI-004, HWR-SPI-005, HWR-SPI-006
     * @req HWR-SPI-008, HWR-SPI-009, HWR-SPI-010, HWR-SPI-011
     * @req FR-SPI-002, FR-SPI-003, FR-SPI-004, FR-SPI-005
     * @req FR-SPI-006, FR-SPI-008, FR-SPI-009
     */
    tmpreg_cr1 &= ~((0x01U << POSITION_SPIx_CR1_CPHA)     |
                    (0x01U << POSITION_SPIx_CR1_CPOL)     |
                    (0x01U << POSITION_SPIx_CR1_MSTR)     |
                    (0x07U << POSITION_SPIx_CR1_BR)       |
                    (0x01U << POSITION_SPIx_CR1_LSBFIRST) |
                    (0x01U << POSITION_SPIx_CR1_SSI)      |
                    (0x01U << POSITION_SPIx_CR1_SSM)      |
                    (0x01U << POSITION_SPIx_CR1_RXONLY)   |
                    (0x01U << POSITION_SPIx_CR1_DFF)      |
                    (0x01U << POSITION_SPIx_CR1_BIDIOE)   |
                    (0x01U << POSITION_SPIx_CR1_BIDIMODE));

    /* @req HWR-SPI-003, HWR-SPI-004, HWR-SPI-005, HWR-SPI-006
     * @req HWR-SPI-008, HWR-SPI-010, HWR-SPI-011
     * @req FR-SPI-002, FR-SPI-003, FR-SPI-004, FR-SPI-005
     * @req FR-SPI-006, FR-SPI-008, FR-SPI-009, FR-SPI-026
     */
    tmpreg_cr1 |= (uint32_t)(SPI_Init->Mode             |
                             SPI_Init->Direction        |
                             SPI_Init->DataSize         |
                             SPI_Init->CLKPolarity      |
                             SPI_Init->CLKPhase         |
                             SPI_Init->BaudRatePrescaler|
                             SPI_Init->FirstBit);

    /* @req HWR-SPI-009, FR-SPI-007 */
    if (SPI_Init->NSS == SPIx_NSS_SOFT)
    {
        /* @req HWR-SPI-009, FR-SPI-007 */
        tmpreg_cr1 |= (0x01U << POSITION_SPIx_CR1_SSM) | (0x01U << POSITION_SPIx_CR1_SSI);
    }

    SPIx->CR1 = tmpreg_cr1;

    /* ---------------- Configure SPIx CR2 ---------------- */
    tmpreg_cr2 = SPIx->CR2;
    tmpreg_cr2 &= ~(0x01U << POSITION_SPIx_CR2_SSOE);

    /* @req HWR-SPI-009, FR-SPI-007 */
    if (SPI_Init->NSS == SPIx_NSS_HARD_OUTPUT)
    {
        /* @req HWR-SPI-009, FR-SPI-007 */
        tmpreg_cr2 |= (0x01U << POSITION_SPIx_CR2_SSOE);
    }

    SPIx->CR2 = tmpreg_cr2;
}

/**
 * @brief   Resets the configuration of the selected SPI peripheral to its default state.
 * @param   SPIx: Pointer to SPI peripheral.
 * @retval  None
 */
void SPIx_DeInit_t(SPIx_TypeDef *SPIx)
{
    /* @req HWR-SPI-002, FR-SPI-024, NFR-SPI-006 */
    assert_param(IS_SPI_ALL_INSTANCE(SPIx));

    /* @req HWR-SPI-007, FR-SPI-010 */
    SPIx->CR1 &= ~SPIx_CR1_SPE_MASK;

    /* @req FR-SPI-010 */
    SPIx->CR1 = 0x0000U;
    SPIx->CR2 = 0x0000U;
    SPIx->I2SCFGR = 0x0000U;
}

/**
 * @brief   Enables or disables the specified SPI peripheral.
 * @param   SPIx: Pointer to SPI peripheral.
 * @param   NewState: ENABLE or DISABLE.
 * @retval  None
 */
void SPIx_Cmd_t(SPIx_TypeDef *SPIx, FunctionalState NewState)
{
    /* @req HWR-SPI-002, FR-SPI-024, NFR-SPI-006 */
    assert_param(IS_SPI_ALL_INSTANCE(SPIx));

    /* @req FR-SPI-011, FR-SPI-027 */
    if (NewState != FunctionalState_DISABLE)
    {
        /* @req HWR-SPI-007, FR-SPI-011 */
        SPIx->CR1 |= SPIx_CR1_SPE_MASK;
    }
    else
    {
        /* @req HWR-SPI-007, FR-SPI-011 */
        SPIx->CR1 &= ~SPIx_CR1_SPE_MASK;
    }
}

/**
 * @brief   Transmits an amount of data through the specified SPI peripheral in blocking mode.
 * @param   SPIx: Pointer to SPI peripheral.
 * @param   pData: Pointer to transmit data buffer.
 * @param   Size: Amount of data elements to send.
 * @param   Timeout: Timeout duration.
 * @retval  HALx_StatusTypeDef: Status of transmission.
 */
HALx_StatusTypeDef SPIx_Transmit_t(SPIx_TypeDef *SPIx, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    HALx_StatusTypeDef status = STD_OK;
    uint16_t count = Size;

    /* @req FR-SPI-012, FR-SPI-013, NFR-SPI-005 */
    if ((pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    while (count > 0U)
    {
        /* @req HWR-SPI-015, FR-SPI-012, FR-SPI-018, NFR-SPI-004 */
        status = SPI_WaitFlagTimeout(SPIx, SPIx_SR_TXE_MASK, SPIx_FLAG_SET, Timeout);
        if (status != STD_OK)
        {
            /* @req FR-SPI-013, NFR-SPI-005 */
    return status;
        }

        /* @req HWR-SPI-011, FR-SPI-012 */
        if ((SPIx->CR1 & (0x01U << POSITION_SPIx_CR1_DFF)) != 0U)
        {
            SPIx->DR = *((uint16_t *)pData);
            pData += 2U;
            count--;
        }
        else
        {
            SPIx->DR = *pData;
            pData++;
            count--;
        }
    }

    /* @req HWR-SPI-015, FR-SPI-012, FR-SPI-018, NFR-SPI-004 */
    status = SPI_WaitFlagTimeout(SPIx, SPIx_SR_BSY_MASK, SPIx_FLAG_RESET, Timeout);

    return status;
}

/**
 * @brief   Receives an amount of data through the specified SPI peripheral in blocking mode.
 * @param   SPIx: Pointer to SPI peripheral.
 * @param   pData: Pointer to receive data buffer.
 * @param   Size: Amount of data elements to receive.
 * @param   Timeout: Timeout duration.
 * @retval  HALx_StatusTypeDef: Status of reception.
 */
HALx_StatusTypeDef SPIx_Receive_t(SPIx_TypeDef *SPIx, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    HALx_StatusTypeDef status = STD_OK;
    uint16_t count = Size;

    /* @req FR-SPI-014, FR-SPI-015, NFR-SPI-005 */
    if ((pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    while (count > 0U)
    {
        /* @req HWR-SPI-015, FR-SPI-014, FR-SPI-018, NFR-SPI-004 */
        status = SPI_WaitFlagTimeout(SPIx, SPIx_SR_TXE_MASK, SPIx_FLAG_SET, Timeout);
        if (status != STD_OK)
        {
            return status;
        }
        /* @req FR-SPI-014 */
        SPIx->DR = 0xFFU;

        /* @req HWR-SPI-015, FR-SPI-014, FR-SPI-018, NFR-SPI-004 */
        status = SPI_WaitFlagTimeout(SPIx, SPIx_SR_RXNE_MASK, SPIx_FLAG_SET, Timeout);
        if (status != STD_OK)
        {
            return status;
        }

        /* @req HWR-SPI-011, FR-SPI-014 */
        if ((SPIx->CR1 & (0x01U << POSITION_SPIx_CR1_DFF)) != 0U)
        {
            *((uint16_t *)pData) = (uint16_t)SPIx->DR;
            pData += 2U;
            count--;
        }
        else
        {
            *pData = (uint8_t)SPIx->DR;
            pData++;
            count--;
        }
    }

    /* @req FR-SPI-015, NFR-SPI-005 */
    return STD_OK;
}

/**
 * @brief   Transmits and receives an amount of data through the specified SPI peripheral in blocking mode.
 * @param   SPIx: Pointer to SPI peripheral.
 * @param   pTxData: Pointer to transmission data buffer.
 * @param   pRxData: Pointer to reception data buffer.
 * @param   Size: Amount of data elements to send and receive.
 * @param   Timeout: Timeout duration.
 * @retval  HALx_StatusTypeDef: Status of operation.
 */
HALx_StatusTypeDef SPIx_TransmitReceive_t(SPIx_TypeDef *SPIx, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    HALx_StatusTypeDef status = STD_OK;
    uint16_t count = Size;

    /* @req FR-SPI-016, FR-SPI-017, NFR-SPI-005 */
    if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    while (count > 0U)
    {
        /* @req HWR-SPI-015, FR-SPI-016, FR-SPI-018, NFR-SPI-004 */
        status = SPI_WaitFlagTimeout(SPIx, SPIx_SR_TXE_MASK, SPIx_FLAG_SET, Timeout);
        if (status != STD_OK) return status;

        /* @req HWR-SPI-011, FR-SPI-016 */
        if ((SPIx->CR1 & (0x01U << POSITION_SPIx_CR1_DFF)) != 0U)
        {
            SPIx->DR = *((uint16_t *)pTxData);
            pTxData += 2U;
        }
        else
        {
            SPIx->DR = *pTxData;
            pTxData++;
        }

        /* @req HWR-SPI-015, FR-SPI-016, FR-SPI-018, NFR-SPI-004 */
        status = SPI_WaitFlagTimeout(SPIx, SPIx_SR_RXNE_MASK, SPIx_FLAG_SET, Timeout);
        if (status != STD_OK) return status;

        /* @req HWR-SPI-011, FR-SPI-016 */
        if ((SPIx->CR1 & (0x01U << POSITION_SPIx_CR1_DFF)) != 0U)
        {
            *((uint16_t *)pRxData) = (uint16_t)SPIx->DR;
            pRxData += 2U;
        }
        else
        {
            *pRxData = (uint8_t)SPIx->DR;
            pRxData++;
        }

        count--;
    }

    /* @req HWR-SPI-015, FR-SPI-016, FR-SPI-017, FR-SPI-018, NFR-SPI-004, NFR-SPI-005 */
    return SPI_WaitFlagTimeout(SPIx, SPIx_SR_BSY_MASK, SPIx_FLAG_RESET, Timeout);
}

/**
 * @brief   Handles interrupt requests for the specified SPI peripheral.
 * @param   SPIx: Pointer to SPI peripheral.
 * @retval  None
 */
void SPIx_EXTI_IRQHandler_t(SPIx_TypeDef *SPIx)
{
    /* @req HWR-SPI-014, HWR-SPI-015, FR-SPI-019 */
    uint32_t itsource = SPIx->CR2;
    uint32_t itflag   = SPIx->SR;

    /* @req HWR-SPI-014, HWR-SPI-015, FR-SPI-020, NFR-SPI-011 */
    if (((itflag & SPIx_SR_TXE_MASK) != 0U) && ((itsource & (0x01U << POSITION_SPIx_CR2_TXEIE)) != 0U))
    {
        /* @req FR-SPI-020, FR-SPI-022 */
        SPIx_TxCpltCallback_t(SPIx);
    }

    /* @req HWR-SPI-014, HWR-SPI-015, FR-SPI-021, NFR-SPI-011 */
    if (((itflag & SPIx_SR_RXNE_MASK) != 0U) && ((itsource & (0x01U << POSITION_SPIx_CR2_RXNEIE)) != 0U))
    {
        /* @req FR-SPI-021, FR-SPI-023 */
        SPIx_RxCpltCallback_t(SPIx);
    }
}

/**
 * @brief   Handles the SPI transmit-complete callback.
 * @param   SPIx: Pointer to SPI peripheral.
 * @retval  None
 */
void SPIx_TxCpltCallback_t(SPIx_TypeDef *SPIx)
{
    /* @req FR-SPI-020, FR-SPI-022, NFR-SPI-010 */
    (void)SPIx;
}

/**
 * @brief   Handles the SPI receive-complete callback.
 * @param   SPIx: Pointer to SPI peripheral.
 * @retval  None
 */
void SPIx_RxCpltCallback_t(SPIx_TypeDef *SPIx)
{
    /* @req FR-SPI-021, FR-SPI-023, NFR-SPI-010 */
    (void)SPIx;
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------- End Of File -----------------------------------------------------*/

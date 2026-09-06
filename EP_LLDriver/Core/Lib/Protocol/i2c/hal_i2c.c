/***********************************************************************************************************************
 * @file    hal_i2c.c
 * @brief   I2C Hardware Abstraction Layer (HAL) Driver Implementation.
 * @details Target MCU: STM32F103xB.
 *          This module provides bare-metal functional implementations for initializing, configuring,
 *          and executing data transfers (Polling, Interrupt, DMA) via the I2C peripheral.
 *
 * @version 1.0.0
 * @date    2026-09-03
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include "hal_i2c.h"

/***********************************************************************************************************************
 *                                            SOURCE FILE VERSION
 **********************************************************************************************************************/

#define HAL_I2C_SW_MAJOR_VERSION    (1U)
#define HAL_I2C_SW_MINOR_VERSION    (0U)
#define HAL_I2C_SW_PATCH_VERSION    (0U)

/***********************************************************************************************************************
 *                                           FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                              LOCAL MACROS
 **********************************************************************************************************************/

/** @defgroup I2C_Internal_States Internal Software States for I2C Driver
  * @{
  */
#define I2C_STATE_RESET             (0x00U)  /*!< Peripheral not initialized or disabled */
#define I2C_STATE_READY             (0x01U)  /*!< Peripheral initialized and ready for use */
#define I2C_STATE_BUSY_TX           (0x02U)  /*!< Transmission process ongoing            */
#define I2C_STATE_BUSY_RX           (0x03U)  /*!< Reception process ongoing               */
/**
  * @}
  */

/***********************************************************************************************************************
 *                                                 EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                            GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                        LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static HALx_StatusTypeDef I2C_WaitOnFlag(I2Cx_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout);
static void               I2C_SendAddress(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t ReadWrite);
static void               I2C_ClearADDR(I2Cx_HandleTypeDef *hi2c);

/***********************************************************************************************************************
 *                                            LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**
 * @brief  Waits until a specific register flag in SR1 reaches the target state or a timeout occurs.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  Flag: Bit flag to check in the SR1 register.
 * @param  Status: Expected state of the flag (SET or RESET).
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Flag reached the expected state before timing out.
 *         - STD_TIMEOUT: Timeout expired.
 */
static HALx_StatusTypeDef I2C_WaitOnFlag(I2Cx_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
    uint32_t count = Timeout * 1000U;

    while (((hi2c->Instance->SR1 & Flag) == Flag) != (Status == SET))
    {
        if (count == 0U)
        {
            return STD_TIMEOUT;
        }
        count--;
    }
    return STD_OK;
}

/**
 * @brief  Sends target slave address with read/write direction bit into Data Register (DR).
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave hardware address.
 * @param  ReadWrite: Transfer direction (0x00 for Write, 0x01 for Read).
 * @retval None
 */
static void I2C_SendAddress(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t ReadWrite)
{
    hi2c->Instance->DR = (uint8_t)((DevAddress & (uint8_t)(~0x01U)) | ReadWrite);
}

/**
 * @brief  Clears the ADDR flag according to STM32F1 hardware sequence (SR1 read followed by SR2 read).
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval None
 */
static void I2C_ClearADDR(I2Cx_HandleTypeDef *hi2c)
{
    volatile uint32_t dummyRead;
    dummyRead = hi2c->Instance->SR1;
    dummyRead = hi2c->Instance->SR2;
    (void)dummyRead;
}

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**
 * @brief  Initializes the I2C peripheral according to the specified parameters in hi2c.
 * @details Computes APB1 frequency, configures TRISE, CCR registers, sets OAR1 own address, and enables the peripheral.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Initialization successful.
 *         - STD_ERROR: Invalid handle pointer (NULL).
 */
HALx_StatusTypeDef I2Cx_Init(I2Cx_HandleTypeDef *hi2c)
{
    if ((hi2c == NULL) || (hi2c->Instance == NULL))
    {
        return STD_ERROR;
    }

    /* Disable peripheral before configuration */
    hi2c->Instance->CR1 &= ~I2Cx_CR1_PE;

    /* Calculate PCLK1 frequency value (in MHz) */
    uint32_t pclk1 = hi2c->Init.PCLK1_Freq;
    uint32_t freqrange = pclk1 / 1000000U;

    /* Configure APB1 frequency range into CR2 register */
    uint32_t tmpreg = hi2c->Instance->CR2;
    tmpreg &= ~I2Cx_CR2_FREQ_Msk;
    tmpreg |= (freqrange & I2Cx_CR2_FREQ_Msk);
    hi2c->Instance->CR2 = tmpreg;

    /* Configure maximum rise time (TRISE) */
    if (hi2c->Init.ClockSpeed <= I2C_CLOCK_STANDARD)
    {
        hi2c->Instance->TRISE = freqrange + 1U;
    }
    else
    {
        hi2c->Instance->TRISE = (((freqrange * 300U) / 1000U) + 1U);
    }

    /* Configure Clock Control Register (CCR) */
    uint16_t result = 0U;
    if (hi2c->Init.ClockSpeed <= I2C_CLOCK_STANDARD)
    {
        result = (uint16_t)(pclk1 / (hi2c->Init.ClockSpeed << 1U));
        if (result < 0x04U)
        {
            result = 0x04U;
        }
        tmpreg = result;
    }
    else
    {
        if (hi2c->Init.DutyCycle == I2C_DUTYCYCLE_2)
        {
            result = (uint16_t)(pclk1 / (hi2c->Init.ClockSpeed * 3U));
        }
        else
        {
            result = (uint16_t)(pclk1 / (hi2c->Init.ClockSpeed * 25U));
            result |= I2C_DUTYCYCLE_16_9;
        }
        tmpreg = result | I2Cx_CCR_FS;
    }
    hi2c->Instance->CCR = tmpreg;

    /* Configure Own Address 1 (OAR1) */
    hi2c->Instance->OAR1 = hi2c->Init.OwnAddress1;

    /* Enable peripheral */
    hi2c->Instance->CR1 |= I2Cx_CR1_PE;

    hi2c->State = I2C_STATE_READY;
    return STD_OK;
}

/**
 * @brief  De-initializes the I2C peripheral registers to their default reset values via Software Reset.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: De-initialization successful.
 *         - STD_ERROR: Invalid handle pointer.
 */
HALx_StatusTypeDef I2Cx_DeInit(I2Cx_HandleTypeDef *hi2c)
{
    if ((hi2c == NULL) || (hi2c->Instance == NULL))
    {
        return STD_ERROR;
    }

    hi2c->Instance->CR1 |= I2Cx_CR1_SWRST;
    hi2c->Instance->CR1 &= ~I2Cx_CR1_SWRST;
    hi2c->State = I2C_STATE_RESET;

    return STD_OK;
}

/**
 * @brief  Transmits data in Master mode using Polling/Blocking method.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  pData: Pointer to transmit data buffer.
 * @param  Size: Amount of data bytes to transmit.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Transmission completed successfully.
 *         - STD_ERROR: Invalid parameter.
 *         - STD_BUSY: Bus is busy.
 *         - STD_TIMEOUT: Operation timed out.
 */
HALx_StatusTypeDef I2Cx_Master_Transmit(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR2_BUSY, SET, Timeout) != STD_OK)
    {
        return STD_BUSY;
    }

    /* Enable Acknowledge & Generate Start condition */
    hi2c->Instance->CR1 |= I2Cx_CR1_ACK;
    hi2c->Instance->CR1 |= I2Cx_CR1_START;

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_SB, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    /* Send Slave address with Write bit */
    I2C_SendAddress(hi2c, DevAddress, 0x00U);

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_ADDR, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    I2C_ClearADDR(hi2c);

    /* Transmit payload byte by byte */
    for (uint16_t i = 0U; i < Size; i++)
    {
        if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
        {
            return STD_TIMEOUT;
        }
        hi2c->Instance->DR = pData[i];
    }

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_BTF, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    /* Generate Stop condition to finalize transfer */
    hi2c->Instance->CR1 |= I2Cx_CR1_STOP;

    return STD_OK;
}

/**
 * @brief  Receives data in Master mode using Polling/Blocking method.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to receive.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Reception completed successfully.
 *         - STD_ERROR: Invalid parameter.
 *         - STD_BUSY: Bus is busy.
 *         - STD_TIMEOUT: Operation timed out.
 */
HALx_StatusTypeDef I2Cx_Master_Receive(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR2_BUSY, SET, Timeout) != STD_OK)
    {
        return STD_BUSY;
    }

    hi2c->Instance->CR1 |= I2Cx_CR1_ACK;
    hi2c->Instance->CR1 |= I2Cx_CR1_START;

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_SB, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    /* Send Slave address with Read bit */
    I2C_SendAddress(hi2c, DevAddress, 0x01U);

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_ADDR, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    if (Size == 1U)
    {
        /* Single-byte reception handling: clear ACK before clearing ADDR and issuing STOP */
        hi2c->Instance->CR1 &= ~I2Cx_CR1_ACK;
        I2C_ClearADDR(hi2c);
        hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
    }
    else
    {
        I2C_ClearADDR(hi2c);
    }

    for (uint16_t i = 0U; i < Size; i++)
    {
        if ((Size > 1U) && (i == (Size - 2U)))
        {
            /* Prepare NACK and STOP for the last byte in the sequence */
            if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_RxNE, RESET, Timeout) != STD_OK)
            {
                return STD_TIMEOUT;
            }
            pData[i] = (uint8_t)hi2c->Instance->DR;
            hi2c->Instance->CR1 &= ~I2Cx_CR1_ACK;
            hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
            i++;
        }

        if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_RxNE, RESET, Timeout) != STD_OK)
        {
            return STD_TIMEOUT;
        }

        pData[i] = (uint8_t)hi2c->Instance->DR;
    }

    return STD_OK;
}

/**
 * @brief  Writes data to a specific internal register or memory address of an I2C device (Polling/Blocking).
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  MemAddress: Internal register/memory address to write to.
 * @param  MemAddSize: Internal memory address size (1 for 8-bit, 2 for 16-bit).
 * @param  pData: Pointer to transmit data buffer.
 * @param  Size: Amount of data bytes to write.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Memory write completed successfully.
 *         - STD_ERROR: Invalid parameter.
 *         - STD_BUSY: Bus is busy.
 *         - STD_TIMEOUT: Operation timed out.
 */
HALx_StatusTypeDef I2Cx_Mem_Write(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR2_BUSY, SET, Timeout) != STD_OK)
    {
        return STD_BUSY;
    }

    hi2c->Instance->CR1 |= I2Cx_CR1_ACK;
    hi2c->Instance->CR1 |= I2Cx_CR1_START;

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_SB, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    I2C_SendAddress(hi2c, DevAddress, 0x00U);

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_ADDR, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    I2C_ClearADDR(hi2c);

    /* Send internal register address (8-bit or 16-bit) */
    if (MemAddSize == 2U)
    {
        if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
        {
            return STD_TIMEOUT;
        }
        hi2c->Instance->DR = (uint8_t)((MemAddress >> 8U) & 0xFFU);
    }
    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }
    hi2c->Instance->DR = (uint8_t)(MemAddress & 0xFFU);

    /* Transmit payload data */
    for (uint16_t i = 0U; i < Size; i++)
    {
        if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
        {
            return STD_TIMEOUT;
        }
        hi2c->Instance->DR = pData[i];
    }

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_BTF, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    hi2c->Instance->CR1 |= I2Cx_CR1_STOP;

    return STD_OK;
}

/**
 * @brief  Reads data from a specific internal register or memory address of an I2C device (Polling/Blocking).
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  MemAddress: Internal register/memory address to read from.
 * @param  MemAddSize: Internal memory address size (1 for 8-bit, 2 for 16-bit).
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to read.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef: Execution status returned from the underlying master receive routine.
 */
HALx_StatusTypeDef I2Cx_Mem_Read(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR2_BUSY, SET, Timeout) != STD_OK)
    {
        return STD_BUSY;
    }

    /* 1. Send START condition and target memory address */
    hi2c->Instance->CR1 |= I2Cx_CR1_ACK;
    hi2c->Instance->CR1 |= I2Cx_CR1_START;

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_SB, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    I2C_SendAddress(hi2c, DevAddress, 0x00U);

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_ADDR, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    I2C_ClearADDR(hi2c);

    if (MemAddSize == 2U)
    {
        if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
        {
            return STD_TIMEOUT;
        }
        hi2c->Instance->DR = (uint8_t)((MemAddress >> 8U) & 0xFFU);
    }
    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }
    hi2c->Instance->DR = (uint8_t)(MemAddress & 0xFFU);

    if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_TxE, RESET, Timeout) != STD_OK)
    {
        return STD_TIMEOUT;
    }

    /* 2. Switch to Master Receive mode to read back the requested data sequence */
    return I2Cx_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
}

/**
 * @brief  Checks whether a target slave device is active and ready for communication on the bus.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave address to test.
 * @param  Trials: Maximum number of retry attempts.
 * @param  Timeout: Timeout duration per attempt in milliseconds.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Device responded with an ACK.
 *         - STD_ERROR: Device failed to respond after specified number of retry attempts.
 */
HALx_StatusTypeDef I2Cx_IsDeviceReady(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
    uint32_t trial = 0U;

    while (trial < Trials)
    {
        hi2c->Instance->CR1 |= I2Cx_CR1_START;

        if (I2C_WaitOnFlag(hi2c, I2Cx_SR1_SB, RESET, Timeout) == STD_OK)
        {
            I2C_SendAddress(hi2c, DevAddress, 0x00U);

            uint32_t count = Timeout * 1000U;
            while (((hi2c->Instance->SR1 & (I2Cx_SR1_ADDR | I2Cx_SR1_AF)) == 0U) && (count > 0U))
            {
                count--;
            }

            if ((hi2c->Instance->SR1 & I2Cx_SR1_ADDR) != 0U)
            {
                I2C_ClearADDR(hi2c);
                hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
                return STD_OK;
            }

            if ((hi2c->Instance->SR1 & I2Cx_SR1_AF) != 0U)
            {
                /* Clear Acknowledge Failure (AF) flag by writing 0 on STM32F1 */
                hi2c->Instance->SR1 = (uint16_t)(~I2Cx_SR1_AF);
                hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
            }
        }
        trial++;
    }
    return STD_ERROR;
}

/**
 * @brief  Initiates Master transmission in non-blocking mode using Interrupts.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  pData: Pointer to transmit data buffer.
 * @param  Size: Amount of data bytes to transmit.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Interrupt-driven transfer successfully triggered.
 *         - STD_ERROR: Invalid parameter.
 *         - STD_BUSY: Peripheral is busy processing another transfer.
 */
HALx_StatusTypeDef I2Cx_Master_Transmit_IT(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (hi2c->State != I2C_STATE_READY)
    {
        return STD_BUSY;
    }

    hi2c->State       = I2C_STATE_BUSY_TX;
    hi2c->DevAddress  = DevAddress;
    hi2c->pTxBuffPtr  = pData;
    hi2c->TxXferSize  = Size;
    hi2c->TxXferCount = Size;

    /* Enable I2C Event, Buffer, Error interrupts and generate START condition */
    hi2c->Instance->CR2 |= (I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITBUFEN | I2Cx_CR2_ITERREN);
    hi2c->Instance->CR1 |= I2Cx_CR1_START;

    return STD_OK;
}

/**
 * @brief  Initiates Master reception in non-blocking mode using Interrupts.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to receive.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Interrupt-driven reception successfully triggered.
 *         - STD_ERROR: Invalid parameter.
 *         - STD_BUSY: Peripheral is busy processing another transfer.
 */
HALx_StatusTypeDef I2Cx_Master_Receive_IT(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (hi2c->State != I2C_STATE_READY)
    {
        return STD_BUSY;
    }

    hi2c->State       = I2C_STATE_BUSY_RX;
    hi2c->DevAddress  = DevAddress;
    hi2c->pRxBuffPtr  = pData;
    hi2c->RxXferSize  = Size;
    hi2c->RxXferCount = Size;

    /* Enable I2C Event, Buffer, Error interrupts and generate START condition */
    hi2c->Instance->CR2 |= (I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITBUFEN | I2Cx_CR2_ITERREN);
    hi2c->Instance->CR1 |= I2Cx_CR1_START;

    return STD_OK;
}

/**
 * @brief  Initiates Master transmission using DMA controller.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  pData: Pointer to transmit data buffer.
 * @param  Size: Amount of data bytes to transmit.
 * @retval HALx_StatusTypeDef: Status result from enabling DMA transfer.
 */
HALx_StatusTypeDef I2Cx_Master_Transmit_DMA(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    hi2c->Instance->CR2 |= I2Cx_CR2_DMAEN;
    return I2Cx_Master_Transmit_IT(hi2c, DevAddress, pData, Size);
}

/**
 * @brief  Initiates Master reception using DMA controller.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @param  DevAddress: Target slave device address.
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to receive.
 * @retval HALx_StatusTypeDef: Status result from enabling DMA transfer.
 */
HALx_StatusTypeDef I2Cx_Master_Receive_DMA(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    if ((hi2c == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    hi2c->Instance->CR2 |= I2Cx_CR2_DMAEN;
    return I2Cx_Master_Receive_IT(hi2c, DevAddress, pData, Size);
}

/**
 * @brief  Handles I2C Event Interrupt requests.
 * @details Serves hardware event flags including SB, ADDR, TxE, and RxNE.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval None
 */
void I2Cx_EV_IRQHandler(I2Cx_HandleTypeDef *hi2c)
{
    uint32_t sr1 = hi2c->Instance->SR1;

    /* Start Bit Event (SB) */
    if ((sr1 & I2Cx_SR1_SB) != 0U)
    {
        I2C_SendAddress(hi2c, hi2c->DevAddress, (hi2c->State == I2C_STATE_BUSY_RX) ? 0x01U : 0x00U);
    }
    /* Address Sent/Matched Event (ADDR) */
    else if ((sr1 & I2Cx_SR1_ADDR) != 0U)
    {
        if ((hi2c->State == I2C_STATE_BUSY_RX) && (hi2c->RxXferCount == 1U))
        {
            hi2c->Instance->CR1 &= ~I2Cx_CR1_ACK;
        }
        I2C_ClearADDR(hi2c);
    }
    /* Transmit Data Register Empty Event (TxE) */
    else if ((sr1 & I2Cx_SR1_TxE) != 0U)
    {
        if (hi2c->TxXferCount > 0U)
        {
            hi2c->Instance->DR = *hi2c->pTxBuffPtr;
            hi2c->pTxBuffPtr++;
            hi2c->TxXferCount--;
        }
        else
        {
            hi2c->Instance->CR2 &= ~(I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITBUFEN);
            hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
            hi2c->State = I2C_STATE_READY;
            I2Cx_MasterTxCpltCallback(hi2c);
        }
    }
    /* Receive Data Register Not Empty Event (RxNE) */
    else if ((sr1 & I2Cx_SR1_RxNE) != 0U)
    {
        if (hi2c->RxXferCount > 0U)
        {
            if (hi2c->RxXferCount == 1U)
            {
                hi2c->Instance->CR1 &= ~I2Cx_CR1_ACK;
                hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
            }

            *hi2c->pRxBuffPtr = (uint8_t)hi2c->Instance->DR;
            hi2c->pRxBuffPtr++;
            hi2c->RxXferCount--;

            if (hi2c->RxXferCount == 0U)
            {
                hi2c->Instance->CR2 &= ~(I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITBUFEN);
                hi2c->State = I2C_STATE_READY;
                I2Cx_MasterRxCpltCallback(hi2c);
            }
        }
    }
}

/**
 * @brief  Handles I2C Error Interrupt requests.
 * @details Serves peripheral hardware errors including Acknowledge Failure (AF), Bus Error (BERR), and Arbitration Loss (ARLO).
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval None
 */
void I2Cx_ER_IRQHandler(I2Cx_HandleTypeDef *hi2c)
{
    uint32_t sr1 = hi2c->Instance->SR1;

    /* Acknowledge Failure error */
    if ((sr1 & I2Cx_SR1_AF) != 0U)
    {
        hi2c->Instance->SR1 = (uint16_t)(~I2Cx_SR1_AF);
        hi2c->Instance->CR1 |= I2Cx_CR1_STOP;
    }
    /* Bus Error */
    if ((sr1 & I2Cx_SR1_BERR) != 0U)
    {
        hi2c->Instance->SR1 = (uint16_t)(~I2Cx_SR1_BERR);
    }
    /* Arbitration Loss error */
    if ((sr1 & I2Cx_SR1_ARLO) != 0U)
    {
        hi2c->Instance->SR1 = (uint16_t)(~I2Cx_SR1_ARLO);
    }

    hi2c->State = I2C_STATE_READY;
    I2Cx_ErrorCallback(hi2c);
}

/**
 * @brief  Master Tx Transfer completed callback (Weak Function).
 * @note   This function can be overridden by user code in application context.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval None
 */
__attribute__((weak)) void I2Cx_MasterTxCpltCallback(I2Cx_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief  Master Rx Transfer completed callback (Weak Function).
 * @note   This function can be overridden by user code in application context.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval None
 */
__attribute__((weak)) void I2Cx_MasterRxCpltCallback(I2Cx_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief  I2C error callback (Weak Function).
 * @note   This function can be overridden by user code in application context.
 * @param  hi2c: Pointer to an I2Cx_HandleTypeDef structure containing I2C configuration.
 * @retval None
 */
__attribute__((weak)) void I2Cx_ErrorCallback(I2Cx_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

#ifdef __cplusplus
}
#endif

/***********************************************************************************************************************
 *                                                   END OF FILE
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_lin.c
 *
 * Description:  HAL LIN Driver - Implementation for STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          10/08/2026          Initial release for LIN Module
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include "hal_lin.h"

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
 *                                    LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
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

/***********************************************************************************************************************
 *                                               LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                              GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
  * @brief  Initializes the UART peripheral for LIN Mode operation.
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure that contains configuration info.
  * @param  breakLength: Configuration for 10-bit or 11-bit break detection length.
  * @retval HALx_StatusTypeDef: STD_OK if initialization succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef LINx_Init(UARTx_HandleTypeDef *huart, LIN_BreakLength_t breakLength)
{
    if ((huart == NULL) || (huart->Instance == NULL))
    {
        return STD_ERROR;
    }

    /* 1. Initialize Base UART hardware configuration */
    HALx_StatusTypeDef status = UARTx_Init(huart);
    if (status != STD_OK)
    {
        return STD_ERROR;
    }

    /* 2. Configure LIN Hardware Enable bit, Break Detection Length and Break Interrupt in CR2 */
    uint32_t tmpreg = huart->Instance->CR2;
    tmpreg &= ~USARTx_CR2_LBDL;
    tmpreg |= USARTx_CR2_LINEN | USARTx_CR2_LBDIE | (uint32_t)breakLength;
    huart->Instance->CR2 = tmpreg;

    return STD_OK;
}

/**
  * @brief  Triggers hardware transmission of the LIN Break Field (Sets SBK bit).
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure.
  * @retval None
  */
void LINx_SendBreak(UARTx_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        huart->Instance->CR1 |= USARTx_CR1_SBK;
    }
}

/**
  * @brief  Calculates Protected Identifier (PID) with Parity bits P0 and P1.
  * @param  id: Frame Identifier (0x00 to 0x3F).
  * @retval uint8_t: Formatted PID byte (ID | P0<<6 | P1<<7).
  */
uint8_t LINx_CalculatePID(uint8_t id)
{
    uint8_t clean_id = id & 0x3FU;
    uint8_t p0 = ((clean_id >> 0) ^ (clean_id >> 1) ^ (clean_id >> 2) ^ (clean_id >> 4)) & 0x01U;
    uint8_t p1 = ~((clean_id >> 1) ^ (clean_id >> 3) ^ (clean_id >> 4) ^ (clean_id >> 5)) & 0x01U;

    return (clean_id | (p0 << 6U) | (p1 << 7U));
}

/**
  * @brief  Calculates LIN Checksum using inverted carry addition.
  * @param  pid: Protected Identifier.
  * @param  pData: Pointer to payload data buffer.
  * @param  len: Length of the payload buffer (1 to 8 bytes).
  * @param  type: LIN_CHECKSUM_CLASSIC or LIN_CHECKSUM_ENHANCED.
  * @retval uint8_t: Inverted sum result.
  */
uint8_t LINx_CalculateChecksum(uint8_t pid, const uint8_t *pData, uint8_t len, LIN_ChecksumType_t type)
{
    if ((pData == NULL) || (len == 0U))
    {
        return 0U;
    }

    uint16_t sum = (type == LIN_CHECKSUM_ENHANCED) ? pid : 0U;

    for (uint8_t i = 0U; i < len; i++)
    {
        sum += pData[i];
        if (sum > 0xFFU)
        {
            sum -= 0xFFU;
        }
    }
    return (uint8_t)(~sum);
}

/**
  * @brief  Transmits a LIN Header (Break Field -> Sync Byte -> PID).
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure.
  * @param  id: Frame Identifier.
  * @retval HALx_StatusTypeDef: STD_OK if header transmission succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef LINx_SendHeader(UARTx_HandleTypeDef *huart, uint8_t id)
{
    if (huart == NULL)
    {
        return STD_ERROR;
    }

    /* 1. Transmit Break Field Signal */
    LINx_SendBreak(huart);

    /* 2. Send Sync Byte (0x55) and Protected ID (PID) via UART Transmit */
    uint8_t pid = LINx_CalculatePID(id);
    uint8_t header[2] = {LIN_SYNC_BYTE, pid};

    return UARTx_Transmit(huart, header, 2U, LIN_DEFAULT_TIMEOUT);
}

/**
  * @brief  Transmits a LIN Data Response (Data Payload + Checksum Byte).
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure.
  * @param  id: Frame Identifier (used for Checksum calculation).
  * @param  pData: Pointer to transmission data payload.
  * @param  len: Length of payload data.
  * @param  type: Checksum algorithm type.
  * @retval HALx_StatusTypeDef: STD_OK if response succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef LINx_SendResponse(UARTx_HandleTypeDef *huart, uint8_t id, const uint8_t *pData, uint8_t len, LIN_ChecksumType_t type)
{
    if ((huart == NULL) || (pData == NULL) || (len == 0U))
    {
        return STD_ERROR;
    }

    uint8_t pid = LINx_CalculatePID(id);
    uint8_t checksum = LINx_CalculateChecksum(pid, pData, len, type);

    /* Transmit Data Payload */
    if (UARTx_Transmit(huart, pData, len, LIN_DEFAULT_TIMEOUT) != STD_OK)
    {
        return STD_ERROR;
    }

    /* Transmit Checksum Byte */
    return UARTx_Transmit(huart, &checksum, 1U, LIN_DEFAULT_TIMEOUT);
}

/**
  * @brief  Transmits a Complete LIN Master Frame (Header + Data + Checksum).
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure.
  * @param  id: Frame Identifier.
  * @param  pData: Pointer to payload data buffer.
  * @param  len: Length of payload data.
  * @param  type: Checksum algorithm type.
  * @retval HALx_StatusTypeDef: STD_OK if frame transmission succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef LINx_SendFrame(UARTx_HandleTypeDef *huart, uint8_t id, const uint8_t *pData, uint8_t len, LIN_ChecksumType_t type)
{
    if (LINx_SendHeader(huart, id) != STD_OK)
    {
        return STD_ERROR;
    }

    return LINx_SendResponse(huart, id, pData, len, type);
}

/**
  * @brief  Handles LIN specific peripheral interrupts (Break Detection).
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure.
  * @retval None
  */
void LINx_IRQHandler(UARTx_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return;
    }

    uint32_t sr = huart->Instance->SR;
    uint32_t cr2 = huart->Instance->CR2;

    /* Check LIN Break Detection Flag (LBD) and Interrupt Enable Flag (LBDIE) */
    if (((sr & USARTx_SR_LBD) != 0U) && ((cr2 & USARTx_CR2_LBDIE) != 0U))
    {
        /* Clear LBD flag by writing 0 */
        huart->Instance->SR &= ~USARTx_SR_LBD;

        /* Execute User Weak Callback */
        LINx_BreakCallback(huart);
    }
}

/**
  * @brief  LIN Break Detection Interrupt Callback.
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure.
  * @retval None
  */
__attribute__((weak)) void LINx_BreakCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
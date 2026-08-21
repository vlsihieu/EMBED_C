/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_uart.c
 *
 * Description:  HAL UART Driver - Implementation for STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          10/08/2026          Initial release for STM32F103xB UART Module
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include "hal_uart.h"

/***********************************************************************************************************************
 *                                           SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                            FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                    EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                      LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                            LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static uint32_t UARTx_GetPCLK(const USARTx_TypeDef_t *Instance);
static void     UARTx_SetBaudRate(USARTx_TypeDef_t *Instance, uint32_t BaudRate, uint32_t pclk);

/***********************************************************************************************************************
 *                                                 LOCAL FUNCTION
 **********************************************************************************************************************/

/**
  * @brief  Gets peripheral clock (PCLK) frequency dynamically based on USART Instance.
  * @param  Instance: Pointer to USART peripheral instance.
  * @retval uint32_t: Clock frequency in Hz.
  */
static uint32_t UARTx_GetPCLK(const USARTx_TypeDef_t *Instance)
{
    uint32_t pclk;

    if (Instance == USART1x)
    {
        /* USART1 on APB2 Bus uses RCC_CFGR_PPRE2 */
        pclk = (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]);
    }
    else
    {
        /* USART2, USART3 on APB1 Bus uses RCC_CFGR_PPRE1 */
        pclk = (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);
    }

    return pclk;
}

/**
  * @brief  Calculates and sets the BRR register value for specified Baud Rate.
  * @param  Instance: Pointer to USART peripheral instance.
  * @param  BaudRate: Target baud rate value.
  * @param  pclk: Peripheral clock frequency in Hz.
  * @retval None
  */
static void UARTx_SetBaudRate(USARTx_TypeDef_t *Instance, uint32_t BaudRate, uint32_t pclk)
{
    float USARTDIV = (float)pclk / ((float)BaudRate * 16.0f);
    
    uint16_t mantissa = (uint16_t)USARTDIV;

    uint16_t fraction = (uint16_t)((USARTDIV - (float)mantissa) * 16.0f + 0.5f);

    if (fraction >= 16U)
    {
        mantissa += 1U;
        fraction = 0U;
    }

    uint32_t bound = (mantissa << 4U) | (fraction & 0x0FU);

    Instance->BRR = bound;
}

/***********************************************************************************************************************
 *                                                 GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
  * @brief  Initializes the UART peripheral according to the specified parameters in UARTx_InitTypeDef.
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure that contains configuration info.
  * @retval HALx_StatusTypeDef: STD_OK if initialization succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Init(UARTx_HandleTypeDef *huart)
{
    if ((huart == NULL) || (huart->Instance == NULL))
    {
        return STD_ERROR;
    }

    /* Disable USART prior to configuration */
    huart->Instance->CR1 &= ~USARTx_CR1_UE;

    /* Configure Word Length, Parity and Transfer Mode in CR1 */
    uint32_t tmpreg = huart->Instance->CR1;
    tmpreg &= ~(USARTx_CR1_M | USARTx_CR1_PCE | USARTx_CR1_PS | USARTx_CR1_TE | USARTx_CR1_RE);
    tmpreg |= huart->Init.WordLength;
    tmpreg |= huart->Init.Parity;
    tmpreg |= huart->Init.Mode;
    huart->Instance->CR1 = tmpreg;

    /* Configure Stop Bits in CR2 */
    tmpreg = huart->Instance->CR2;
    tmpreg &= ~USARTx_CR2_STOP_MASK;
    tmpreg |= huart->Init.StopBits;
    huart->Instance->CR2 = tmpreg;

    /* Configure Hardware Flow Control in CR3 */
    tmpreg = huart->Instance->CR3;
    tmpreg &= ~(USARTx_CR3_RTSE | USARTx_CR3_CTSE);
    tmpreg |= huart->Init.HwFlowCtl;
    huart->Instance->CR3 = tmpreg;

    /* Configure BaudRate */
    uint32_t pclk = UARTx_GetPCLK(huart->Instance);
    UARTx_SetBaudRate(huart->Instance, huart->Init.BaudRate, pclk);

    /* Enable Peripheral */
    huart->Instance->CR1 |= USARTx_CR1_UE;

    huart->ErrorCode = 0U;
    return STD_OK;
}

/**
  * @brief  Transmits an amount of data in blocking mode (Polling).
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be sent.
  * @param  Timeout: Timeout duration in milliseconds.
  * @retval HALx_StatusTypeDef: STD_OK if transmission succeeded, STD_ERROR on parameter or timeout error.
  */
HALx_StatusTypeDef UARTx_Transmit(UARTx_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((huart == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    uint32_t tickstart = Timeout; /* Dummy countdown timer simulation */

    for (uint16_t i = 0U; i < Size; i++)
    {
        while ((huart->Instance->SR & USARTx_SR_TXE) == 0U)
        {
            if (tickstart == 0U)
            {
                return STD_ERROR;
            }
            tickstart--;
        }
        huart->Instance->DR = (uint16_t)pData[i];
    }

    /* Wait for TC bit to be set before exiting */
    while ((huart->Instance->SR & USARTx_SR_TC) == 0U)
    {
        if (tickstart == 0U)
        {
            return STD_ERROR;
        }
        tickstart--;
    }

    return STD_OK;
}

/**
  * @brief  Receives an amount of data in blocking mode (Polling).
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be received.
  * @param  Timeout: Timeout duration in milliseconds.
  * @retval HALx_StatusTypeDef: STD_OK if reception succeeded, STD_ERROR on parameter or timeout error.
  */
HALx_StatusTypeDef UARTx_Receive(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((huart == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    uint32_t tickstart = Timeout;

    for (uint16_t i = 0U; i < Size; i++)
    {
        while ((huart->Instance->SR & USARTx_SR_RXNE) == 0U)
        {
            if (tickstart == 0U)
            {
                return STD_ERROR;
            }
            tickstart--;
        }
        pData[i] = (uint8_t)(huart->Instance->DR & 0xFFU);
    }

    return STD_OK;
}

/**
  * @brief  Transmits an amount of data in non-blocking Interrupt mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be sent.
  * @retval HALx_StatusTypeDef: STD_OK if transfer started successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Transmit_IT(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    if ((huart == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (huart->TxXferCount > 0U)
    {
        return STD_ERROR; /* Busy */
    }

    huart->pTxBuffPtr  = pData;
    huart->TxXferSize   = Size;
    huart->TxXferCount  = Size;

    /* Enable TXE Interrupt */
    huart->Instance->CR1 |= USARTx_CR1_TXEIE;

    return STD_OK;
}

/**
  * @brief  Receives an amount of data in non-blocking Interrupt mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be received.
  * @retval HALx_StatusTypeDef: STD_OK if reception started successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Receive_IT(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    if ((huart == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    if (huart->RxXferCount > 0U)
    {
        return STD_ERROR; /* Busy */
    }

    huart->pRxBuffPtr  = pData;
    huart->RxXferSize   = Size;
    huart->RxXferCount  = Size;

    /* Enable RXNE & Parity Error Interrupts */
    huart->Instance->CR1 |= (USARTx_CR1_RXNEIE | USARTx_CR1_PEIE);

    return STD_OK;
}

/**
  * @brief  Transmits an amount of data in non-blocking DMA mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be sent.
  * @retval HALx_StatusTypeDef: STD_OK if DMA transfer initiated successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Transmit_DMA(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    if ((huart == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    huart->pTxBuffPtr = pData;
    huart->TxXferSize  = Size;

    /* Enable UART DMA Transmitter bit */
    huart->Instance->CR3 |= USARTx_CR3_DMAT;

    return STD_OK;
}

/**
  * @brief  Receives an amount of data in non-blocking DMA mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be received.
  * @retval HALx_StatusTypeDef: STD_OK if DMA reception initiated successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Receive_DMA(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    if ((huart == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    huart->pRxBuffPtr = pData;
    huart->RxXferSize  = Size;

    /* Enable UART DMA Receiver bit */
    huart->Instance->CR3 |= USARTx_CR3_DMAR;

    return STD_OK;
}

/**
  * @brief  Stops ongoing UART DMA transfer (Tx or Rx).
  * @param  huart: Pointer to UART handle structure.
  * @retval HALx_StatusTypeDef: STD_OK if DMA stop operation succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_DMAStop(UARTx_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return STD_ERROR;
    }

    huart->Instance->CR3 &= ~(USARTx_CR3_DMAT | USARTx_CR3_DMAR);

    return STD_OK;
}

/**
  * @brief  Handles UART Interrupt Requests for specified instance.
  * @param  huart: Pointer to UART handle structure.
  * @retval None
  */
void UARTx_IRQHandler(UARTx_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return;
    }

    uint32_t sr = huart->Instance->SR;
    uint32_t cr1 = huart->Instance->CR1;

    /* RXNE Interrupt Handling */
    if (((sr & USARTx_SR_RXNE) != 0U) && ((cr1 & USARTx_CR1_RXNEIE) != 0U))
    {
        *huart->pRxBuffPtr = (uint8_t)(huart->Instance->DR & 0xFFU);
        huart->pRxBuffPtr++;
        huart->RxXferCount--;

        if (huart->RxXferCount == 0U)
        {
            huart->Instance->CR1 &= ~USARTx_CR1_RXNEIE;
            UARTx_RxCpltCallback(huart);
        }
    }

    /* TXE Interrupt Handling */
    if (((sr & USARTx_SR_TXE) != 0U) && ((cr1 & USARTx_CR1_TXEIE) != 0U))
    {
        huart->Instance->DR = (uint16_t)(*huart->pTxBuffPtr);
        huart->pTxBuffPtr++;
        huart->TxXferCount--;

        if (huart->TxXferCount == 0U)
        {
            huart->Instance->CR1 &= ~USARTx_CR1_TXEIE;
            huart->Instance->CR1 |= USARTx_CR1_TCIE;
        }
    }

    /* TC Interrupt Handling */
    if (((sr & USARTx_SR_TC) != 0U) && ((cr1 & USARTx_CR1_TCIE) != 0U))
    {
        huart->Instance->CR1 &= ~USARTx_CR1_TCIE;
        UARTx_TxCpltCallback(huart);
    }
}

/**
  * @brief  Handles DMA Interrupt Requests associated with UART Tx/Rx channels.
  * @param  huart: Pointer to UART handle structure.
  * @param  is_tx: Flag indicating channel type (1 for Tx channel, 0 for Rx channel).
  * @retval None
  */
void UARTx_DMA_IRQHandler(UARTx_HandleTypeDef *huart, uint8_t is_tx)
{
    if (huart == NULL)
    {
        return;
    }

    if (is_tx != 0U)
    {
        huart->Instance->CR3 &= ~USARTx_CR3_DMAT;
        UARTx_TxCpltCallback(huart);
    }
    else
    {
        huart->Instance->CR3 &= ~USARTx_CR3_DMAR;
        UARTx_RxCpltCallback(huart);
    }
}

/**
  * @brief  Tx Transfer completed weak callback function.
  * @param  huart: Pointer to UART handle structure.
  * @retval None
  */
__attribute__((weak)) void UARTx_TxCpltCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

/**
  * @brief  Rx Transfer completed weak callback function.
  * @param  huart: Pointer to UART handle structure.
  * @retval None
  */
__attribute__((weak)) void UARTx_RxCpltCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

/*--------------------------------------------------- End Of File -----------------------------------------------------*/

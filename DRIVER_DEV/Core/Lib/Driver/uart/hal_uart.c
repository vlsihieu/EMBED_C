/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: hal_uart.c
 *
 * Description: HAL UART Driver - Implementation for STM32F103xB devices.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           10/08/2026          Initial release for STM32F103xB UART Module
 *              2.0.0           04/09/2026          Added optimized RX Ring Buffer support
 *
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                    INCLUDES
 **********************************************************************************************************************/

#include "hal_uart.h"

/***********************************************************************************************************************
 *                                           LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static uint32_t UARTx_GetPCLK(const USARTx_TypeDef_t *Instance);
static void     UARTx_SetBaudRate(USARTx_TypeDef_t *Instance,
                                  uint32_t BaudRate,
                                  uint32_t pclk);

/***********************************************************************************************************************
 *                                                 LOCAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief  Get the peripheral clock frequency used by the selected USART instance.
 */
static uint32_t UARTx_GetPCLK(const USARTx_TypeDef_t *Instance)
{
    uint32_t pclk;

    if (Instance == USART1x)
    {
        /* USART1 is connected to APB2. */
        pclk = (SystemCoreClock >>
                APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >>
                              RCC_CFGR_PPRE2_Pos]);
    }
    else
    {
        /* USART2 and USART3 are connected to APB1. */
        pclk = (SystemCoreClock >>
                APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >>
                              RCC_CFGR_PPRE1_Pos]);
    }

    return pclk;
}

/**
 * @brief  Calculate and program the USART BRR register.
 */
static void UARTx_SetBaudRate(USARTx_TypeDef_t *Instance,
                              uint32_t BaudRate,
                              uint32_t pclk)
{
    float usart_div;
    uint16_t mantissa;
    uint16_t fraction;
    uint32_t brr;

    usart_div = (float)pclk / ((float)BaudRate * 16.0f);

    mantissa = (uint16_t)usart_div;

    fraction = (uint16_t)(((usart_div - (float)mantissa) * 16.0f) + 0.5f);

    if (fraction >= 16U)
    {
        mantissa += 1U;
        fraction = 0U;
    }

    brr = ((uint32_t)mantissa << 4U) |
          ((uint32_t)fraction & 0x0FU);

    Instance->BRR = brr;
}

/***********************************************************************************************************************
 *                                                GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief  Initialise the UART peripheral.
 */
HALx_StatusTypeDef UARTx_Init(UARTx_HandleTypeDef *huart)
{
    uint32_t tmpreg;
    uint32_t pclk;

    if ((huart == NULL) ||
        (huart->Instance == NULL))
    {
        return STD_ERROR;
    }

    if (huart->Instance == USART1x)
    {
      RCC->APB2ENR |= (1U << 14);   // USART1EN
    }
    else if (huart->Instance == USART2x)
    {
      RCC->APB1ENR |= (1U << 17);   // USART2EN
    }
    else if (huart->Instance == USART3x)
    {
      RCC->APB1ENR |= (1U << 18);   // USART3EN
    }

    /* Disable USART before modifying configuration registers. */
    huart->Instance->CR1 &= ~USARTx_CR1_UE;

    /* Configure Word Length, Parity and Transfer Mode in CR1. */
    tmpreg = huart->Instance->CR1;

    tmpreg &= ~(USARTx_CR1_M   |
                USARTx_CR1_PCE |
                USARTx_CR1_PS  |
                USARTx_CR1_TE  |
                USARTx_CR1_RE);

    tmpreg |= huart->Init.WordLength;
    tmpreg |= huart->Init.Parity;
    tmpreg |= huart->Init.Mode;

    huart->Instance->CR1 = tmpreg;

    /* Configure Stop Bits in CR2. */
    tmpreg = huart->Instance->CR2;
    tmpreg &= ~USARTx_CR2_STOP_MASK;
    tmpreg |= huart->Init.StopBits;
    huart->Instance->CR2 = tmpreg;

    /* Configure RTS/CTS hardware flow control in CR3. */
    tmpreg = huart->Instance->CR3;
    tmpreg &= ~(USARTx_CR3_RTSE |
                USARTx_CR3_CTSE);
    tmpreg |= huart->Init.HwFlowCtl;
    huart->Instance->CR3 = tmpreg;

    /* Calculate peripheral clock and configure Baud Rate. */
    pclk = UARTx_GetPCLK(huart->Instance);
    UARTx_SetBaudRate(huart->Instance,
                      huart->Init.BaudRate,
                      pclk);

    /* Reset interrupt transfer runtime state. */
    huart->pTxBuffPtr = NULL;
    huart->TxXferSize = 0U;
    huart->TxXferCount = 0U;

    huart->pRxBuffPtr = NULL;
    huart->RxXferSize = 0U;
    huart->RxXferCount = 0U;

    /* Ring Buffer is explicitly attached after UART initialisation. */
    huart->pRxRingBuffer = NULL;
    huart->RxOverflowCount = 0U;

    huart->ErrorCode = 0U;

    /* Enable USART peripheral. */
    huart->Instance->CR1 |= USARTx_CR1_UE;

    return STD_OK;
}

/**
 * @brief  Transmit an amount of data in blocking mode.
 */
HALx_StatusTypeDef UARTx_Transmit(UARTx_HandleTypeDef *huart,
                                  const uint8_t *pData,
                                  uint16_t Size,
                                  uint32_t Timeout)
{
    uint32_t timeout_counter;
    uint16_t i;

    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    timeout_counter = Timeout;

    for (i = 0U; i < Size; i++)
    {
        /* Wait until TDR/DR can accept the next byte. */
        while ((huart->Instance->SR & USARTx_SR_TXE) == 0U)
        {
            if (timeout_counter == 0U)
            {
                return STD_ERROR;
            }

            timeout_counter--;
        }

        huart->Instance->DR = (uint16_t)pData[i];
    }

    /* Wait until the final frame has completely left the shift register. */
    while ((huart->Instance->SR & USARTx_SR_TC) == 0U)
    {
        if (timeout_counter == 0U)
        {
            return STD_ERROR;
        }

        timeout_counter--;
    }

    return STD_OK;
}

/**
 * @brief  Receive an amount of data in blocking mode.
 */
HALx_StatusTypeDef UARTx_Receive(UARTx_HandleTypeDef *huart,
                                 uint8_t *pData,
                                 uint16_t Size,
                                 uint32_t Timeout)
{
    uint32_t timeout_counter;
    uint16_t i;

    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    timeout_counter = Timeout;

    for (i = 0U; i < Size; i++)
    {
        while ((huart->Instance->SR & USARTx_SR_RXNE) == 0U)
        {
            if (timeout_counter == 0U)
            {
                return STD_ERROR;
            }

            timeout_counter--;
        }

        pData[i] = (uint8_t)(huart->Instance->DR & 0xFFU);
    }

    return STD_OK;
}

/**
 * @brief  Start a fixed-length UART transmission in interrupt mode.
 */
HALx_StatusTypeDef UARTx_Transmit_IT(UARTx_HandleTypeDef *huart,
                                     uint8_t *pData,
                                     uint16_t Size)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    if (huart->TxXferCount > 0U)
    {
        return STD_ERROR;
    }

    huart->pTxBuffPtr = pData;
    huart->TxXferSize = Size;
    huart->TxXferCount = Size;

    /* Enable TXE interrupt to start transmission. */
    huart->Instance->CR1 |= USARTx_CR1_TXEIE;

    return STD_OK;
}

/**
 * @brief  Start a fixed-length UART reception in interrupt mode.
 */
HALx_StatusTypeDef UARTx_Receive_IT(UARTx_HandleTypeDef *huart,
                                    uint8_t *pData,
                                    uint16_t Size)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    /* DR can have only one software consumer. */
    if (huart->pRxRingBuffer != NULL)
    {
        return STD_ERROR;
    }

    if (huart->RxXferCount > 0U)
    {
        return STD_ERROR;
    }

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->RxXferCount = Size;

    /* Enable RXNE and Parity Error interrupts. */
    huart->Instance->CR1 |= (USARTx_CR1_RXNEIE |
                             USARTx_CR1_PEIE);

    return STD_OK;
}

/**
 * @brief  Start UART transmission in DMA mode.
 */
HALx_StatusTypeDef UARTx_Transmit_DMA(UARTx_HandleTypeDef *huart,
                                      uint8_t *pData,
                                      uint16_t Size)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    huart->pTxBuffPtr = pData;
    huart->TxXferSize = Size;

    /* Enable USART DMA transmitter request. */
    huart->Instance->CR3 |= USARTx_CR3_DMAT;

    return STD_OK;
}

/**
 * @brief  Start UART reception in DMA mode.
 */
HALx_StatusTypeDef UARTx_Receive_DMA(UARTx_HandleTypeDef *huart,
                                     uint8_t *pData,
                                     uint16_t Size)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    if (huart->pRxRingBuffer != NULL)
    {
        return STD_ERROR;
    }

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;

    /* Enable USART DMA receiver request. */
    huart->Instance->CR3 |= USARTx_CR3_DMAR;

    return STD_OK;
}

/**
 * @brief  Stop UART DMA transfer requests.
 */
HALx_StatusTypeDef UARTx_DMAStop(UARTx_HandleTypeDef *huart)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL))
    {
        return STD_ERROR;
    }

    huart->Instance->CR3 &= ~(USARTx_CR3_DMAT |
                              USARTx_CR3_DMAR);

    return STD_OK;
}

/**
 * @brief  Start continuous UART RX using the generic Ring Buffer.
 */
HALx_StatusTypeDef UARTx_RxRingBufferStart(UARTx_HandleTypeDef *huart,
                                           RingBuffer_HandleTypeDef *pRingBuf)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL) ||
        (pRingBuf == NULL) ||
        (pRingBuf->pBuffer == NULL) ||
        (pRingBuf->Size < RING_BUFFER_MIN_SIZE))
    {
        return STD_ERROR;
    }

    /* Do not allow legacy fixed-length RX interrupt and Ring Buffer RX together. */
    if (huart->RxXferCount > 0U)
    {
        return STD_ERROR;
    }

    /* Do not allow DMA RX and RXNE interrupt to consume DR simultaneously. */
    if ((huart->Instance->CR3 & USARTx_CR3_DMAR) != 0U)
    {
        return STD_ERROR;
    }

    /* Attach the already initialised Ring Buffer. */
    huart->pRxRingBuffer = pRingBuf;

    /* Start a fresh overflow diagnostic count for this RX session. */
    huart->RxOverflowCount = 0U;

    /* RXNE interrupt remains enabled continuously. */
    huart->Instance->CR1 |= USARTx_CR1_RXNEIE;

    return STD_OK;
}

/**
 * @brief  Stop continuous UART RX Ring Buffer mode.
 */
HALx_StatusTypeDef UARTx_RxRingBufferStop(UARTx_HandleTypeDef *huart)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL))
    {
        return STD_ERROR;
    }

    /* Stop continuous RX interrupt generation. */
    huart->Instance->CR1 &= ~USARTx_CR1_RXNEIE;

    /* Detach Ring Buffer but preserve its already received data. */
    huart->pRxRingBuffer = NULL;

    return STD_OK;
}

/**
 * @brief  Read one byte from the attached UART RX Ring Buffer.
 */
RingBuffer_StatusTypeDef UARTx_RxReadByte(UARTx_HandleTypeDef *huart,
                                          uint8_t *pData)
{
    if ((huart == NULL) ||
        (huart->pRxRingBuffer == NULL) ||
        (pData == NULL))
    {
        return RING_BUFFER_ERROR;
    }

    return RingBuffer_ReadChar(huart->pRxRingBuffer,
                               pData);
}

/**
 * @brief  Read an exact number of bytes from the attached UART RX Ring Buffer.
 */
RingBuffer_StatusTypeDef UARTx_RxRead(UARTx_HandleTypeDef *huart,
                                      uint8_t *pData,
                                      uint16_t Length)
{
    if ((huart == NULL) ||
        (huart->pRxRingBuffer == NULL) ||
        (pData == NULL) ||
        (Length == 0U))
    {
        return RING_BUFFER_ERROR;
    }

    return RingBuffer_Read(huart->pRxRingBuffer,
                           pData,
                           Length);
}

/**
 * @brief  Get the number of unread bytes in the UART RX Ring Buffer.
 */
uint16_t UARTx_RxAvailable(const UARTx_HandleTypeDef *huart)
{
    if ((huart == NULL) ||
        (huart->pRxRingBuffer == NULL))
    {
        return 0U;
    }

    return RingBuffer_GetCount(huart->pRxRingBuffer);
}

/**
 * @brief  Get the accumulated RX Ring Buffer overflow count.
 */
uint32_t UARTx_GetRxOverflowCount(const UARTx_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return 0U;
    }

    return huart->RxOverflowCount;
}

/**
 * @brief  Clear the accumulated RX Ring Buffer overflow count.
 */
void UARTx_ClearRxOverflowCount(UARTx_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        huart->RxOverflowCount = 0U;
    }
}

/**
 * @brief  Handle UART interrupt requests for the specified instance.
 */
void UARTx_IRQHandler(UARTx_HandleTypeDef *huart)
{
    uint32_t sr;
    uint32_t cr1;

    if ((huart == NULL) ||
        (huart->Instance == NULL))
    {
        return;
    }

    sr = huart->Instance->SR;
    cr1 = huart->Instance->CR1;

    /* RXNE Interrupt Handling. */
    if (((sr & USARTx_SR_RXNE) != 0U) &&
        ((cr1 & USARTx_CR1_RXNEIE) != 0U))
    {
        uint8_t rx_data;

        rx_data = (uint8_t)(huart->Instance->DR & 0xFFU);

        if (huart->pRxRingBuffer != NULL)
        {
            if (RingBuffer_WriteChar(huart->pRxRingBuffer,
                                     rx_data) == RING_BUFFER_FULL)
            {
                huart->RxOverflowCount++;
            }
        }
        else if ((huart->RxXferCount > 0U) &&
                 (huart->pRxBuffPtr != NULL))
        {
            *huart->pRxBuffPtr = rx_data;

            huart->pRxBuffPtr++;
            huart->RxXferCount--;

            if (huart->RxXferCount == 0U)
            {
                huart->Instance->CR1 &= ~USARTx_CR1_RXNEIE;
                UARTx_RxCpltCallback(huart);
            }
        }
    }

    /* TXE Interrupt Handling. */
    if (((sr & USARTx_SR_TXE) != 0U) &&
        ((cr1 & USARTx_CR1_TXEIE) != 0U))
    {
        if ((huart->TxXferCount > 0U) &&
            (huart->pTxBuffPtr != NULL))
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
        else
        {
            huart->Instance->CR1 &= ~USARTx_CR1_TXEIE;
        }
    }

    /* TC Interrupt Handling. */
    if (((sr & USARTx_SR_TC) != 0U) &&
        ((cr1 & USARTx_CR1_TCIE) != 0U))
    {
        huart->Instance->CR1 &= ~USARTx_CR1_TCIE;
        UARTx_TxCpltCallback(huart);
    }
}

/**
 * @brief  Handle DMA interrupt completion associated with UART TX or RX.
 */
void UARTx_DMA_IRQHandler(UARTx_HandleTypeDef *huart,
                           uint8_t is_tx)
{
    if ((huart == NULL) ||
        (huart->Instance == NULL))
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

__attribute__((weak)) void UARTx_TxCpltCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

__attribute__((weak)) void UARTx_RxCpltCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

#ifdef __cplusplus
}
#endif

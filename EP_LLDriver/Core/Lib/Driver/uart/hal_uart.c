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
 *                                               SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                     EXTERN
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
 *
 * USART1 is connected to APB2 while USART2 and USART3 are connected to APB1.
 * The function reads the RCC APB prescaler configuration and derives the current PCLK.
 *
 * @param[in] Instance Pointer to USART peripheral instance.
 *
 * @retval Peripheral clock frequency in Hz.
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
 *
 * Calculates USARTDIV from peripheral clock and requested baud rate,
 * rounds the fractional part and writes Mantissa/Fraction into BRR.
 *
 * @param[in,out] Instance Pointer to USART peripheral instance.
 * @param[in]     BaudRate Requested communication baud rate.
 * @param[in]     pclk     USART peripheral clock frequency in Hz.
 *
 * @retval None
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
 *
 * Configures word length, parity, transfer mode, stop bits, hardware flow control,
 * baud rate and enables the USART peripheral.
 *
 * Runtime transfer fields and RX Ring Buffer state are reset to a known state.
 *
 * @param[in,out] huart Pointer to UARTx_HandleTypeDef structure.
 *
 * @retval STD_OK    UART initialised successfully.
 * @retval STD_ERROR Invalid parameter.
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

<<<<<<< HEAD
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

    /* Disable USART prior to configuration */
=======
    /* Disable USART before modifying configuration registers. */
>>>>>>> 45723b9 (update source)
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
 *
 * Waits for TXE before writing each byte and waits for TC after the final byte
 * so the last frame has completely left the TX pin.
 *
 * @param[in,out] huart   Pointer to UART handle structure.
 * @param[in]     pData   Pointer to source data buffer.
 * @param[in]     Size    Number of bytes to transmit.
 * @param[in]     Timeout Timeout loop count.
 *
 * @retval STD_OK    Transmission completed successfully.
 * @retval STD_ERROR Invalid parameter or timeout.
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
 *
 * Waits for RXNE and copies each received byte directly from USART DR
 * into the destination buffer.
 *
 * @param[in,out] huart   Pointer to UART handle structure.
 * @param[out]    pData   Pointer to destination data buffer.
 * @param[in]     Size    Number of bytes to receive.
 * @param[in]     Timeout Timeout loop count.
 *
 * @retval STD_OK    Reception completed successfully.
 * @retval STD_ERROR Invalid parameter or timeout.
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
 *
 * Saves the caller buffer and enables TXE interrupt. UARTx_IRQHandler()
 * sends one byte each time TXE becomes active.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[in]     pData Pointer to source data buffer.
 * @param[in]     Size  Number of bytes to transmit.
 *
 * @retval STD_OK    Interrupt transfer started successfully.
 * @retval STD_ERROR Invalid parameter or transmitter busy.
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
 *
 * Saves the destination buffer and enables RXNE interrupt. This legacy transfer
 * mode cannot run at the same time as continuous RX Ring Buffer mode.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[out]    pData Pointer to destination data buffer.
 * @param[in]     Size  Number of bytes to receive.
 *
 * @retval STD_OK    Interrupt reception started successfully.
 * @retval STD_ERROR Invalid parameter, receiver busy or Ring Buffer mode active.
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
 *
 * Saves the transfer information and enables the USART DMAT request bit.
 * DMA channel configuration/start is handled by the DMA driver.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[in]     pData Pointer to source data buffer.
 * @param[in]     Size  Number of bytes to transmit.
 *
 * @retval STD_OK    DMA request enabled successfully.
 * @retval STD_ERROR Invalid parameter.
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
 *
 * DMA RX and RX Ring Buffer interrupt mode cannot access USART DR simultaneously.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[out]    pData Pointer to destination data buffer.
 * @param[in]     Size  Number of bytes to receive.
 *
 * @retval STD_OK    DMA request enabled successfully.
 * @retval STD_ERROR Invalid parameter or Ring Buffer mode active.
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
 *
 * Clears both DMAT and DMAR bits in USART CR3.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval STD_OK    DMA requests disabled successfully.
 * @retval STD_ERROR Invalid parameter.
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
 *
 * The caller initialises the Ring Buffer first using RingBuffer_Init().
 * UART then stores each RXNE byte into that Ring Buffer inside UARTx_IRQHandler().
 *
 * Flow:
 *
 * RX pin -> USART DR -> RXNE IRQ -> RingBuffer_WriteChar() -> Main/Application
 *
 * @param[in,out] huart    Pointer to UART handle structure.
 * @param[in,out] pRingBuf Pointer to initialised Ring Buffer handle.
 *
 * @retval STD_OK    RX Ring Buffer mode started successfully.
 * @retval STD_ERROR Invalid parameter or another RX path is active.
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

    /*
     * RXNE interrupt remains enabled continuously.
     * It is disabled only by UARTx_RxRingBufferStop().
     */
    huart->Instance->CR1 |= USARTx_CR1_RXNEIE;

    return STD_OK;
}

/**
 * @brief  Stop continuous UART RX Ring Buffer mode.
 *
 * Disables RXNE interrupt and detaches the software Ring Buffer.
 * Stored Ring Buffer data is not automatically flushed.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval STD_OK    RX Ring Buffer mode stopped successfully.
 * @retval STD_ERROR Invalid parameter.
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
 *
 * This function executes in application context. The USART ISR is the producer
 * and this API is the consumer.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[out]    pData Pointer to destination byte.
 *
 * @retval RING_BUFFER_OK    Byte read successfully.
 * @retval RING_BUFFER_EMPTY No received data is available.
 * @retval RING_BUFFER_ERROR Invalid parameter or Ring Buffer not attached.
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
 *
 * The underlying generic Ring Buffer uses all-or-nothing behaviour.
 * If Length bytes are not yet available, no byte is consumed.
 *
 * @param[in,out] huart  Pointer to UART handle structure.
 * @param[out]    pData  Pointer to destination buffer.
 * @param[in]     Length Number of bytes to read.
 *
 * @retval RING_BUFFER_OK    Requested bytes read successfully.
 * @retval RING_BUFFER_EMPTY Not enough bytes are available.
 * @retval RING_BUFFER_ERROR Invalid parameter or Ring Buffer not attached.
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
 *
 * @param[in] huart Pointer to UART handle structure.
 *
 * @retval Number of unread received bytes.
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
 *
 * The counter increments whenever a new USART byte arrives while the software
 * Ring Buffer is full. The newly arrived byte is dropped.
 *
 * @param[in] huart Pointer to UART handle structure.
 *
 * @retval Number of RX bytes dropped because Ring Buffer was full.
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
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
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
 *
 * RX behaviour has two mutually exclusive paths:
 *
 * 1. Continuous Ring Buffer RX:
 *
 *    USART DR -> RingBuffer_WriteChar() -> return from IRQ
 *
 * 2. Legacy fixed-length interrupt RX:
 *
 *    USART DR -> pRxBuffPtr -> RxXferCount-- -> callback when complete
 *
 * TX keeps the original fixed-length interrupt transfer model.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
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

        /*
         * Reading DR consumes the received byte and clears RXNE.
         * Read DR exactly once, then dispatch that byte to the active software RX path.
         */
        rx_data = (uint8_t)(huart->Instance->DR & 0xFFU);

        if (huart->pRxRingBuffer != NULL)
        {
            /*
             * Continuous RX Ring Buffer mode.
             *
             * ISR is the single Producer:
             * - read one hardware byte
             * - push one software byte
             * - do not parse protocol here
             */
            if (RingBuffer_WriteChar(huart->pRxRingBuffer,
                                     rx_data) == RING_BUFFER_FULL)
            {
                /*
                 * Drop-new overflow policy.
                 *
                 * Existing unread data is preserved.
                 * The newly received byte is discarded and counted for diagnostics.
                 */
                huart->RxOverflowCount++;
            }
        }
        else if ((huart->RxXferCount > 0U) &&
                 (huart->pRxBuffPtr != NULL))
        {
            /* Legacy fixed-length interrupt reception. */
            *huart->pRxBuffPtr = rx_data;

            huart->pRxBuffPtr++;
            huart->RxXferCount--;

            if (huart->RxXferCount == 0U)
            {
                /* Stop RXNE interrupt after requested Size bytes are received. */
                huart->Instance->CR1 &= ~USARTx_CR1_RXNEIE;

                UARTx_RxCpltCallback(huart);
            }
        }
        else
        {
            /*
             * No software RX consumer is active.
             *
             * DR has already been read above, therefore the byte is safely discarded
             * instead of leaving RXNE pending and risking an interrupt storm/overrun.
             */
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
                /*
                 * No more application bytes remain.
                 * Stop TXE interrupt and wait for TC so the final frame leaves TX completely.
                 */
                huart->Instance->CR1 &= ~USARTx_CR1_TXEIE;
                huart->Instance->CR1 |= USARTx_CR1_TCIE;
            }
        }
        else
        {
            /* Defensive stop if TXE interrupt is enabled without an active transfer. */
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
 *
 * The DMA driver is responsible for channel-level flags. This UART helper
 * disables the peripheral DMA request and dispatches the corresponding callback.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[in]     is_tx 1 for TX DMA channel, 0 for RX DMA channel.
 *
 * @retval None
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

/**
 * @brief  UART transmit-complete weak callback.
 *
 * Application code may override this function.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
 */
__attribute__((weak)) void UARTx_TxCpltCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

/**
 * @brief  UART receive-complete weak callback.
 *
 * Called by legacy fixed-length interrupt RX and DMA RX completion.
 * Continuous RX Ring Buffer mode intentionally does not call this callback
 * for every byte because the main/application consumes data asynchronously.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
 */
__attribute__((weak)) void UARTx_RxCpltCallback(UARTx_HandleTypeDef *huart)
{
    (void)huart;
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------- End Of File -----------------------------------------------------*/

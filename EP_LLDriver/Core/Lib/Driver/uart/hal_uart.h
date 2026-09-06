/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: hal_uart.h
 *
 * Description: HAL UART Driver - Data types, configuration structures and API prototypes
 *              for STM32F103xB USART peripherals.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           10/08/2026          Initial release for STM32F103xB UART Module
 *              2.0.0           04/09/2026          Added optimized RX Ring Buffer support
 *
 **********************************************************************************************************************/

#ifndef HAL_UART_H_
#define HAL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                    INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stddef.h>

#include "hal_dma.h"
#include "hal_ringbuffer.h"

/***********************************************************************************************************************
 *                                                  MACROS
 **********************************************************************************************************************/

/**
 * @brief  Check valid USART instance.
 */
#define IS_UART_INSTANCE(INSTANCE)             (((INSTANCE) == USART1x) || \
                                                ((INSTANCE) == USART2x) || \
                                                ((INSTANCE) == USART3x))

/***********************************************************************************************************************
 *                                   TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief  UART Word Length Configuration Enum.
 */
typedef enum
{
    UARTx_WordLength_8B = 0x00000000U,
    UARTx_WordLength_9B = (1U << 12U)
} UARTx_WordLength_t;

/**
 * @brief  UART Stop Bits Configuration Enum.
 */
typedef enum
{
    UARTx_StopBits_1   = 0x00000000U,
    UARTx_StopBits_0_5 = (1U << 12U),
    UARTx_StopBits_2   = (2U << 12U),
    UARTx_StopBits_1_5 = (3U << 12U)
} UARTx_StopBits_t;

/**
 * @brief  UART Parity Selection Enum.
 */
typedef enum
{
    UARTx_Parity_NONE  = 0x00000000U,
    UARTx_Parity_EVEN  = (1U << 10U),
    UARTx_Parity_ODD   = ((1U << 10U) | (1U << 9U)),
    UARTx_Parity_MARK  = 0xFFFFFFFFU,
    UARTx_Parity_SPACE = 0xFFFFFFFEU
} UARTx_Parity_t;

/**
 * @brief  UART Transfer Mode Selection Enum.
 */
typedef enum
{
    UARTx_Mode_RX    = (1U << 2U),
    UARTx_Mode_TX    = (1U << 3U),
    UARTx_Mode_TX_RX = ((1U << 2U) | (1U << 3U))
} UARTx_Mode_t;

/**
 * @brief  UART Hardware Flow Control Enum.
 */
typedef enum
{
    UARTx_HwFlowCtrl_NONE    = 0x00000000U,
    UARTx_HwFlowCtrl_RTS     = (1U << 8U),
    UARTx_HwFlowCtrl_CTS     = (1U << 9U),
    UARTx_HwFlowCtrl_RTS_CTS = ((1U << 8U) | (1U << 9U))
} UARTx_HwFlowCtl_t;

/**
 * @brief  UART Status Flags Mask Enum.
 */
typedef enum
{
    UARTx_Flag_PE   = (1U << 0U),
    UARTx_Flag_FE   = (1U << 1U),
    UARTx_Flag_NE   = (1U << 2U),
    UARTx_Flag_ORE  = (1U << 3U),
    UARTx_Flag_IDLE = (1U << 4U),
    UARTx_Flag_RXNE = (1U << 5U),
    UARTx_Flag_TC   = (1U << 6U),
    UARTx_Flag_TXE  = (1U << 7U),
    UARTx_Flag_CTS  = (1U << 9U)
} UART_Flag_t;

/**
 * @brief  UART Initialization Parameters Structure.
 */
typedef struct
{
    uint32_t            BaudRate;       /*!< UART communication baud rate. */
    UARTx_WordLength_t  WordLength;     /*!< Number of data bits transmitted/received. */
    UARTx_StopBits_t    StopBits;       /*!< Number of stop bits. */
    UARTx_Parity_t      Parity;         /*!< Parity configuration. */
    UARTx_Mode_t        Mode;           /*!< RX, TX or TX/RX mode. */
    UARTx_HwFlowCtl_t   HwFlowCtl;      /*!< RTS/CTS hardware flow control. */
} UARTx_InitTypeDef;

/**
 * @brief  UART DMA Configuration Handle Structure.
 */
typedef struct
{
    DMAx_Channel_TypeDef_t *Channel;    /*!< Pointer to associated DMA channel. */
    uint8_t                 IRQn;       /*!< DMA channel NVIC interrupt vector number. */
} UARTx_DMA_Handle_t;

/**
 * @brief  UART Peripheral Handle Structure.
 *
 * The RX Ring Buffer path follows a Single Producer / Single Consumer model:
 *
 * USART RX IRQ -> Producer -> Ring Buffer -> Consumer -> Main/Application
 *
 * The USART interrupt only reads DR and pushes the received byte into the Ring Buffer.
 * Protocol parsing and application processing are performed outside the interrupt.
 */
typedef struct
{
    USARTx_TypeDef_t       *Instance;            /*!< UART register base address. */
    UARTx_InitTypeDef       Init;                /*!< UART communication parameters. */

    uint8_t                *pTxBuffPtr;          /*!< Pointer to UART interrupt TX buffer. */
    uint16_t                TxXferSize;          /*!< UART interrupt TX transfer size. */
    volatile uint16_t       TxXferCount;         /*!< Remaining interrupt TX bytes. */

    uint8_t                *pRxBuffPtr;          /*!< Pointer to legacy fixed-length interrupt RX buffer. */
    uint16_t                RxXferSize;          /*!< Legacy interrupt RX transfer size. */
    volatile uint16_t       RxXferCount;         /*!< Remaining legacy interrupt RX bytes. */

    RingBuffer_HandleTypeDef *pRxRingBuffer;     /*!< Attached continuous RX Ring Buffer. */
    volatile uint32_t       RxOverflowCount;     /*!< Number of bytes dropped because RX Ring Buffer was full. */

    UARTx_DMA_Handle_t      DmaTx;               /*!< UART TX DMA configuration. */
    UARTx_DMA_Handle_t      DmaRx;               /*!< UART RX DMA configuration. */

    volatile uint32_t       ErrorCode;           /*!< UART error code. */
} UARTx_HandleTypeDef;

/***********************************************************************************************************************
 *                                        GLOBAL VARIABLE DECLARATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/**
 * @brief  Initialise the UART peripheral.
 *
 * Configures word length, parity, transfer mode, stop bits, hardware flow control,
 * baud rate and enables the USART peripheral.
 *
 * @param[in,out] huart Pointer to UARTx_HandleTypeDef structure.
 *
 * @retval STD_OK    UART initialised successfully.
 * @retval STD_ERROR Invalid parameter.
 */
HALx_StatusTypeDef UARTx_Init(UARTx_HandleTypeDef *huart);

/**
 * @brief  Transmit an amount of data in blocking mode.
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
                                  uint32_t Timeout);

/**
 * @brief  Receive an amount of data in blocking mode.
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
                                 uint32_t Timeout);

/**
 * @brief  Start a fixed-length UART transmission in interrupt mode.
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
                                     uint16_t Size);

/**
 * @brief  Start a fixed-length UART reception in interrupt mode.
 *
 * This legacy API receives exactly Size bytes and then disables RXNE interrupt.
 * It cannot be used while continuous RX Ring Buffer mode is active.
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
                                    uint16_t Size);

/**
 * @brief  Start UART transmission in DMA mode.
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
                                      uint16_t Size);

/**
 * @brief  Start UART reception in DMA mode.
 *
 * DMA RX and RX Ring Buffer interrupt mode cannot consume USART DR simultaneously.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[out]    pData Pointer to destination data buffer.
 * @param[in]     Size  Number of bytes to receive.
 *
 * @retval STD_OK    DMA request enabled successfully.
 * @retval STD_ERROR Invalid parameter or RX Ring Buffer mode active.
 */
HALx_StatusTypeDef UARTx_Receive_DMA(UARTx_HandleTypeDef *huart,
                                     uint8_t *pData,
                                     uint16_t Size);

/**
 * @brief  Stop ongoing UART DMA transfer requests.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval STD_OK    DMA requests disabled successfully.
 * @retval STD_ERROR Invalid parameter.
 */
HALx_StatusTypeDef UARTx_DMAStop(UARTx_HandleTypeDef *huart);

/**
 * @brief  Start continuous UART RX using the generic Ring Buffer.
 *
 * The Ring Buffer shall already be initialised using RingBuffer_Init().
 * RXNE interrupt remains enabled continuously until UARTx_RxRingBufferStop() is called.
 *
 * @param[in,out] huart     Pointer to UART handle structure.
 * @param[in,out] pRingBuf  Pointer to initialised Ring Buffer handle.
 *
 * @retval STD_OK    RX Ring Buffer mode started successfully.
 * @retval STD_ERROR Invalid parameter or another RX transfer is active.
 */
HALx_StatusTypeDef UARTx_RxRingBufferStart(UARTx_HandleTypeDef *huart,
                                           RingBuffer_HandleTypeDef *pRingBuf);

/**
 * @brief  Stop continuous UART RX Ring Buffer mode.
 *
 * Disables RXNE interrupt and detaches the Ring Buffer from the UART handle.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval STD_OK    RX Ring Buffer mode stopped successfully.
 * @retval STD_ERROR Invalid parameter.
 */
HALx_StatusTypeDef UARTx_RxRingBufferStop(UARTx_HandleTypeDef *huart);

/**
 * @brief  Read one byte from the attached UART RX Ring Buffer.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[out]    pData Pointer to destination byte.
 *
 * @retval RING_BUFFER_OK    Byte read successfully.
 * @retval RING_BUFFER_EMPTY No received byte is available.
 * @retval RING_BUFFER_ERROR Invalid parameter or Ring Buffer not attached.
 */
RingBuffer_StatusTypeDef UARTx_RxReadByte(UARTx_HandleTypeDef *huart,
                                          uint8_t *pData);

/**
 * @brief  Read an exact number of bytes from the attached UART RX Ring Buffer.
 *
 * The underlying RingBuffer_Read() uses all-or-nothing behaviour.
 *
 * @param[in,out] huart  Pointer to UART handle structure.
 * @param[out]    pData  Pointer to destination buffer.
 * @param[in]     Length Number of bytes to read.
 *
 * @retval RING_BUFFER_OK    Requested bytes read successfully.
 * @retval RING_BUFFER_EMPTY Not enough received bytes are available.
 * @retval RING_BUFFER_ERROR Invalid parameter or Ring Buffer not attached.
 */
RingBuffer_StatusTypeDef UARTx_RxRead(UARTx_HandleTypeDef *huart,
                                      uint8_t *pData,
                                      uint16_t Length);

/**
 * @brief  Get the number of unread bytes in the UART RX Ring Buffer.
 *
 * @param[in] huart Pointer to UART handle structure.
 *
 * @retval Number of unread bytes.
 */
uint16_t UARTx_RxAvailable(const UARTx_HandleTypeDef *huart);

/**
 * @brief  Get the accumulated RX Ring Buffer overflow count.
 *
 * One count represents one received byte dropped because the software Ring Buffer was full.
 *
 * @param[in] huart Pointer to UART handle structure.
 *
 * @retval Number of dropped RX bytes.
 */
uint32_t UARTx_GetRxOverflowCount(const UARTx_HandleTypeDef *huart);

/**
 * @brief  Clear the accumulated RX Ring Buffer overflow count.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
 */
void UARTx_ClearRxOverflowCount(UARTx_HandleTypeDef *huart);

/**
 * @brief  Handle UART interrupt requests for the specified instance.
 *
 * RX Ring Buffer mode:
 * USART DR -> RingBuffer_WriteChar() -> return from IRQ.
 *
 * Legacy fixed-length RX interrupt mode remains supported when no Ring Buffer is attached.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
 */
void UARTx_IRQHandler(UARTx_HandleTypeDef *huart);

/**
 * @brief  Handle DMA interrupt completion associated with UART TX or RX.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 * @param[in]     is_tx 1 for TX DMA channel, 0 for RX DMA channel.
 *
 * @retval None
 */
void UARTx_DMA_IRQHandler(UARTx_HandleTypeDef *huart,
                           uint8_t is_tx);

/**
 * @brief  UART transmit-complete weak callback.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
 */
void UARTx_TxCpltCallback(UARTx_HandleTypeDef *huart);

/**
 * @brief  UART receive-complete weak callback for legacy fixed-length RX/DMA mode.
 *
 * Continuous RX Ring Buffer mode does not call this callback for every received byte.
 *
 * @param[in,out] huart Pointer to UART handle structure.
 *
 * @retval None
 */
void UARTx_RxCpltCallback(UARTx_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/

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

#include "stm32f103xb.h"
#include "define.h"
#include "hal_dma.h"
#include "hal_ringbuffer.h"

/***********************************************************************************************************************
 *                                                  MACROS
 **********************************************************************************************************************/

#ifdef IS_UART_INSTANCE
#undef IS_UART_INSTANCE
#endif
#define IS_UART_INSTANCE(INSTANCE)             (((INSTANCE) == USART1x) || \
                                                ((INSTANCE) == USART2x) || \
                                                ((INSTANCE) == USART3x))

/* Mapping USART Registers Bitmask to Standard CMSIS */
#define USARTx_CR1_UE                          USART_CR1_UE
#define USARTx_CR1_M                           USART_CR1_M
#define USARTx_CR1_PCE                         USART_CR1_PCE
#define USARTx_CR1_PS                          USART_CR1_PS
#define USARTx_CR1_TE                          USART_CR1_TE
#define USARTx_CR1_RE                          USART_CR1_RE
#define USARTx_CR1_TXEIE                       USART_CR1_TXEIE
#define USARTx_CR1_TCIE                        USART_CR1_TCIE
#define USARTx_CR1_RXNEIE                      USART_CR1_RXNEIE
#define USARTx_CR1_PEIE                        USART_CR1_PEIE

#define USARTx_CR2_STOP_MASK                   USART_CR2_STOP

#define USARTx_CR3_RTSE                        USART_CR3_RTSE
#define USARTx_CR3_CTSE                        USART_CR3_CTSE
#define USARTx_CR3_DMAT                        USART_CR3_DMAT
#define USARTx_CR3_DMAR                        USART_CR3_DMAR

#define USARTx_SR_TXE                          USART_SR_TXE
#define USARTx_SR_TC                           USART_SR_TC
#define USARTx_SR_RXNE                         USART_SR_RXNE

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
 *                                             FUNCTION PROTOTYPES
 **********************************************************************************************************************/

HALx_StatusTypeDef UARTx_Init(UARTx_HandleTypeDef *huart);
HALx_StatusTypeDef UARTx_Transmit(UARTx_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HALx_StatusTypeDef UARTx_Receive(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HALx_StatusTypeDef UARTx_Transmit_IT(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef UARTx_Receive_IT(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef UARTx_Transmit_DMA(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef UARTx_Receive_DMA(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HALx_StatusTypeDef UARTx_DMAStop(UARTx_HandleTypeDef *huart);

HALx_StatusTypeDef UARTx_RxRingBufferStart(UARTx_HandleTypeDef *huart, RingBuffer_HandleTypeDef *pRingBuf);
HALx_StatusTypeDef UARTx_RxRingBufferStop(UARTx_HandleTypeDef *huart);
RingBuffer_StatusTypeDef UARTx_RxReadByte(UARTx_HandleTypeDef *huart, uint8_t *pData);
RingBuffer_StatusTypeDef UARTx_RxRead(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Length);
uint16_t UARTx_RxAvailable(const UARTx_HandleTypeDef *huart);
uint32_t UARTx_GetRxOverflowCount(const UARTx_HandleTypeDef *huart);
void UARTx_ClearRxOverflowCount(UARTx_HandleTypeDef *huart);

void UARTx_IRQHandler(UARTx_HandleTypeDef *huart);
void UARTx_DMA_IRQHandler(UARTx_HandleTypeDef *huart, uint8_t is_tx);
void UARTx_TxCpltCallback(UARTx_HandleTypeDef *huart);
void UARTx_RxCpltCallback(UARTx_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H_ */

/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_uart.h
 *
 * Description:  HAL UART Driver — Data types, register structures, bitmasks,
 *               and API prototypes for Universal Synchronous Asynchronous
 *               Receiver Transmitter (USART/UART) peripheral on STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          21/08/2026          Initial release for STM32F103xB UART Module
 *
 **********************************************************************************************************************/

#ifndef HAL_UART_H_
#define HAL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "../../../peripheral_Lib/Driver/dma/hal_dma.h"

/***********************************************************************************************************************
 *                                                  MACROS
 **********************************************************************************************************************/

/**
  * @brief Check valid USART Instance macro
  */
#define IS_UARTx_INSTANCE(INSTANCE) (((INSTANCE) == USART1x) || \
                                    ((INSTANCE) == USART2x) || \
                                    ((INSTANCE) == USART3x))


/**
  * @brief USART Control Register Bit Definitions
  */
#define USARTx_CR1_UE                     (1U << 13U)   /* USART Enable */
#define USARTx_CR1_M                      (1U << 12U)   /* Word Length */
#define USARTx_CR1_PCE                    (1U << 10U)   /* Parity Control Enable */
#define USARTx_CR1_PS                     (1U << 9U)    /* Parity Selection */
#define USARTx_CR1_PEIE                   (1U << 8U)    /* PE Interrupt Enable */
#define USARTx_CR1_TXEIE                  (1U << 7U)    /* TXE Interrupt Enable */
#define USARTx_CR1_TCIE                   (1U << 6U)    /* TC Interrupt Enable */
#define USARTx_CR1_RXNEIE                 (1U << 5U)    /* RXNE Interrupt Enable */
#define USARTx_CR1_TE                     (1U << 3U)    /* Transmitter Enable */
#define USARTx_CR1_RE                     (1U << 2U)    /* Receiver Enable */

#define USARTx_CR2_STOP_MASK              (3U << 12U)   /* STOP bits mask */

#define USARTx_CR3_RTSE                   (1U << 8U)    /* RTS Enable */
#define USARTx_CR3_CTSE                   (1U << 9U)    /* CTS Enable */
#define USARTx_CR3_DMAT                   (1U << 7U)    /* DMA Enable Transmitter */
#define USARTx_CR3_DMAR                   (1U << 6U)    /* DMA Enable Receiver */

#define USARTx_SR_TXE                     (1U << 7U)    /* Transmit data register empty */
#define USARTx_SR_TC                      (1U << 6U)    /* Transmission Complete */
#define USARTx_SR_RXNE                    (1U << 5U)    /* Read data register not empty */

// /* Dummy System Clock for BaudRate Calculation (Default 8MHz PCLK2 / PCLK1 assuming 8MHz HSI/HSE) */
// #ifndef PCLK1_FREQ
// #define PCLK1_FREQ                        8000000U
// #endif

// #ifndef PCLK2_FREQ
// #define PCLK2_FREQ                        8000000U
// #endif

/***********************************************************************************************************************
 *                                   TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
  * @brief UART Word Length Configuration Enum
  */
typedef enum 
{
    UARTx_WordLength_8B = 0x00000000U,
    UARTx_WordLength_9B = (1U << 12U)
} UARTx_WordLength_t;

/**
  * @brief UART Stop Bits Configuration Enum
  */
typedef enum 
{
    UARTx_StopBits_1   = 0x00000000U,
    UARTx_StopBits_0_5 = (1U << 12U),
    UARTx_StopBits_2   = (2U << 12U),
    UARTx_StopBits_1_5 = (3U << 12U)
} UARTx_StopBits_t;

/**
  * @brief UART Parity Selection Enum
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
  * @brief UART Transfer Mode Selection Enum
  */
typedef enum 
{
    UARTx_Mode_RX    = (1U << 2U),
    UARTx_Mode_TX    = (1U << 3U),
    UARTx_Mode_TX_RX = ((1U << 2U) | (1U << 3U))
} UARTx_Mode_t;

/**
  * @brief UART Hardware Flow Control Enum
  */
typedef enum 
{
    UARTx_HwFlowCtrl_NONE    = 0x00000000U,
    UARTx_HwFlowCtrl_RTS     = (1U << 8U),
    UARTx_HwFlowCtrl_CTS     = (1U << 9U),
    UARTx_HwFlowCtrl_RTS_CTS = ((1U << 8U) | (1U << 9U))
} UARTx_HwFlowCtl_t;

/**
  * @brief UART Status Flags Mask Enum
  */
typedef enum 
{
    UARTx_Flag_PE   = (1U << 0U),   /* Parity Error Flag */
    UARTx_Flag_FE   = (1U << 1U),   /* Framing Error Flag */
    UARTx_Flag_NE   = (1U << 2U),   /* Noise Error Flag */
    UARTx_Flag_ORE  = (1U << 3U),   /* Overrun Error Flag */
    UARTx_Flag_IDLE = (1U << 4U),   /* IDLE line detected Flag */
    UARTx_Flag_RXNE = (1U << 5U),   /* Read data register not empty Flag */
    UARTx_Flag_TC   = (1U << 6U),   /* Transmission Complete Flag */
    UARTx_Flag_TXE  = (1U << 7U),   /* Transmit data register empty Flag */
    UARTx_Flag_CTS  = (1U << 9U)    /* CTS Flag */
} UART_Flag_t;

/**
  * @brief UART Initialization Parameters Structure
  */
typedef struct 
{
    uint32_t            BaudRate;    /* Configures the UART communication baud rate */
    UARTx_WordLength_t  WordLength;  /* Specifies the number of data bits transmitted/received */
    UARTx_StopBits_t    StopBits;    /* Specifies the number of stop bits transmitted */
    UARTx_Parity_t      Parity;      /* Specifies the parity mode */
    UARTx_Mode_t        Mode;        /* Specifies whether Rx/Tx mode is enabled or disabled */
    UARTx_HwFlowCtl_t   HwFlowCtl;   /* Specifies whether Hardware Flow Control is enabled */
} UARTx_InitTypeDef;

/**
  * @brief UART DMA Configuration Handle Structure
  */
typedef struct 
{
    DMAx_Channel_TypeDef_t *Channel;  /* Pointer to associated DMA Channel register base */
    uint8_t                 IRQn;     /* NVIC Interrupt Vector Number for the DMA Channel */
} UARTx_DMA_Handle_t;

/**
  * @brief UART Peripheral Handle Structure
  */
typedef struct 
{
    USARTx_TypeDef_t   *Instance;    /* UART registers base address */
    UARTx_InitTypeDef   Init;        /* UART communication parameters */
    uint8_t           *pTxBuffPtr;  /* Pointer to UART Tx transfer buffer */
    uint16_t           TxXferSize;   /* UART Tx transfer size */
    volatile uint16_t   TxXferCount;  /* UART Tx transfer counter */
    uint8_t           *pRxBuffPtr;  /* Pointer to UART Rx transfer buffer */
    uint16_t           RxXferSize;   /* UART Rx transfer size */
    volatile uint16_t   RxXferCount;  /* UART Rx transfer counter */
    UARTx_DMA_Handle_t  DmaTx;       /* UART Tx DMA handle parameters */
    UARTx_DMA_Handle_t  DmaRx;       /* UART Rx DMA handle parameters */
    volatile uint32_t   ErrorCode;   /* UART Error code */
} UARTx_HandleTypeDef;

/***********************************************************************************************************************
 *                                        GLOBAL VARIABLE DECLARATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/**
  * @brief  Initializes the UART peripheral according to the specified parameters in UARTx_InitTypeDef.
  * @param  huart: Pointer to a UARTx_HandleTypeDef structure that contains configuration info.
  * @retval HALx_StatusTypeDef: STD_OK if initialization succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Init(UARTx_HandleTypeDef *huart);

/**
  * @brief  Transmits an amount of data in blocking mode (Polling).
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be sent.
  * @param  Timeout: Timeout duration in milliseconds.
  * @retval HALx_StatusTypeDef: STD_OK if transmission succeeded, STD_ERROR on parameter or timeout error.
  */
HALx_StatusTypeDef UARTx_Transmit(UARTx_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  Receives an amount of data in blocking mode (Polling).
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be received.
  * @param  Timeout: Timeout duration in milliseconds.
  * @retval HALx_StatusTypeDef: STD_OK if reception succeeded, STD_ERROR on parameter or timeout error.
  */
HALx_StatusTypeDef UARTx_Receive(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  Transmits an amount of data in non-blocking Interrupt mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be sent.
  * @retval HALx_StatusTypeDef: STD_OK if transfer started successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Transmit_IT(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

/**
  * @brief  Receives an amount of data in non-blocking Interrupt mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be received.
  * @retval HALx_StatusTypeDef: STD_OK if reception started successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Receive_IT(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

/**
  * @brief  Transmits an amount of data in non-blocking DMA mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be sent.
  * @retval HALx_StatusTypeDef: STD_OK if DMA transfer initiated successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Transmit_DMA(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

/**
  * @brief  Receives an amount of data in non-blocking DMA mode.
  * @param  huart: Pointer to UART handle structure.
  * @param  pData: Pointer to data buffer.
  * @param  Size: Amount of data elements to be received.
  * @retval HALx_StatusTypeDef: STD_OK if DMA reception initiated successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_Receive_DMA(UARTx_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

/**
  * @brief  Stops ongoing UART DMA transfer (Tx or Rx).
  * @param  huart: Pointer to UART handle structure.
  * @retval HALx_StatusTypeDef: STD_OK if DMA stop operation succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef UARTx_DMAStop(UARTx_HandleTypeDef *huart);

/**
  * @brief  Handles UART Interrupt Requests for specified instance.
  * @param  huart: Pointer to UART handle structure.
  * @retval None
  */
void UARTx_IRQHandler(UARTx_HandleTypeDef *huart);

/**
  * @brief  Handles DMA Interrupt Requests associated with UART Tx/Rx channels.
  * @param  huart: Pointer to UART handle structure.
  * @param  is_tx: Flag indicating channel type (1 for Tx channel, 0 for Rx channel).
  * @retval None
  */
void UARTx_DMA_IRQHandler(UARTx_HandleTypeDef *huart, uint8_t is_tx);

/**
  * @brief  Tx Transfer completed callback function.
  * @param  huart: Pointer to UART handle structure.
  * @retval None
  */
void UARTx_TxCpltCallback(UARTx_HandleTypeDef *huart);

/**
  * @brief  Rx Transfer completed callback function.
  * @param  huart: Pointer to UART handle structure.
  * @retval None
  */
void UARTx_RxCpltCallback(UARTx_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
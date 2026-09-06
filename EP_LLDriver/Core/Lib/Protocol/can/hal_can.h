/***********************************************************************************************************************
 * @file    hal_can.h
 * @brief   HAL CAN Driver Header File - Declarations for STM32F103xB devices (bxCAN).
 * 
 * @project STM32F103_BareMetal_Drivers
 * @compiler GCC
 *
 * @revision
 *          Version    Date          Author      Change History
 *          1.0.0      10/08/2026    Driver Dev  Initial release for CAN Module
 *
 **********************************************************************************************************************/

#ifndef HAL_CAN_H
#define HAL_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "define.h"

/***********************************************************************************************************************
 *                                           MACROS & CONSTANTS
 **********************************************************************************************************************/

/** 
 * @defgroup CAN_Identifier_Type CAN Identifier Types
 * @{
 */
#define CAN_ID_STD                     (0x00000000U) /*!< Standard 11-bit ID */
#define CAN_ID_EXT                     (0x00000004U) /*!< Extended 29-bit ID */
/** @} */

/** 
 * @defgroup CAN_Remote_Transmission_Request CAN Remote Transmission Request
 * @{
 */
#define CAN_RTR_DATA                   (0x00000000U) /*!< Data Frame */
#define CAN_RTR_REMOTE                 (0x00000002U) /*!< Remote Frame */
/** @} */

/** 
 * @defgroup CAN_Operating_Mode CAN Operating Modes
 * @{
 */
#define CAN_MODE_NORMAL                (0x00000000U) /*!< Normal Operating Mode */
#define CAN_MODE_LOOPBACK              (0x40000000U) /*!< Loopback Mode */
#define CAN_MODE_SILENT                (0x80000000U) /*!< Silent Mode */
#define CAN_MODE_SILENT_LOOPBACK       (0xC0000000U) /*!< Silent Loopback Mode */
/** @} */

/** 
 * @defgroup CAN_Receive_FIFO CAN Receive FIFOs
 * @{
 */
#define CAN_RX_FIFO0                   (0x00000000U) /*!< Receive FIFO 0 */
#define CAN_RX_FIFO1                   (0x00000001U) /*!< Receive FIFO 1 */
/** @} */

/** 
 * @defgroup CAN_Filter_Mode_And_Scale CAN Filter Modes & Scale Definitions
 * @{
 */
#define CAN_FILTERMODE_IDMASK          (0x00000000U) /*!< Identifier Mask Mode */
#define CAN_FILTERMODE_IDLIST          (0x00000001U) /*!< Identifier List Mode */
#define CAN_FILTERSCALE_16BIT          (0x00000000U) /*!< Two 16-bit Filters */
#define CAN_FILTERSCALE_32BIT          (0x00000001U) /*!< One 32-bit Filter */
#define CAN_FILTER_FIFO0               (0x00000000U) /*!< Filter assigned to FIFO 0 */
#define CAN_FILTER_FIFO1               (0x00000001U) /*!< Filter assigned to FIFO 1 */
#define CAN_FILTER_DISABLE             (0x00000000U) /*!< Filter disabled */
#define CAN_FILTER_ENABLE              (0x00000001U) /*!< Filter enabled */
/** @} */

/** 
 * @defgroup CAN_Interrupt_Enables CAN Interrupt Enable Bitfield Masks
 * @{
 */
#define CAN_IT_TX_MAILBOX_EMPTY        (1U << 0)     /*!< Transmit Mailbox Empty Interrupt */
#define CAN_IT_RX_FIFO0_MSG_PENDING    (1U << 1)     /*!< FIFO 0 Message Pending Interrupt */
#define CAN_IT_RX_FIFO1_MSG_PENDING    (1U << 4)     /*!< FIFO 1 Message Pending Interrupt */
#define CAN_IT_ERROR_WARNING           (1U << 8)     /*!< Error Warning Interrupt */
#define CAN_IT_ERROR_PASSIVE           (1U << 9)     /*!< Error Passive Interrupt */
#define CAN_IT_BUSOFF                  (1U << 10)    /*!< Bus-Off Interrupt */
/** @} */

/** 
 * @defgroup CAN_Driver_States CAN Driver Operating States
 * @{
 */
#define CAN_STATE_RESET                (0x00U)       /*!< CAN not initialized */
#define CAN_STATE_READY                (0x01U)       /*!< CAN initialized and ready */
#define CAN_STATE_LISTENING            (0x02U)       /*!< CAN active on bus */
/** @} */

/***********************************************************************************************************************
 *                                          DATATYPES & STRUCTURES
 **********************************************************************************************************************/

/**
 * @brief Generic Flag status enumeration
 */
typedef enum
{
    RESET = 0U,
    SET   = !RESET
} FlagStatus;

/**
 * @brief  Functional State definition (ENABLE / DISABLE).
 */
typedef enum
{
    FunctionalState_DISABLE = 0U,
    FunctionalState_ENABLE  = 1U
} FunctionalState;

/**
 * @brief CAN Tx Header Structure
 */
typedef struct
{
    uint32_t StdId;    /*!< Standard Identifier (11-bit: 0 to 0x7FF) */
    uint32_t ExtId;    /*!< Extended Identifier (29-bit: 0 to 0x1FFFFFFF) */
    uint32_t IDE;      /*!< Identifier Extension: CAN_ID_STD or CAN_ID_EXT */
    uint32_t RTR;      /*!< Remote Transmission Request: CAN_RTR_DATA or CAN_RTR_REMOTE */
    uint32_t DLC;      /*!< Data Length Code (0 to 8 bytes) */
} CAN_TxHeaderTypeDef;

/**
 * @brief CAN Rx Header Structure
 */
typedef struct
{
    uint32_t StdId;            /*!< Standard Identifier */
    uint32_t ExtId;            /*!< Extended Identifier */
    uint32_t IDE;              /*!< Identifier Extension: CAN_ID_STD or CAN_ID_EXT */
    uint32_t RTR;              /*!< Remote Transmission Request: CAN_RTR_DATA or CAN_RTR_REMOTE */
    uint32_t DLC;              /*!< Data Length Code */
    uint32_t FilterMatchIndex; /*!< Index of matching filter bank */
} CAN_RxHeaderTypeDef;

/**
 * @brief CAN Filter Configuration Structure
 */
typedef struct
{
    uint32_t FilterIdHigh;          /*!< MSB of ID (16-bit) */
    uint32_t FilterIdLow;           /*!< LSB of ID (16-bit) */
    uint32_t FilterMaskIdHigh;      /*!< MSB of Mask / Second ID (16-bit) */
    uint32_t FilterMaskIdLow;       /*!< LSB of Mask / Second ID (16-bit) */
    uint32_t FilterFIFOAssignment;  /*!< FIFO assignment: CAN_FILTER_FIFO0 or CAN_FILTER_FIFO1 */
    uint32_t FilterBank;            /*!< Filter bank index (0 to 13) */
    uint32_t FilterMode;            /*!< Filter mode: CAN_FILTERMODE_IDMASK or CAN_FILTERMODE_IDLIST */
    uint32_t FilterScale;           /*!< Filter scale: CAN_FILTERSCALE_16BIT or CAN_FILTERSCALE_32BIT */
    uint32_t FilterActivation;      /*!< Filter state: CAN_FILTER_DISABLE or CAN_FILTER_ENABLE */
} CAN_FilterTypeDef;

/**
 * @brief CAN Initialization Configuration Structure
 */
typedef struct
{
    uint32_t Prescaler;                  /*!< Baud Rate Prescaler BRP (1 to 1024) */
    uint32_t Mode;                       /*!< Operating Mode: CAN_MODE_NORMAL, CAN_MODE_LOOPBACK, etc. */
    uint32_t SyncJumpWidth;              /*!< Resynchronization Jump Width: CAN_SJW_1TQ .. CAN_SJW_4TQ */
    uint32_t TimeSeg1;                   /*!< Time Segment 1: CAN_BS1_1TQ .. CAN_BS1_16TQ */
    uint32_t TimeSeg2;                   /*!< Time Segment 2: CAN_BS2_1TQ .. CAN_BS2_8TQ */
    FunctionalState AutoBusOff;          /*!< Automatic Bus-Off management: ENABLE / DISABLE */
    FunctionalState AutoWakeUp;          /*!< Automatic Wake-Up mode: ENABLE / DISABLE */
    FunctionalState AutoRetransmission;  /*!< Automatic Retransmission: ENABLE / DISABLE */
    FunctionalState ReceiveFifoLocked;   /*!< Receive FIFO Locked mode: ENABLE / DISABLE */
    FunctionalState TransmitFifoPriority;/*!< Transmit FIFO Priority mode: ENABLE / DISABLE */
} CAN_InitTypeDef;

/**
 * @brief CAN Handle Structure
 */
typedef struct
{
    CANx_TypeDef_t      *Instance;     /*!< Register base address */
    CAN_InitTypeDef     Init;          /*!< CAN initialization parameters */
    volatile uint32_t   ErrorCode;     /*!< CAN Error code */
    volatile uint8_t     State;         /*!< CAN Driver State */
} CANx_HandleTypeDef;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/**
 * @brief  Initializes the CAN peripheral according to the specified parameters in the CAN_InitTypeDef.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing configuration info.
 * @retval HALx_StatusTypeDef: HAL status (HAL_OK, HAL_ERROR, HAL_BUSY, etc.).
 */
HALx_StatusTypeDef CANx_Init(CANx_HandleTypeDef *hcan);

/**
 * @brief  De-initializes the CAN peripheral registers to their default reset values.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_DeInit(CANx_HandleTypeDef *hcan);

/**
 * @brief  Configures the CAN reception filter bank according to specified parameters.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @param  sFilterConfig: Pointer to a CAN_FilterTypeDef structure containing filter parameters.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_ConfigFilter(CANx_HandleTypeDef *hcan, const CAN_FilterTypeDef *sFilterConfig);

/**
 * @brief  Starts the CAN module and enables communication on the bus.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_Start(CANx_HandleTypeDef *hcan);

/**
 * @brief  Stops the CAN module and disables communication on the bus.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_Stop(CANx_HandleTypeDef *hcan);

/* Data Transfer APIs */

/**
 * @brief  Adds a message to the first free Tx Mailbox and activates the corresponding transmission request.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @param  pHeader: Pointer to a CAN_TxHeaderTypeDef structure containing header info.
 * @param  aData: Array containing the payload data bytes (0 to 8 bytes).
 * @param  pTxMailbox: Pointer to variable where the allocated Tx mailbox index will be stored.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_AddTxMessage(CANx_HandleTypeDef *hcan, const CAN_TxHeaderTypeDef *pHeader, const uint8_t aData[], uint32_t *pTxMailbox);

/**
 * @brief  Gets an Rx message from the specified Receive FIFO mailbox.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @param  RxFifo: Receive FIFO number (CAN_RX_FIFO0 or CAN_RX_FIFO1).
 * @param  pHeader: Pointer to a CAN_RxHeaderTypeDef structure to store message header info.
 * @param  aData: Array where the received payload data bytes will be copied.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_GetRxMessage(CANx_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader, uint8_t aData[]);

/* Interrupt Control & Handlers */

/**
 * @brief  Enables active interrupts on the CAN peripheral.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @param  ActiveITs: Combination of interrupt flags to enable (e.g., CAN_IT_RX_FIFO0_MSG_PENDING).
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_ActivateNotification(CANx_HandleTypeDef *hcan, uint32_t ActiveITs);

/**
 * @brief  Disables active interrupts on the CAN peripheral.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 * @param  InactiveITs: Combination of interrupt flags to disable.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef CANx_DeactivateNotification(CANx_HandleTypeDef *hcan, uint32_t InactiveITs);

/**
 * @brief  Handles CAN Transmit Interrupt requests.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_TX_IRQHandler(CANx_HandleTypeDef *hcan);

/**
 * @brief  Handles CAN Receive FIFO 0 Interrupt requests.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_RX0_IRQHandler(CANx_HandleTypeDef *hcan);

/**
 * @brief  Handles CAN Receive FIFO 1 Interrupt requests.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_RX1_IRQHandler(CANx_HandleTypeDef *hcan);

/**
 * @brief  Handles CAN Status Change and Error (SCE) Interrupt requests.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_SCE_IRQHandler(CANx_HandleTypeDef *hcan);

/* Weak Callbacks */

/**
 * @brief  Tx Mailbox 0 Complete callback.
 * @note   Weak function that can be overridden in user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_TxMailbox0CompleteCallback(CANx_HandleTypeDef *hcan);

/**
 * @brief  Rx FIFO 0 Message Pending callback.
 * @note   Weak function that can be overridden in user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_RxFifo0MsgPendingCallback(CANx_HandleTypeDef *hcan);

/**
 * @brief  Rx FIFO 1 Message Pending callback.
 * @note   Weak function that can be overridden in user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_RxFifo1MsgPendingCallback(CANx_HandleTypeDef *hcan);

/**
 * @brief  CAN Error callback.
 * @note   Weak function that can be overridden in user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure.
 */
void CANx_ErrorCallback(CANx_HandleTypeDef *hcan);

#ifdef __cplusplus
}
#endif

#endif /* HAL_CAN_H */

/***********************************************************************************************************************
 * @file    hal_can.c
 * @brief   CAN Hardware Abstraction Layer (HAL) Driver Implementation.
 * @details Target MCU: STM32F103xB (bxCAN peripheral).
 *          This module provides bare-metal functional implementations for initializing, configuring filter banks,
 *          transmitting/receiving CAN frames, and handling interrupts for the Controller Area Network peripheral.
 *
 * @version 1.0.0
 * @date    2026-08-10
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include "hal_can.h"

/***********************************************************************************************************************
 *                                            SOURCE FILE VERSION
 **********************************************************************************************************************/

#define HAL_CAN_SW_MAJOR_VERSION    (1U)
#define HAL_CAN_SW_MINOR_VERSION    (0U)
#define HAL_CAN_SW_PATCH_VERSION    (0U)

/***********************************************************************************************************************
 *                                           FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                              LOCAL MACROS
 **********************************************************************************************************************/

#define CAN_TIMEOUT_VALUE            (10000U) /*!< Timeout loop limit for register polling operations */

/* Master Control Register (MCR) Bit Definitions */
#define CAN_MCR_INRQ                 (1U << 0) /*!< Initialization Request bit                        */
#define CAN_MCR_SLEEP                (1U << 1) /*!< Sleep Mode Request bit                            */
#define CAN_MCR_TXFP                 (1U << 2) /*!< Transmit FIFO Priority control bit                */
#define CAN_MCR_RFLM                 (1U << 3) /*!< Receive FIFO Locked Mode bit                      */
#define CAN_MCR_NART                 (1U << 4) /*!< No Automatic Retransmission bit                   */
#define CAN_MCR_AWUM                 (1U << 5) /*!< Automatic Wake-Up Mode bit                        */
#define CAN_MCR_ABOM                 (1U << 6) /*!< Automatic Bus-Off Management bit                  */

/* Master Status Register (MSR) Bit Definitions */
#define CAN_MSR_INAK                 (1U << 0) /*!< Initialization Acknowledge bit                    */
#define CAN_MSR_SLAK                 (1U << 1) /*!< Sleep Acknowledge bit                             */

/* Transmit Status Register (TSR) Bit Definitions */
#define CAN_TSR_TME0                 (1U << 26) /*!< Transmit Mailbox 0 Empty bit                     */
#define CAN_TSR_TME1                 (1U << 27) /*!< Transmit Mailbox 1 Empty bit                     */
#define CAN_TSR_TME2                 (1U << 28) /*!< Transmit Mailbox 2 Empty bit                     */

/* Receive FIFO 0 / FIFO 1 Register (RF0R / RF1R) Bit Definitions */
#define CAN_RFR_FMP_Msk              (0x03U)    /*!< Filter Match Pending Mask                        */
#define CAN_RFR_RFOM                 (1U << 5)  /*!< Release Output Mailbox bit                       */

/* Filter Master Register (FMR) Bit Definitions */
#define CAN_FMR_FINIT                (1U << 0)  /*!< Filter Initialization Mode Request bit           */

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

/***********************************************************************************************************************
 *                                            LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**
 * @brief  Initializes the CAN peripheral according to the specified parameters in hcan.
 * @details Requests initialization mode, configures operational features (Auto Bus-Off, Auto Wake-Up, NART, RFLM, TXFP),
 *          and programs bit timing parameters into the BTR register.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Initialization successful.
 *         - STD_ERROR: Invalid handle or instance pointer (NULL).
 *         - STD_TIMEOUT: Peripheral failed to exit Sleep mode or enter Initialization mode.
 */
HALx_StatusTypeDef CANx_Init(CANx_HandleTypeDef *hcan)
{
    if ((hcan == NULL) || (hcan->Instance == NULL))
    {
        return STD_ERROR;
    }

    /* 1. Exit Sleep mode */
    hcan->Instance->MCR &= ~CAN_MCR_SLEEP;

    uint32_t timeout = CAN_TIMEOUT_VALUE;
    while ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U)
    {
        if (--timeout == 0U) return STD_TIMEOUT;
    }

    /* 2. Request Initialization */
    hcan->Instance->MCR |= CAN_MCR_INRQ;

    timeout = CAN_TIMEOUT_VALUE;
    while ((hcan->Instance->MSR & CAN_MSR_INAK) == 0U)
    {
        if (--timeout == 0U) return STD_TIMEOUT;
    }

    /* 3. Configure MCR Operating Parameters */
    if (hcan->Init.AutoBusOff == FunctionalState_ENABLE)            hcan->Instance->MCR |= CAN_MCR_ABOM;
    else                                            hcan->Instance->MCR &= ~CAN_MCR_ABOM;

    if (hcan->Init.AutoWakeUp == FunctionalState_ENABLE)            hcan->Instance->MCR |= CAN_MCR_AWUM;
    else                                            hcan->Instance->MCR &= ~CAN_MCR_AWUM;

    if (hcan->Init.AutoRetransmission == FunctionalState_DISABLE)   hcan->Instance->MCR |= CAN_MCR_NART;
    else                                            hcan->Instance->MCR &= ~CAN_MCR_NART;

    if (hcan->Init.ReceiveFifoLocked == FunctionalState_ENABLE)     hcan->Instance->MCR |= CAN_MCR_RFLM;
    else                                            hcan->Instance->MCR &= ~CAN_MCR_RFLM;

    if (hcan->Init.TransmitFifoPriority == FunctionalState_ENABLE)  hcan->Instance->MCR |= CAN_MCR_TXFP;
    else                                            hcan->Instance->MCR &= ~CAN_MCR_TXFP;

    /* 4. Configure Bit Timing (BTR) */
    hcan->Instance->BTR = (uint32_t)(hcan->Init.Mode |
                                     hcan->Init.SyncJumpWidth |
                                     hcan->Init.TimeSeg1 |
                                     hcan->Init.TimeSeg2 |
                                     (hcan->Init.Prescaler - 1U));

    hcan->State = CAN_STATE_READY;
    return STD_OK;
}

/**
 * @brief  Starts the CAN module by exiting Initialization mode and entering Normal/Listening mode.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Module started successfully.
 *         - STD_ERROR: Invalid handle pointer or driver is not in READY state.
 *         - STD_TIMEOUT: Peripheral failed to exit Initialization mode.
 */
HALx_StatusTypeDef CANx_Start(CANx_HandleTypeDef *hcan)
{
    if (hcan == NULL) return STD_ERROR;

    if (hcan->State == CAN_STATE_READY)
    {
        /* Clear Initialization Request */
        hcan->Instance->MCR &= ~CAN_MCR_INRQ;

        uint32_t timeout = CAN_TIMEOUT_VALUE;
        while ((hcan->Instance->MSR & CAN_MSR_INAK) != 0U)
        {
            if (--timeout == 0U) return STD_TIMEOUT;
        }

        hcan->State = CAN_STATE_LISTENING;
        return STD_OK;
    }
    return STD_ERROR;
}

/**
 * @brief  Stops the CAN module and returns it to Initialization mode.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Module stopped successfully.
 *         - STD_ERROR: Invalid handle pointer.
 *         - STD_TIMEOUT: Peripheral failed to enter Initialization mode.
 */
HALx_StatusTypeDef CANx_Stop(CANx_HandleTypeDef *hcan)
{
    if (hcan == NULL) return STD_ERROR;

    /* Request Initialization mode to stop CAN communication */
    hcan->Instance->MCR |= CAN_MCR_INRQ;

    uint32_t timeout = CAN_TIMEOUT_VALUE;
    while ((hcan->Instance->MSR & CAN_MSR_INAK) == 0U)
    {
        if (--timeout == 0U) return STD_TIMEOUT;
    }

    hcan->State = CAN_STATE_READY;
    return STD_OK;
}

/**
 * @brief  Configures the CAN hardware filter bank according to the specified parameters.
 * @details Configures scale (16-bit or 32-bit), mode (Mask or List), FIFO assignment, and filter activation.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @param  sFilterConfig: Pointer to a CAN_FilterTypeDef structure containing the filter configuration.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Filter configured successfully.
 *         - STD_ERROR: Invalid parameter or filter bank index out of range (> 13).
 */
HALx_StatusTypeDef CANx_ConfigFilter(CANx_HandleTypeDef *hcan, const CAN_FilterTypeDef *sFilterConfig)
{
    if ((hcan == NULL) || (sFilterConfig == NULL) || (sFilterConfig->FilterBank > 13U))
    {
        return STD_ERROR;
    }

    uint32_t filtermaskbitshift = (1U << sFilterConfig->FilterBank);

    /* Enter Filter Initialization mode */
    hcan->Instance->FMR |= CAN_FMR_FINIT;

    /* Deactivate selected filter bank */
    hcan->Instance->FA1R &= ~filtermaskbitshift;

    /* Filter Scale Setup (16-bit / 32-bit) */
    if (sFilterConfig->FilterScale == CAN_FILTERSCALE_16BIT)
    {
        hcan->Instance->FS1R &= ~filtermaskbitshift;
        hcan->Instance->FilterRegister[sFilterConfig->FilterBank].FR1 =
            ((sFilterConfig->FilterMaskIdLow & 0xFFFFU) << 16U) | (sFilterConfig->FilterIdLow & 0xFFFFU);
        hcan->Instance->FilterRegister[sFilterConfig->FilterBank].FR2 =
            ((sFilterConfig->FilterMaskIdHigh & 0xFFFFU) << 16U) | (sFilterConfig->FilterIdHigh & 0xFFFFU);
    }
    else
    {
        hcan->Instance->FS1R |= filtermaskbitshift;
        hcan->Instance->FilterRegister[sFilterConfig->FilterBank].FR1 =
            ((sFilterConfig->FilterIdHigh & 0xFFFFU) << 16U) | (sFilterConfig->FilterIdLow & 0xFFFFU);
        hcan->Instance->FilterRegister[sFilterConfig->FilterBank].FR2 =
            ((sFilterConfig->FilterMaskIdHigh & 0xFFFFU) << 16U) | (sFilterConfig->FilterMaskIdLow & 0xFFFFU);
    }

    /* Filter Mode (IdMask or IdList) */
    if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDLIST)
    {
        hcan->Instance->FM1R |= filtermaskbitshift;
    }
    else
    {
        hcan->Instance->FM1R &= ~filtermaskbitshift;
    }

    /* FIFO Assignment */
    if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO1)
    {
        hcan->Instance->FFA1R |= filtermaskbitshift;
    }
    else
    {
        hcan->Instance->FFA1R &= ~filtermaskbitshift;
    }

    /* Activation */
    if (sFilterConfig->FilterActivation == CAN_FILTER_ENABLE)
    {
        hcan->Instance->FA1R |= filtermaskbitshift;
    }

    /* Leave Filter Initialization mode */
    hcan->Instance->FMR &= ~CAN_FMR_FINIT;

    return STD_OK;
}

/**
 * @brief  Places a CAN frame into an empty transmit mailbox and requests transmission.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @param  pHeader: Pointer to a CAN_TxHeaderTypeDef structure containing message metadata.
 * @param  aData: Array containing payload bytes (up to 8 bytes).
 * @param  pTxMailbox: Pointer to store the bitmask corresponding to the allocated transmit mailbox.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Transmission request submitted successfully.
 *         - STD_ERROR: Invalid parameter.
 *         - STD_BUSY: All transmit mailboxes are full.
 */
HALx_StatusTypeDef CANx_AddTxMessage(CANx_HandleTypeDef *hcan, const CAN_TxHeaderTypeDef *pHeader, const uint8_t aData[], uint32_t *pTxMailbox)
{
    if ((hcan == NULL) || (pHeader == NULL) || (aData == NULL))
    {
        return STD_ERROR;
    }

    uint32_t tsr = hcan->Instance->TSR;
    uint32_t transmitmailbox = 4U;

    /* Select Empty Mailbox */
    if ((tsr & CAN_TSR_TME0) != 0U)      transmitmailbox = 0U;
    else if ((tsr & CAN_TSR_TME1) != 0U) transmitmailbox = 1U;
    else if ((tsr & CAN_TSR_TME2) != 0U) transmitmailbox = 2U;

    if (transmitmailbox < 3U)
    {
        if (pTxMailbox != NULL) *pTxMailbox = (1U << transmitmailbox);

        /* Setup Identifier */
        if (pHeader->IDE == CAN_ID_STD)
        {
            hcan->Instance->TxMailbox[transmitmailbox].TIR = ((pHeader->StdId & 0x7FFU) << 21U) | pHeader->RTR;
        }
        else
        {
            hcan->Instance->TxMailbox[transmitmailbox].TIR = ((pHeader->ExtId & 0x1FFFFFFFU) << 3U) | pHeader->IDE | pHeader->RTR;
        }

        /* Setup DLC */
        hcan->Instance->TxMailbox[transmitmailbox].TDTR = (pHeader->DLC & 0x0FU);

        /* Load Payload Data */
        hcan->Instance->TxMailbox[transmitmailbox].TDLR =
            (((uint32_t)aData[3] << 24U) | ((uint32_t)aData[2] << 16U) | ((uint32_t)aData[1] << 8U) | (uint32_t)aData[0]);
        hcan->Instance->TxMailbox[transmitmailbox].TDHR =
            (((uint32_t)aData[7] << 24U) | ((uint32_t)aData[6] << 16U) | ((uint32_t)aData[5] << 8U) | (uint32_t)aData[4]);

        /* Request Transmission */
        hcan->Instance->TxMailbox[transmitmailbox].TIR |= (1U << 0);

        return STD_OK;
    }

    return STD_BUSY;
}

/**
 * @brief  Retrieves a received CAN frame from the specified RX FIFO mailbox and releases the FIFO entry.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @param  RxFifo: Target RX FIFO (CAN_RX_FIFO0 or CAN_RX_FIFO1).
 * @param  pHeader: Pointer to a CAN_RxHeaderTypeDef structure where frame headers will be stored.
 * @param  aData: Buffer where payload bytes will be copied.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Frame retrieved successfully.
 *         - STD_ERROR: Invalid parameter or selected FIFO has no pending messages.
 */
HALx_StatusTypeDef CANx_GetRxMessage(CANx_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader, uint8_t aData[])
{
    if ((hcan == NULL) || (pHeader == NULL) || (aData == NULL) || (RxFifo > CAN_RX_FIFO1))
    {
        return STD_ERROR;
    }

    /* Check message pending in FIFO */
    if (RxFifo == CAN_RX_FIFO0)
    {
        if ((hcan->Instance->RF0R & CAN_RFR_FMP_Msk) == 0U) return STD_ERROR;
    }
    else
    {
        if ((hcan->Instance->RF1R & CAN_RFR_FMP_Msk) == 0U) return STD_ERROR;
    }

    /* Get Identifier & Attributes */
    pHeader->IDE = hcan->Instance->FIFOMailbox[RxFifo].RIR & CAN_ID_EXT;
    if (pHeader->IDE == CAN_ID_STD)
    {
        pHeader->StdId = (hcan->Instance->FIFOMailbox[RxFifo].RIR >> 21U) & 0x7FFU;
    }
    else
    {
        pHeader->ExtId = (hcan->Instance->FIFOMailbox[RxFifo].RIR >> 3U) & 0x1FFFFFFFU;
    }

    pHeader->RTR = hcan->Instance->FIFOMailbox[RxFifo].RIR & CAN_RTR_REMOTE;
    pHeader->DLC = hcan->Instance->FIFOMailbox[RxFifo].RDTR & 0x0FU;
    pHeader->FilterMatchIndex = (hcan->Instance->FIFOMailbox[RxFifo].RDTR >> 8U) & 0xFFU;

    /* Get Payload Data */
    uint32_t data_low  = hcan->Instance->FIFOMailbox[RxFifo].RDLR;
    uint32_t data_high = hcan->Instance->FIFOMailbox[RxFifo].RDHR;

    aData[0] = (uint8_t)(data_low & 0xFFU);
    aData[1] = (uint8_t)((data_low >> 8U) & 0xFFU);
    aData[2] = (uint8_t)((data_low >> 16U) & 0xFFU);
    aData[3] = (uint8_t)((data_low >> 24U) & 0xFFU);
    aData[4] = (uint8_t)(data_high & 0xFFU);
    aData[5] = (uint8_t)((data_high >> 8U) & 0xFFU);
    aData[6] = (uint8_t)((data_high >> 16U) & 0xFFU);
    aData[7] = (uint8_t)((data_high >> 24U) & 0xFFU);

    /* Release FIFO Mailbox */
    if (RxFifo == CAN_RX_FIFO0) hcan->Instance->RF0R |= CAN_RFR_RFOM;
    else                        hcan->Instance->RF1R |= CAN_RFR_RFOM;

    return STD_OK;
}

/**
 * @brief  Enables specified CAN interrupts in the Interrupt Enable Register (IER).
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @param  ActiveITs: Bitmask of interrupts to enable.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Interrupts activated successfully.
 *         - STD_ERROR: Invalid handle pointer.
 */
HALx_StatusTypeDef CANx_ActivateNotification(CANx_HandleTypeDef *hcan, uint32_t ActiveITs)
{
    if (hcan == NULL) return STD_ERROR;
    hcan->Instance->IER |= ActiveITs;
    return STD_OK;
}

/**
 * @brief  Disables specified CAN interrupts in the Interrupt Enable Register (IER).
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @param  InactiveITs: Bitmask of interrupts to disable.
 * @retval HALx_StatusTypeDef:
 *         - STD_OK: Interrupts deactivated successfully.
 *         - STD_ERROR: Invalid handle pointer.
 */
HALx_StatusTypeDef CANx_DeactivateNotification(CANx_HandleTypeDef *hcan, uint32_t InactiveITs)
{
    if (hcan == NULL) return STD_ERROR;
    hcan->Instance->IER &= ~InactiveITs;
    return STD_OK;
}

/**
 * @brief  Handles CAN Receive FIFO 0 Interrupt requests.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval None
 */
void CANx_RX0_IRQHandler(CANx_HandleTypeDef *hcan)
{
    if ((hcan->Instance->RF0R & CAN_RFR_FMP_Msk) != 0U)
    {
        CANx_RxFifo0MsgPendingCallback(hcan);
    }
}

/**
 * @brief  Handles CAN Receive FIFO 1 Interrupt requests.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval None
 */
void CANx_RX1_IRQHandler(CANx_HandleTypeDef *hcan)
{
    if ((hcan->Instance->RF1R & CAN_RFR_FMP_Msk) != 0U)
    {
        CANx_RxFifo1MsgPendingCallback(hcan);
    }
}

/**
 * @brief  Tx Mailbox 0 complete callback (Weak Function).
 * @note   Can be overridden by user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval None
 */
__attribute__((weak)) void CANx_TxMailbox0CompleteCallback(CANx_HandleTypeDef *hcan) { (void)hcan; }

/**
 * @brief  Rx FIFO 0 message pending callback (Weak Function).
 * @note   Can be overridden by user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval None
 */
__attribute__((weak)) void CANx_RxFifo0MsgPendingCallback(CANx_HandleTypeDef *hcan) { (void)hcan; }

/**
 * @brief  Rx FIFO 1 message pending callback (Weak Function).
 * @note   Can be overridden by user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval None
 */
__attribute__((weak)) void CANx_RxFifo1MsgPendingCallback(CANx_HandleTypeDef *hcan) { (void)hcan; }

/**
 * @brief  CAN error callback (Weak Function).
 * @note   Can be overridden by user application.
 * @param  hcan: Pointer to a CANx_HandleTypeDef structure containing CAN configuration.
 * @retval None
 */
__attribute__((weak)) void CANx_ErrorCallback(CANx_HandleTypeDef *hcan)              { (void)hcan; }

#ifdef __cplusplus
}
#endif

/***********************************************************************************************************************
 *                                                   END OF FILE
 **********************************************************************************************************************/

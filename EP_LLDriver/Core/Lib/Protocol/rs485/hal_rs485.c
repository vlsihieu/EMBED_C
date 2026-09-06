/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_rs485.c
 *
 * Description:  HAL RS485 Driver - Implementation for STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          10/08/2026          Initial release for RS485 Module
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include "hal_rs485.h"

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
  * @brief  Initializes RS485 Handle and attaches it to the configured UART instance.
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  huart: Pointer to base UART handle.
  * @param  dePin: GPIO Pin configuration for Driver Enable.
  * @param  polarity: Active polarity for DE Pin.
  * @param  SetPinFunc: Function pointer to write GPIO output (GPIO_WritePin).
  * @retval HALx_StatusTypeDef: STD_OK if initialized successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Init(RS485_HandleTypeDef *hrs485, UARTx_HandleTypeDef *huart, RS485_PinConfig_t dePin, RS485_DE_Polarity_t polarity, void (*SetPinFunc)(void*, uint16_t, bool))
{
    if ((hrs485 == NULL) || (huart == NULL) || (SetPinFunc == NULL))
    {
        return STD_ERROR;
    }

    hrs485->huart      = huart;
    hrs485->DE_Pin     = dePin;
    hrs485->Polarity   = polarity;
    hrs485->SetPinFunc = SetPinFunc;

    /* Initialize base UART */
    if (UARTx_Init(huart) != STD_OK)
    {
        return STD_ERROR;
    }

    /* Set RS485 Transceiver to Receive mode by default */
    RS485_SetMode_RX(hrs485);

    return STD_OK;
}

/**
  * @brief  Sets RS485 Transceiver to Transmission mode (DE/RE = Active Level).
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @retval None
  */
void RS485_SetMode_TX(RS485_HandleTypeDef *hrs485)
{
    if ((hrs485 != NULL) && (hrs485->SetPinFunc != NULL))
    {
        bool state = (hrs485->Polarity == RS485_DE_ACTIVE_HIGH) ? true : false;
        hrs485->SetPinFunc(hrs485->DE_Pin.GPIOx, hrs485->DE_Pin.GPIO_Pin, state);
    }
}

/**
  * @brief  Sets RS485 Transceiver to Reception mode (DE/RE = Inactive Level).
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @retval None
  */
void RS485_SetMode_RX(RS485_HandleTypeDef *hrs485)
{
    if ((hrs485 != NULL) && (hrs485->SetPinFunc != NULL))
    {
        bool state = (hrs485->Polarity == RS485_DE_ACTIVE_HIGH) ? false : true;
        hrs485->SetPinFunc(hrs485->DE_Pin.GPIOx, hrs485->DE_Pin.GPIO_Pin, state);
    }
}

/**
  * @brief  Transmits data over RS485 in blocking mode (Polling).
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  pData: Pointer to source data buffer.
  * @param  Size: Amount of bytes to transmit.
  * @param  Timeout: Timeout duration in milliseconds.
  * @retval HALx_StatusTypeDef: STD_OK if transmitted successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Transmit(RS485_HandleTypeDef *hrs485, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((hrs485 == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    /* 1. Switch transceiver to TX mode */
    RS485_SetMode_TX(hrs485);

    /* 2. Perform blocking transmission */
    HALx_StatusTypeDef status = UARTx_Transmit(hrs485->huart, pData, Size, Timeout);

    /* 3. Switch transceiver back to RX mode after transmission completes */
    RS485_SetMode_RX(hrs485);

    return status;
}

/**
  * @brief  Receives data over RS485 in blocking mode (Polling).
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  pData: Pointer to destination data buffer.
  * @param  Size: Amount of bytes to receive.
  * @param  Timeout: Timeout duration in milliseconds.
  * @retval HALx_StatusTypeDef: STD_OK if reception succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Receive(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if ((hrs485 == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    /* 1. Ensure transceiver is in RX mode */
    RS485_SetMode_RX(hrs485);

    /* 2. Receive data */
    return UARTx_Receive(hrs485->huart, pData, Size, Timeout);
}

/**
  * @brief  Transmits data over RS485 in non-blocking Interrupt mode.
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  pData: Pointer to source data buffer.
  * @param  Size: Amount of bytes to transmit.
  * @retval HALx_StatusTypeDef: STD_OK if IT transfer initiated, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Transmit_IT(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size)
{
    if ((hrs485 == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    RS485_SetMode_TX(hrs485);
    return UARTx_Transmit_IT(hrs485->huart, pData, Size);
}

/**
  * @brief  Receives data over RS485 in non-blocking Interrupt mode.
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  pData: Pointer to destination data buffer.
  * @param  Size: Amount of bytes to receive.
  * @retval HALx_StatusTypeDef: STD_OK if IT reception initiated, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Receive_IT(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size)
{
    if ((hrs485 == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    RS485_SetMode_RX(hrs485);
    return UARTx_Receive_IT(hrs485->huart, pData, Size);
}

/**
  * @brief  Transmits data over RS485 in non-blocking DMA mode.
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  pData: Pointer to source data buffer.
  * @param  Size: Amount of bytes to transmit.
  * @retval HALx_StatusTypeDef: STD_OK if DMA transfer initiated, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Transmit_DMA(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size)
{
    if ((hrs485 == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    RS485_SetMode_TX(hrs485);
    return UARTx_Transmit_DMA(hrs485->huart, pData, Size);
}

/**
  * @brief  Receives data over RS485 in non-blocking DMA mode.
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @param  pData: Pointer to destination data buffer.
  * @param  Size: Amount of bytes to receive.
  * @retval HALx_StatusTypeDef: STD_OK if DMA reception initiated, STD_ERROR otherwise.
  */
HALx_StatusTypeDef RS485_Receive_DMA(RS485_HandleTypeDef *hrs485, uint8_t *pData, uint16_t Size)
{
    if ((hrs485 == NULL) || (pData == NULL) || (Size == 0U))
    {
        return STD_ERROR;
    }

    RS485_SetMode_RX(hrs485);
    return UARTx_Receive_DMA(hrs485->huart, pData, Size);
}

/**
  * @brief  Callback function to be called when IT/DMA transmission completes.
  *         This function MUST be called inside `UARTx_TxCpltCallback()` to automatically return RS485 pin to RX state.
  * @param  hrs485: Pointer to RS485_HandleTypeDef structure.
  * @retval None
  */
void RS485_TxCpltCallback(RS485_HandleTypeDef *hrs485)
{
    if (hrs485 != NULL)
    {
        RS485_SetMode_RX(hrs485);
    }
}

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
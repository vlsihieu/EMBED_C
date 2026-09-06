/***********************************************************************************************************************
 * @file    hal_i2c.h
 * @brief   HAL I2C Driver — Data types, register structures, bitmasks,
 *          and API prototypes for I2C module on STM32F103xB devices.
 * 
 * @project STM32F103_BareMetal_Drivers
 * @compiler GCC
 *
 * @revision
 *          Version    Date          Author      Change History
 *          1.0.0      10/08/2026    Driver Dev  Initial release for STM32F103xB I2C Module
 *
 **********************************************************************************************************************/

#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "define.h"

/***********************************************************************************************************************
 *                                         HEADER FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             MACROS / DEFINES
 **********************************************************************************************************************/

/** 
 * @defgroup I2C_CR1_Bit_Definitions I2C Control Register 1 (CR1) Bitfields
 * @{
 */
#define I2Cx_CR1_PE_Pos             (0U)
#define I2Cx_CR1_PE                 (1U << I2Cx_CR1_PE_Pos)       /*!< Peripheral Enable */
#define I2Cx_CR1_START_Pos          (8U)
#define I2Cx_CR1_START              (1U << I2Cx_CR1_START_Pos)    /*!< Start Generation */
#define I2Cx_CR1_STOP_Pos           (9U)
#define I2Cx_CR1_STOP               (1U << I2Cx_CR1_STOP_Pos)     /*!< Stop Generation */
#define I2Cx_CR1_ACK_Pos            (10U)
#define I2Cx_CR1_ACK                (1U << I2Cx_CR1_ACK_Pos)      /*!< Acknowledge Enable */
#define I2Cx_CR1_SWRST_Pos          (15U)
#define I2Cx_CR1_SWRST              (1U << I2Cx_CR1_SWRST_Pos)    /*!< Software Reset */
/** @} */

/** 
 * @defgroup I2C_CR2_Bit_Definitions I2C Control Register 2 (CR2) Bitfields
 * @{
 */
#define I2Cx_CR2_FREQ_Pos           (0U)
#define I2Cx_CR2_FREQ_Msk          (0x3FU << I2Cx_CR2_FREQ_Pos)  /*!< Peripheral Clock Frequency Mask */
#define I2Cx_CR2_ITERREN_Pos        (8U)
#define I2Cx_CR2_ITERREN            (1U << I2Cx_CR2_ITERREN_Pos)  /*!< Error Interrupt Enable */
#define I2Cx_CR2_ITEVTEN_Pos        (9U)
#define I2Cx_CR2_ITEVTEN            (1U << I2Cx_CR2_ITEVTEN_Pos)  /*!< Event Interrupt Enable */
#define I2Cx_CR2_ITBUFEN_Pos        (10U)
#define I2Cx_CR2_ITBUFEN           (1U << I2Cx_CR2_ITBUFEN_Pos)  /*!< Buffer Interrupt Enable */
#define I2Cx_CR2_DMAEN_Pos          (11U)
#define I2Cx_CR2_DMAEN              (1U << I2Cx_CR2_DMAEN_Pos)    /*!< DMA Requests Enable */
/** @} */

/** 
 * @defgroup I2C_SR1_Bit_Definitions I2C Status Register 1 (SR1) Bitfields
 * @{
 */
#define I2Cx_SR1_SB_Pos             (0U)
#define I2Cx_SR1_SB                 (1U << I2Cx_SR1_SB_Pos)       /*!< Start Bit Flag (Master Mode) */
#define I2Cx_SR1_ADDR_Pos           (1U)
#define I2Cx_SR1_ADDR               (1U << I2Cx_SR1_ADDR_Pos)     /*!< Address Sent / Matched Flag */
#define I2Cx_SR1_BTF_Pos            (2U)
#define I2Cx_SR1_BTF                (1U << I2Cx_SR1_BTF_Pos)      /*!< Byte Transfer Finished Flag */
#define I2Cx_SR1_ADD10_Pos          (3U)
#define I2Cx_SR1_ADD10              (1U << I2Cx_SR1_ADD10_Pos)    /*!< 10-bit Header Sent Flag */
#define I2Cx_SR1_STOPF_Pos          (4U)
#define I2Cx_SR1_STOPF              (1U << I2Cx_SR1_STOPF_Pos)    /*!< Stop Detection Flag (Slave Mode) */
#define I2Cx_SR1_RxNE_Pos           (6U)
#define I2Cx_SR1_RxNE               (1U << I2Cx_SR1_RxNE_Pos)     /*!< Data Register Not Empty Flag (Receiver) */
#define I2Cx_SR1_TxE_Pos            (7U)
#define I2Cx_SR1_TxE                (1U << I2Cx_SR1_TxE_Pos)      /*!< Data Register Empty Flag (Transmitter) */
#define I2Cx_SR1_BERR_Pos           (8U)
#define I2Cx_SR1_BERR               (1U << I2Cx_SR1_BERR_Pos)     /*!< Bus Error Flag */
#define I2Cx_SR1_ARLO_Pos           (9U)
#define I2Cx_SR1_ARLO               (1U << I2Cx_SR1_ARLO_Pos)     /*!< Arbitration Lost Flag */
#define I2Cx_SR1_AF_Pos             (10U)
#define I2Cx_SR1_AF                 (1U << I2Cx_SR1_AF_Pos)       /*!< Acknowledge Failure Flag */
#define I2Cx_SR1_OVR_Pos            (11U)
#define I2Cx_SR1_OVR                (1U << I2Cx_SR1_OVR_Pos)      /*!< Overrun/Underrun Flag */
/** @} */

/** 
 * @defgroup I2C_SR2_Bit_Definitions I2C Status Register 2 (SR2) Bitfields
 * @{
 */
#define I2Cx_SR2_MSL_Pos            (0U)
#define I2Cx_SR2_MSL                 (1U << I2Cx_SR2_MSL_Pos)      /*!< Master/Slave Mode Flag */
#define I2Cx_SR2_BUSY_Pos           (2U)
#define I2Cx_SR2_BUSY               (1U << I2Cx_SR2_BUSY_Pos)     /*!< Bus Busy Flag */
#define I2Cx_SR2_TRA_Pos            (2U)
#define I2Cx_SR2_TRA                (1U << I2Cx_SR2_TRA_Pos)      /*!< Transmitter/Receiver Flag */
/** @} */

/** 
 * @defgroup I2C_CCR_Bit_Definitions I2C Clock Control Register (CCR) Bitfields
 * @{
 */
#define I2Cx_CCR_FS_Pos             (15U)
#define I2Cx_CCR_FS                 (1U << I2Cx_CCR_FS_Pos)       /*!< Fast Mode Selection */
#define I2Cx_CCR_DUTY_Pos           (14U)
#define I2Cx_CCR_DUTY               (1U << I2Cx_CCR_DUTY_Pos)     /*!< Fast Mode Duty Cycle Selection */
/** @} */

/** @brief Default timeout value for I2C blocking operations (ms) */
#define I2C_DEFAULT_TIMEOUT         (1000U)

/***********************************************************************************************************************
 *                                       TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
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
 * @brief I2C Clock Speed Mode Enum
 */
typedef enum {
    I2C_CLOCK_STANDARD = 100000U, /*!< Standard mode: 100 kHz */
    I2C_CLOCK_FAST     = 400000U  /*!< Fast mode: 400 kHz */
} I2C_ClockSpeed_t;

/**
 * @brief I2C Duty Cycle Selection Enum for Fast Mode
 */
typedef enum {
    I2C_DUTYCYCLE_2    = 0x00000000U, /*!< Fast Mode Duty Cycle Tlow/Thigh = 2 */
    I2C_DUTYCYCLE_16_9 = I2Cx_CCR_DUTY /*!< Fast Mode Duty Cycle Tlow/Thigh = 16/9 */
} I2C_DutyCycle_t;

/**
 * @brief I2C Configuration Structure
 */
typedef struct {
    uint32_t        ClockSpeed;     /*!< Clock Speed in Hz (Max 400000) */
    I2C_DutyCycle_t DutyCycle;      /*!< Duty cycle configuration in fast mode */
    uint16_t        OwnAddress1;    /*!< Own Device Address 7-bit / 10-bit */
    uint32_t        AddressingMode; /*!< 7-bit or 10-bit addressing mode */
    uint32_t        PCLK1_Freq;     /*!< APB1 Clock Frequency in Hz (e.g., 36000000U) */
} I2C_InitTypeDef;

/**
 * @brief I2C Handle Structure
 */
typedef struct {
    I2Cx_TypeDef_t      *Instance;     /*!< Register base address */
    I2C_InitTypeDef     Init;          /*!< I2C communication parameters */
    uint8_t             *pTxBuffPtr;   /*!< Pointer to Tx Transfer Buffer */
    uint16_t            TxXferSize;    /*!< Tx Transfer Size */
    volatile uint16_t   TxXferCount;   /*!< Tx Transfer Counter */
    uint8_t             *pRxBuffPtr;   /*!< Pointer to Rx Transfer Buffer */
    uint16_t            RxXferSize;    /*!< Rx Transfer Size */
    volatile uint16_t   RxXferCount;   /*!< Rx Transfer Counter */
    uint16_t            DevAddress;    /*!< Target Slave Device Address for IT/DMA */
    volatile uint32_t   ErrorCode;     /*!< I2C Error code */
    volatile uint8_t     State;         /*!< I2C State Machine Indicator */
} I2Cx_HandleTypeDef;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/**
 * @brief  Initializes the I2C peripheral according to the specified parameters in the I2C_InitTypeDef.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure containing the configuration information for the I2C module.
 * @retval HALx_StatusTypeDef: HAL status (HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT).
 */
HALx_StatusTypeDef I2Cx_Init(I2Cx_HandleTypeDef *hi2c);

/**
 * @brief  De-initializes the I2C peripheral registers to their default reset values.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure containing the configuration information for the I2C module.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_DeInit(I2Cx_HandleTypeDef *hi2c);

/* Master Polling APIs */

/**
 * @brief  Transmits an amount of data in master mode (Blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device 7-bit/10-bit address.
 * @param  pData: Pointer to data buffer.
 * @param  Size: Amount of data bytes to be sent.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Master_Transmit(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, const uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * @brief  Receives an amount of data in master mode (Blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device 7-bit/10-bit address.
 * @param  pData: Pointer to data buffer.
 * @param  Size: Amount of data bytes to be received.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Master_Receive(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/* Memory / Register Read & Write APIs */

/**
 * @brief  Write an amount of data to a specific internal memory/register address of a device (Blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device 7-bit/10-bit address.
 * @param  MemAddress: Internal memory/register address.
 * @param  MemAddSize: Size of internal memory address (e.g., 1 byte or 2 bytes).
 * @param  pData: Pointer to data buffer.
 * @param  Size: Amount of data bytes to be written.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Mem_Write(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * @brief  Read an amount of data from a specific internal memory/register address of a device (Blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device 7-bit/10-bit address.
 * @param  MemAddress: Internal memory/register address.
 * @param  MemAddSize: Size of internal memory address.
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to be read.
 * @param  Timeout: Timeout duration in milliseconds.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Mem_Read(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/* Non-Blocking Interrupt APIs */

/**
 * @brief  Transmits an amount of data in master mode with Interrupt (Non-blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device address.
 * @param  pData: Pointer to data buffer.
 * @param  Size: Amount of data bytes to be sent.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Master_Transmit_IT(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

/**
 * @brief  Receives an amount of data in master mode with Interrupt (Non-blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device address.
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to be received.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Master_Receive_IT(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

/* Non-Blocking DMA APIs */

/**
 * @brief  Transmits an amount of data in master mode with DMA (Non-blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device address.
 * @param  pData: Pointer to data buffer.
 * @param  Size: Amount of data bytes to be sent.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Master_Transmit_DMA(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

/**
 * @brief  Receives an amount of data in master mode with DMA (Non-blocking mode).
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device address.
 * @param  pData: Pointer to receive data buffer.
 * @param  Size: Amount of data bytes to be received.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_Master_Receive_DMA(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

/* Utility & Interrupt Handlers */

/**
 * @brief  Checks if target device is ready for communication.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 * @param  DevAddress: Target device address.
 * @param  Trials: Number of trials.
 * @param  Timeout: Timeout duration for each trial in milliseconds.
 * @retval HALx_StatusTypeDef: HAL status.
 */
HALx_StatusTypeDef I2Cx_IsDeviceReady(I2Cx_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);

/**
 * @brief  This function handles I2C Event interrupt request.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 */
void               I2Cx_EV_IRQHandler(I2Cx_HandleTypeDef *hi2c);

/**
 * @brief  This function handles I2C Error interrupt request.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 */
void               I2Cx_ER_IRQHandler(I2Cx_HandleTypeDef *hi2c);

/**
 * @brief  Master Tx Transfer completed callback.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 */
void               I2Cx_MasterTxCpltCallback(I2Cx_HandleTypeDef *hi2c);

/**
 * @brief  Master Rx Transfer completed callback.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 */
void               I2Cx_MasterRxCpltCallback(I2Cx_HandleTypeDef *hi2c);

/**
 * @brief  I2C error callback.
 * @param  hi2c: Pointer to a I2Cx_HandleTypeDef structure.
 */
void               I2Cx_ErrorCallback(I2Cx_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* HAL_I2C_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
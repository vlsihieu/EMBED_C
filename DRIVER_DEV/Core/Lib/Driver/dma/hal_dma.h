/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name: hal_gpio.h
 *
 * Description: HAL GPIO Driver - Data types, register structures, bitmasks,
 *              and API prototypes for the General Purpose I/O peripheral
 *              on STM32F103xB devices.
 * 
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           21/08/2026          Initial release for STM32F103xB GPIO Module
 *
 **********************************************************************************************************************/

#ifndef HAL_DMA_H_
#define HAL_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                     INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "define.h"
#include "stm32f103xb.h"
#include "hal_clock.h"

/***********************************************************************************************************************
 *                                                     MACROS
 **********************************************************************************************************************/

/* DMA Channel CCR Control Bits */
#define DMAx_CCR_EN                     (1U << 0U)

#ifndef DMAx_DIRECTION_MEM_TO_PERIPH
#define DMAx_DIRECTION_MEM_TO_PERIPH    DMAx_Direction_MEM_TO_PERIPH
#endif

#ifndef DMAx_DIRECTION_MEM_TO_MEM
#define DMAx_DIRECTION_MEM_TO_MEM       DMAx_Direction_MEM_TO_MEM
#endif

/* Interrupt Enable Bits (CCR Register) */
#ifndef DMAx_IT_TC
#define DMAx_IT_TC                      DMAx_Interupt_TC   /* Bit 1: Transfer Complete */
#endif

#ifndef DMAx_IT_HT
#define DMAx_IT_HT                      DMAx_Interupt_HT   /* Bit 2: Half Transfer */
#endif

#ifndef DMAx_IT_TE
#define DMAx_IT_TE                      DMAx_Interupt_TE   /* Bit 3: Transfer Error */
#endif

/* Circular Mode Macro */
#ifndef DMAx_MODE_CIRCULAR
#define DMAx_MODE_CIRCULAR              DMAx_Mode_CIRCULAR
#endif

/***********************************************************************************************************************
 *                                       TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief DMA Transfer Direction
 */
typedef enum 
{
    DMAx_Direction_PERIPH_TO_MEM = 0x00000000U,
    DMAx_Direction_MEM_TO_PERIPH = (1U << 4U),
    DMAx_Direction_MEM_TO_MEM    = (1U << 14U)
} DMAx_Direction_t;

/**
  * @brief DMA Peripheral Increment
  */
typedef enum 
{
    DMAx_PeriphInc_INC_DISABLE = 0x00000000U,
    DMAx_PeriphInc_INC_ENABLE  = (1U << 6U)
} DMAx_PeriphInc_t;

/**
  * @brief DMA Memory Increment
  */
typedef enum 
{
    DMAx_MenMInc_DISABLE = 0x00000000U,
    DMAx_MenMInc_ENABLE  = (1U << 7U)
} DMAx_MemInc_t;

/**
  * @brief DMA Peripheral Data Alignment
  */
typedef enum 
{
    DMAx_PeriphaDataAilgn_BYTE     = 0x00000000U,
    DMAx_PeriphaDataAilgn_HALFWORD = (1U << 8U),
    DMAx_PeriphaDataAilgn_WORD     = (2U << 8U)
} DMAx_PeriphDataAlign_t;

/**
  * @brief DMA Memory Data Alignment
  */
typedef enum 
{
    DMAx_MemDataAlign_BYTE     = 0x00000000U,
    DMAx_MemDataAlign_HALFWORD = (1U << 10U),
    DMAx_MemDataAlign_WORD     = (2U << 11U)
} DMAx_MemDataAlign_t;

/**
  * @brief DMA Operating Mode
  */
typedef enum 
{
    DMAx_Mode_NORMAL = 0x00000000U,
    DMAx_Mode_CIRCULAR = (1U << 5U)
} DMAx_Mode_t;

/**
  * @brief DMA Channel Priority Level
  */
typedef enum 
{
    DMAx_Prority_LOW       = 0x00000000U,
    DMAx_Prority_MEDIUM    = (1U << 12U),
    DMAx_Prority_HIGH      = (2U << 12U),
    DMAx_Prority_VERY_HIGH = (3U << 12U)
} DMAx_Priority_t;

/**
  * @brief DMA Interrupt Flags
  */
typedef enum 
{
    DMAx_Interupt_NONE = 0x00000000U,
    DMAx_Interupt_TC   = (1U << 1U), /* Transfer Complete Interrupt */
    DMAx_Interupt_HT   = (1U << 2U), /* Half Transfer Interrupt */
    DMAx_Interupt_TE   = (1U << 3U)  /* Transfer Error Interrupt */
} DMAx_Interrupt_t;

/**
  * @brief DMA Configuration Structure
  */
typedef struct 
{
    DMAx_Direction_t       Direction;
    DMAx_PeriphInc_t       PeriphInc;
    DMAx_MemInc_t          MemInc;
    DMAx_PeriphDataAlign_t PeriphDataAlignment;
    DMAx_MemDataAlign_t    MemDataAlignment;
    DMAx_Mode_t            Mode;
    DMAx_Priority_t        Priority;
} DMAx_InitTypeDef;

struct __DMAx_HandleTypeDef;

typedef void (*DMAx_CallbackTypeDef)(struct __DMAx_HandleTypeDef *hdma);

/**
  * @brief DMA Handle Structure
  */
typedef struct __DMAx_HandleTypeDef 
{
    DMAx_Channel_TypeDef_t *Instance;
    DMAx_InitTypeDef        Init;
    
    DMAx_CallbackTypeDef    XferCpltCallback;     /* Transfer Complete Callback */
    DMAx_CallbackTypeDef    XferHalfCpltCallback; /* Half Transfer Callback */
    DMAx_CallbackTypeDef    XferErrorCallback;    /* Transfer Error Callback */
    
    __IO uint32_t          ErrorCode;
} DMAx_HandleTypeDef;

/***********************************************************************************************************************
 *                                           GLOBAL VARIABLE DECLARATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                FUNCTION PROTOTYPES
 **********************************************************************************************************************/

HALx_StatusTypeDef DMAx_Init(DMAx_HandleTypeDef *hdma);
HALx_StatusTypeDef DMAx_DeInit(DMAx_HandleTypeDef *hdma);
HALx_StatusTypeDef DMAx_Start(DMAx_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HALx_StatusTypeDef DMAx_Start_IT(DMAx_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HALx_StatusTypeDef DMAx_Stop(DMAx_HandleTypeDef *hdma);
HALx_StatusTypeDef DMAx_Stop_IT(DMAx_HandleTypeDef *hdma);

uint16_t DMAx_GetCounter(DMAx_HandleTypeDef *hdma);
void DMAx_IRQHandler(DMAx_HandleTypeDef *hdma);

#ifdef __cplusplus
}
#endif

#endif /* HAL_DMA_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
